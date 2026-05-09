#include "ParkingStorage.h"
#include "Utils.h"
#include <iomanip>
#include <sstream>
using namespace std;

// ============================================================
//  Đếm số xe theo loại trong bãi
// ============================================================
void ParkingStorage::countVehicles(int &countBicycle, int &countMotorbike,
                                   int &countCar) {
  countBicycle = countMotorbike = countCar = 0;
  queue<Vehicle *> temp = parkingQueue;
  while (!temp.empty()) {
    int t = temp.front()->getType();
    if (t == 1)
      ++countBicycle;
    else if (t == 2)
      ++countMotorbike;
    else if (t == 3)
      ++countCar;
    temp.pop();
  }
}

// ============================================================
//  Kiểm tra biển số trùng
// ============================================================
bool ParkingStorage::isDuplicatePlate(const string &plate) {
  queue<Vehicle *> temp = parkingQueue;
  while (!temp.empty()) {
    if (temp.front()->getPlate() == plate)
      return true;
    temp.pop();
  }
  return false;
}

// ============================================================
//  Nhập biển số hợp lệ và kiểm tra trùng biển số
// ============================================================
string ParkingStorage::readPlate(bool checkDuplicate, int vehicleType) {
  string plate;
  while (true) {
    cout << "Nhap bien so: ";
    getline(cin, plate);
    plate = Utils::normalizeString(plate);
    if (plate.empty()) {
      cout << "  [!] Khong duoc de trong!\n";
      continue;
    }

    // Validate theo loại xe
    bool valid = false;
    if (vehicleType == 2) {
      // Xe máy: 2 số + 1-2 chữ + 4-5 số, VD: 29B12345, 51G112345
      valid = Utils::isValidMotorbikePlate(plate);
      if (!valid) {
        cout << "  [-] Bien so xe may khong dung dinh dang!\n";
        cout << "      (VD hop le: 29B12345, 30AB1234)\n";
        continue;
      }
    } else if (vehicleType == 3) {
      // O to: 2 số + đúng 1 chữ cái + 4-5 số, VD: 51F1234, 29A56789
      valid = Utils::isValidCarPlate(plate);
      if (!valid) {
        cout << "  [-] Bien so o to khong dung dinh dang!\n";
        cout << "      (VD hop le: 29A12345, 51F1234)\n";
        continue;
      }
    } else {
      // Kiểm tra chung: chấp nhận cả định dạng xe máy hoac ô tô
      if (!Utils::isValidMotorbikePlate(plate) &&
          !Utils::isValidCarPlate(plate)) {
        cout << "  [-] Bien so khong dung dinh dang xe may hoac o to!\n";
        cout << "      (VD: 29B12345, 29A12345)\n";
        continue;
      }
    }

    if (checkDuplicate && isDuplicatePlate(plate)) {
      cout << "  [!] Bien so " << plate << " da ton tai trong bai!\n";
      continue;
    }
    return plate;
  }
}

// ============================================================
//  File I/O
// ============================================================
void ParkingStorage::saveToFile() {
  ofstream out("data.txt");
  queue<Vehicle *> temp = parkingQueue;
  while (!temp.empty()) {
    Vehicle *v = temp.front();
    // Lưu dưới dạng ticketID|plate (xe đạp không có plate nên là ticketID|NONE)
    string savedPlate = v->getPlate().empty() ? "NONE" : v->getPlate();
    string savedID = v->getTicket().getId();
    string col2 = savedID + "|" + savedPlate;
    out << v->getType() << " " << col2 << " " << v->getTicket().getDateIn()
        << " " << v->getTicket().getTimeIn() << " "
        << v->getTicket().getEmployeeID() << " "
        << v->getTicket().getIsMonthly() << " "
        << v->getTicket().getExpirationDate() << " "
        << v->getTicket().getSlotCode() << endl;
    temp.pop();
  }
  out.close();
  cout << "Da luu file!\n";
}

void ParkingStorage::saveHistoryToFile() {
  ofstream out("history.txt");
  if (!out) {
    cout << "Khong mo duoc file!\n";
    return;
  }
  stack<Vehicle *> temp = history;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    string savedPlate = v->getPlate().empty() ? "NONE" : v->getPlate();
    string savedID = v->getTicket().getId();
    string col2 = savedID + "|" + savedPlate;
    VehicleConfig cfgHist = getVehicleConfig(v->getType());
    out << v->getType() << " " << col2 << " " << v->getTicket().getDateIn()
        << " " << v->getTicket().getTimeIn() << " "
        << v->getTicket().getDateOut() << " " << v->getTicket().getTimeOut()
        << " " << v->calculateFee(cfgHist.dayPrice, cfgHist.nightPrice) << " "
        << v->getTicket().getEmployeeID() << " "
        << v->getTicket().getIsMonthly() << " "
        << v->getTicket().getExpirationDate() << " "
        << v->getTicket().getSlotCode() << endl;
    temp.pop();
  }
  out.close();
  // cout << "Da luu lich su xe roi bai!\n";
}

void ParkingStorage::loadHistoryFromFile(MonthlyTicketManager &monthlyManager) {
  ifstream in("history.txt");
  if (!in) {
    cout << "Khong mo duoc file history!\n";
    return;
  }
  history = stack<Vehicle *>();
  revenue = 0;
  int type, fee, monthly;
  string col2, Indate, Outdate, eid, slotCode;
  time_t timeIn, timeOut, expDate;
  while (in >> type >> col2 >> Indate >> timeIn >> Outdate >> timeOut >> fee >>
         eid >> monthly >> expDate >> slotCode) {
    // Tách col2 thành ticketID và plate
    string tid, plate;
    size_t sep = col2.find('|');
    if (sep != string::npos) {
      tid = col2.substr(0, sep);
      plate = col2.substr(sep + 1);
    } else {
      // Format cũ (tương thích ngược)
      tid = col2;
      plate = (type == 1) ? "" : col2;
    }
    if (plate == "NONE")
      plate = "";

    Vehicle *v;
    if (type == 1)
      v = new Bicycle("", tid);
    else if (type == 2)
      v = new Motorbike(plate, tid);
    else
      v = new Car(plate, tid);

    v->getTicket()._setDateIn(Indate);
    v->getTicket().setTimeIn(timeIn);
    v->getTicket()._setDateOut(Outdate);
    v->getTicket()._setTimeOut(timeOut);
    v->getTicket().setEmployeeID(eid);
    v->getTicket().setIsMonthly(monthly == 1);
    v->getTicket().setExpirationDate(expDate);
    v->getTicket().setSlotCode(slotCode);
    history.push(v);
    revenue += fee;
  }
  in.close();
}

void ParkingStorage::loadFromFile(MonthlyTicketManager &monthlyManager) {
  ifstream in("data.txt");
  if (!in) {
    cout << "Khong co file!\n";
    return;
  }
  while (!parkingQueue.empty())
    parkingQueue.pop();
  int type, monthly;
  string col2, inDate, eid, slotCode;
  time_t inTime, expDate;

  // Lambda khôi phục counter
  auto restoreCounter = [](const string &id, const string &prefix,
                           int &counter) {
    if (id.size() >= prefix.size() && id.substr(0, prefix.size()) == prefix) {
      try {
        int num = stoi(id.substr(prefix.size()));
        if (num >= counter)
          counter = num + 1;
      } catch (...) {
      }
    }
  };

  while (in >> type >> col2 >> inDate >> inTime >> eid >> monthly >> expDate >>
         slotCode) {
    // Tách col2 thành ticketID và plate
    string tid, plate;
    size_t sep = col2.find('|');
    if (sep != string::npos) {
      tid = col2.substr(0, sep);
      plate = col2.substr(sep + 1);
    } else {
      // Format cũ (tương thích ngược)
      tid = col2;
      plate = (type == 1) ? "" : col2;
    }
    if (plate == "NONE")
      plate = "";

    Vehicle *v;
    if (type == 1) {
      v = new Bicycle("", tid);
      if (monthly == 0)
        restoreCounter(tid, "VXD", counterVXD);
    } else if (type == 2) {
      v = new Motorbike(plate, tid);
      if (monthly == 0)
        restoreCounter(tid, "VXM", counterVXM);
    } else {
      v = new Car(plate, tid);
      if (monthly == 0)
        restoreCounter(tid, "VOT", counterVOT);
    }
    v->getTicket()._setDateIn(inDate);
    v->getTicket().setTimeIn(inTime);
    v->getTicket().setEmployeeID(eid);
    v->getTicket().setIsMonthly(monthly == 1);
    v->getTicket().setExpirationDate(expDate);
    v->getTicket().setSlotCode(slotCode);
    if (slotCode != "N/A") {
      updateSlotStatus(slotCode, SlotStatus::Occupied);
    }
    parkingQueue.push(v);
  }
  in.close();
}

// ============================================================
//  Quản lý xe: thêm xe
// ============================================================
bool ParkingStorage::addVehicle(string empID,
                                MonthlyTicketManager &monthlyManager) {

  cout << "\n---------- THEM XE VAO BAI GIU XE ----------\n";
  cout << "  1. Xe dap\n";
  cout << "  2. Xe may\n";
  cout << "  3. O to\n";
  cout << "  0. Quay lai\n";
  cout << "--------------------------------------------\n";
  cout << "  Chon loai xe: ";
  int type = Utils::readMenuChoice(0, 3);
  if (type == 0)
    return false;

  // Kiểm tra đầy chỗ
  int countBicycle, countMotorbike, countCar;
  countVehicles(countBicycle, countMotorbike, countCar);

  VehicleConfig cfg = getVehicleConfig(type);
  int currentCount =
      (type == 1 ? countBicycle : (type == 2 ? countMotorbike : countCar));

  if (currentCount >= cfg.maxCapacity) {
    cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    cout << "          THONG BAO: HET CHO!\n";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    string typeName = (type == 1 ? "Xe dap" : (type == 2 ? "Xe may" : "O to"));
    cout << "  " << typeName << " da day (" << currentCount << "/"
         << cfg.maxCapacity << ")\n";

    cout << "\n  --- Vi tri trong con lai ---\n";
    cout << "  Xe dap : " << (getVehicleConfig(1).maxCapacity - countBicycle)
         << " cho trong\n";
    cout << "  Xe may : " << (getVehicleConfig(2).maxCapacity - countMotorbike)
         << " cho trong\n";
    cout << "  O to   : " << (getVehicleConfig(3).maxCapacity - countCar)
         << " cho trong\n";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    return true;
  }

  // Bước 2: Nhập thông tin định danh
  string identifier;
  int status =
      0; // 0: No ticket, 1: Valid, 2: Expired, 3: Expiring soon, 4: Locked
  string plate = "";

  if (type == 1) {
    while (true) {
      cout << "  Nhap ma ve thang (1=Co, 2=Khong): ";
      string choice;
      getline(cin, choice);
      if (choice == "1") {
        cout << "  Nhap ma ve: ";
        getline(cin, identifier);
        identifier = Utils::normalizeString(identifier);
        if (!identifier.empty()) {
          if (isVehicleInLot(identifier)) {
            cout << "  [!] Ve thang nay hien dang duoc su dung trong bai!\n";
            continue; // Go back to '1=Co, 2=Khong'
          }
          status = monthlyManager.checkTicketByID(identifier);
        }
        break;
      } else if (choice == "2") {
        identifier = "";
        break;
      } else {
        cout << "  [!] Lua chon khong hop le!\n";
      }
    }
  } else {
    cout << "  Vui long nhap thong tin xe:\n";
    plate = readPlate(true, type);
    identifier = plate;
    status = monthlyManager.checkTicket(plate);
  }

  using Factory = function<Vehicle *(const string &, const string &)>;
  const unordered_map<int, Factory> vehicleFactory = {
      {1, [](const string &p, const string &t) { return new Bicycle(p, t); }},
      {2, [](const string &p, const string &t) { return new Motorbike(p, t); }},
      {3, [](const string &p, const string &t) { return new Car(p, t); }},
  };

  string ticketID;
  if (status == 1 || status == 3) {
    // Có vé tháng hợp lệ: dùng mã vé tháng
    if (type == 1) {
      ticketID = identifier;
    } else {
      ticketID = monthlyManager.getTicketID(plate);
    }
  } else {
    // Vé lượt: sinh mã theo định dạng VXD/VXM/VOT
    auto formatID = [](const string &prefix, int &counter) {
      stringstream ss;
      ss << prefix << setfill('0') << setw(3) << counter++;
      return ss.str();
    };
    if (type == 1)
      ticketID = formatID("VXD", counterVXD);
    else if (type == 2)
      ticketID = formatID("VXM", counterVXM);
    else
      ticketID = formatID("VOT", counterVOT);
  }

  Vehicle *v = vehicleFactory.at(type)(plate, ticketID);
  v->getTicket().setEmployeeID(empID);

  // Bước 3: Kiểm tra trạng thái vé và áp dụng loại vé (Theo luồng xử lý mới)
  if (status == 1 || status == 3) {
    // CÓ VÉ THÁNG & CÒN HẠN -> MIỄN PHÍ
    if (status == 3)
      cout << "\n  [!] Canh bao: Ve thang SAP HET HAN!\n";
    v->getTicket().setIsMonthly(true);
    if (type == 1)
      v->getTicket().setExpirationDate(
          monthlyManager.getExpirationDateByID(identifier));
    else
      v->getTicket().setExpirationDate(monthlyManager.getExpirationDate(plate));
  } else {
    // KHÔNG CÓ VÉ THÁNG HOẶC HẾT HẠN -> VÉ LƯỢT
    if (status == 2)
      cout << "\n  [!] Thong bao: Ve thang cua xe nay da HET HAN!\n";
    else if (status == 4)
      cout << "\n  [!] Thong bao: Ve thang cua xe nay hien dang BI KHOA!\n";

    v->getTicket().setIsMonthly(false);
  }

  // Phân bổ vị trí
  string allocatedSlot =
      allocateSlot(type, plate, v->getTicket().getIsMonthly());
  v->getTicket().setSlotCode(allocatedSlot);
  if (allocatedSlot == "N/A") {
    cout << "  [!] CANH BAO: Khong tim thay vi tri trong phu hop!\n";
  }

  parkingQueue.push(v);

  cout << "\n========================================";
  cout << "\n           THEM XE THANH CONG!          ";
  cout << "\n========================================\n";
  VehicleConfig cfg2 = getVehicleConfig(v->getType());
  v->display(cfg2.dayPrice, cfg2.nightPrice);
  cout << "========================================\n";
  saveToFile();
  return true;
}

// ============================================================
//  Quản lý xe: xóa xe (xe rời bãi)
// ============================================================
string ParkingStorage::getDurationString(time_t start, time_t end) {
  if (start > end)
    return "0 phut";
  long diff = (long)difftime(end, start);
  int days = diff / 86400;
  diff %= 86400;
  int hours = diff / 3600;
  diff %= 3600;
  int minutes = diff / 60;

  string res = "";
  if (days > 0)
    res += to_string(days) + " ngay ";
  if (hours > 0 || days > 0)
    res += to_string(hours) + " gio ";
  res += to_string(minutes) + " phut";
  return res;
}

bool ParkingStorage::removeVehicle(string empID) {
  while (true) {
    if (parkingQueue.empty()) {
      cout << "Bai xe rong\n";
      return true;
    }

    cout << "\n---------- XE ROI BAI ----------\n";
    cout << "  1. Nhap ma ve\n";
    cout << "  2. Nhap bien so\n";
    cout << "  0. Quay lai\n";
    cout << "--------------------------------\n";
    cout << "  Chon: ";
    int opt = Utils::readMenuChoice(0, 2);
    if (opt == 0)
      return false;

    string inputStr;
    if (opt == 1) {
      while (true) {
        cout << "  Nhap ma ve: ";
        getline(cin, inputStr);
        inputStr = Utils::normalizeString(inputStr);
        if (inputStr.empty()) {
          cout << "  [!] Khong duoc de trong!\n";
          continue;
        }
        if (Utils::hasInvalidChar(inputStr)) {
          cout << "  [!] Ma ve chi duoc chua chu cai hoac so (khong khoang "
                  "trang/ky tu dac biet)!\n";
          continue;
        }
        break;
      }
    } else {
      inputStr = readPlate(false);
    }

    queue<Vehicle *> temp;
    bool found = false;
    while (!parkingQueue.empty()) {
      Vehicle *v = parkingQueue.front();
      parkingQueue.pop();

      bool isMatch = false;
      if (opt == 1 && v->getTicket().getId() == inputStr) {
        isMatch = true;
      } else if (opt == 2 && v->getPlate() == inputStr &&
                 !v->getPlate().empty()) {
        isMatch = true;
      }

      if (!found && isMatch) {
        v->getTicket().setTimeOut();
        v->getTicket().setEmployeeID(empID);
        VehicleConfig cfgOut = getVehicleConfig(v->getType());
        int fee = v->calculateFee(cfgOut.dayPrice, cfgOut.nightPrice);
        cout << "\n========================================";
        cout << "\n         XE ROI BAI THANH CONG        ";
        cout << "\n========================================\n";
        v->display(cfgOut.dayPrice, cfgOut.nightPrice);
        cout << "Ngay ra  : " << v->getTicket().getDateOut() << endl;
        cout << "Gio ra   : " << v->formatTime(v->getTicket().getTimeOut())
             << endl;
        cout << "T.Gian   : "
             << getDurationString(v->getTicket().getTimeIn(),
                                  v->getTicket().getTimeOut())
             << endl;

        if (v->getTicket().getIsMonthly() &&
            v->getTicket().getTimeOut() > v->getTicket().getExpirationDate()) {
          cout << "  [!] Chu y: Ve thang da het han luc: "
               << v->formatTime(v->getTicket().getExpirationDate()) << endl;
          cout << "  [!] Tien gui phat sinh tinh tu luc het han den khi ra.\n";
        }

        cout << "Tien gui : " << fee << " VND\n";
        cout << "========================================\n";
        revenue += fee;

        // Giải phóng vị trí
        releaseSlot(v->getTicket().getSlotCode());

        history.push(v);
        found = true;
      } else {
        temp.push(v);
      }
    }
    parkingQueue = temp;

    if (found) {
      saveToFile();
      saveHistoryToFile();
      return true;
    } else {
      if (opt == 1)
        cout << "\n  [-] Khong tim thay xe co ma ve: " << inputStr << "\n";
      else
        cout << "\n  [-] Khong tim thay xe co bien so: " << inputStr << "\n";

      cout << "\n  1. Tiep tuc nhap lai\n";
      cout << "  2. Quay lai menu\n";
      cout << "  Chon: ";
      int retryOpt = Utils::readMenuChoice(1, 2);
      if (retryOpt == 2)
        return false;
    }
  }
}

// ============================================================
//  Quản lý vị trí: Khởi tạo bãi xe
// ============================================================
void ParkingStorage::initSlots() {
  zones.clear();

  // Zone A: Xe máy (40 slots)
  ParkingZone zoneA("Khu A (Xe may)", 2);
  for (int i = 1; i <= 40; ++i) {
    string code = "A" + (i < 10 ? string("0") : string("")) + to_string(i);
    zoneA.addSlot(code, i);
  }
  zones.push_back(zoneA);

  // Zone B: Ô tô (30 slots)
  ParkingZone zoneB("Khu B (O to)", 3);
  for (int i = 1; i <= 30; ++i) {
    string code = "B" + (i < 10 ? string("0") : string("")) + to_string(i);
    zoneB.addSlot(code, i);
  }
  zones.push_back(zoneB);

  // Zone C: Xe đạp (20 slots)
  ParkingZone zoneC("Khu C (Xe dap)", 1);
  for (int i = 1; i <= 20; ++i) {
    string code = "C" + (i < 10 ? string("0") : string("")) + to_string(i);
    zoneC.addSlot(code, i);
  }
  zones.push_back(zoneC);
}

string ParkingStorage::allocateSlot(int vehicleType, const string &plate,
                                    bool isMonthly) {
  for (auto &zone : zones) {
    if (zone.vehicleType == vehicleType) {
      for (auto &slot : zone.slots) {
        if (slot.status == SlotStatus::Available) {
          slot.status = SlotStatus::Occupied;
          slot.plate = plate;
          return slot.code;
        }
      }
    }
  }
  return "N/A";
}

void ParkingStorage::releaseSlot(const string &slotCode) {
  for (auto &zone : zones) {
    for (auto &slot : zone.slots) {
      if (slot.code == slotCode) {
        slot.status = SlotStatus::Available;
        slot.plate = "";
        return;
      }
    }
  }
}

void ParkingStorage::updateSlotStatus(const string &slotCode,
                                      SlotStatus status) {
  for (auto &zone : zones) {
    for (auto &slot : zone.slots) {
      if (slot.code == slotCode) {
        slot.status = status;
        return;
      }
    }
  }
}

// ============================================================
//  Quản lý cấu hình (Giá & Sức chứa)
// ============================================================
void ParkingStorage::setVehicleConfig(int type, int day, int night, int monthly,
                                      int cap) {
  configs[type] = {day, night, monthly, cap};
  saveConfig();
}

VehicleConfig ParkingStorage::getVehicleConfig(int type) {
  if (configs.find(type) == configs.end()) {
    // Mặc định nếu chưa có
    if (type == 1)
      return {2000, 3000, 50000, 20};
    if (type == 2)
      return {5000, 10000, 200000, 40};
    if (type == 3)
      return {20000, 30000, 1000000, 30};
  }
  return configs[type];
}

void ParkingStorage::showConfigs() {
  cout << "\n=== CAU HINH GIA VE & SUC CHUA ===\n";
  cout << left << setw(10) << "Loai xe" << " | " << right << setw(10)
       << "Gia Ngay" << " | " << right << setw(10) << "Gia Dem" << " | "
       << right << setw(10) << "Ve Thang" << " | " << right << setw(10)
       << "Suc chua" << endl;
  cout << "--------------------------------------------------------------------"
          "--------\n";
  vector<string> names = {"", "Xe dap", "Xe may", "O to"};
  for (int i = 1; i <= 3; ++i) {
    VehicleConfig cfg = getVehicleConfig(i);
    cout << left << setw(10) << names[i] << " | " << right << setw(10)
         << cfg.dayPrice << " | " << right << setw(10) << cfg.nightPrice
         << " | " << right << setw(10) << cfg.monthlyPrice << " | " << right
         << setw(10) << cfg.maxCapacity << endl;
  }
}

void ParkingStorage::saveConfig() {
  ofstream out("config.txt");
  for (auto const &it : configs) {
    out << it.first << " " << it.second.dayPrice << " " << it.second.nightPrice
        << " " << it.second.monthlyPrice << " " << it.second.maxCapacity
        << endl;
  }
  out.close();
}

void ParkingStorage::loadConfig() {
  ifstream in("config.txt");
  if (!in) {
    // Khởi tạo mặc định nếu chưa có file
    configs[1] = {2000, 3000, 50000, 20};
    configs[2] = {5000, 10000, 200000, 40};
    configs[3] = {20000, 30000, 1000000, 30};
    return;
  }
  int type, day, night, monthly, cap;
  while (in >> type >> day >> night >> monthly >> cap) {
    configs[type] = {day, night, monthly, cap};
  }
  in.close();
}

bool ParkingStorage::isVehicleInLot(const string &identifier) {
  queue<Vehicle *> temp = parkingQueue;
  string upperID = Utils::normalizeString(identifier);
  while (!temp.empty()) {
    Vehicle *v = temp.front();
    temp.pop();
    if (Utils::normalizeString(v->getPlate()) == upperID ||
        Utils::normalizeString(v->getTicket().getId()) == upperID) {
      return true;
    }
  }
  return false;
}
