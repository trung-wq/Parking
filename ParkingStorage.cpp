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
//  Đếm số xe vé tháng hiện có trong bãi theo loại
// ============================================================
int ParkingStorage::countMonthlyInLot(int type) {
  int count = 0;
  queue<Vehicle *> temp = parkingQueue;
  while (!temp.empty()) {
    if (temp.front()->getType() == type &&
        temp.front()->getTicket().getIsMonthly()) {
      count++;
    }
    temp.pop();
  }
  return count;
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
        << v->getTicket().getSlotCode() << " "
        << (v->getTicket().getUpgradedMidStay() ? 1 : 0) << endl;
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

  string line;
  while (getline(in, line)) {
    if (line.empty())
      continue;
    stringstream ss(line);
    if (!(ss >> type >> col2 >> inDate >> inTime >> eid >> monthly >> expDate >>
          slotCode))
      continue;

    int upgraded = 0;
    ss >> upgraded; // Sẽ lấy 0 nếu file cũ không có cờ này
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
    v->getTicket().setUpgradedMidStay(upgraded == 1);
    if (slotCode != "N/A") {
      updateSlotStatus(slotCode, SlotStatus::Occupied);
    }
    parkingQueue.push(v);
  }
  in.close();
}

// ============================================================
//  Nâng cấp xe vé lượt đang trong bãi → vé tháng (mid-stay)
//  Đặt isMonthly = true, expirationDate = thời điểm đăng ký (T2)
//  → calculateFee sẽ tính phí từ T2 đến lúc ra (như vé tháng quá hạn)
//  → Thời gian T1 (vào) đến T2 (đăng ký) được miễn phí
// ============================================================
bool ParkingStorage::upgradeVehicleToMonthly(const string &plate,
                                             const string &ticketID,
                                             time_t registrationTime,
                                             const string &newMonthlyTicketID) {
  queue<Vehicle *> temp = parkingQueue;
  bool found = false;
  while (!temp.empty()) {
    Vehicle *v = temp.front();
    temp.pop();
    bool matchPlate =
        !plate.empty() && plate != "NONE" && v->getPlate() == plate;
    bool matchTicket = !ticketID.empty() && v->getTicket().getId() == ticketID;
    if ((matchPlate || matchTicket) && !v->getTicket().getIsMonthly()) {
      v->getTicket().setIsMonthly(true);
      v->getTicket().setUpgradedMidStay(true);
      v->getTicket().setExpirationDate(registrationTime); // T2
      if (!newMonthlyTicketID.empty()) {
        v->getTicket().setId(newMonthlyTicketID);
      }
      found = true;
      break;
    }
  }
  if (found)
    saveToFile();
  return found;
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

  // Thu thập thông tin sức chứa và số lượng xe hiện tại
  int countBicycle, countMotorbike, countCar;
  countVehicles(countBicycle, countMotorbike, countCar);
  VehicleConfig cfg = getVehicleConfig(type);
  int currentCount =
      (type == 1 ? countBicycle : (type == 2 ? countMotorbike : countCar));

  // Bước 2: Nhập thông tin định danh
  string identifier;
  int status =
      0; // 0: No ticket, 1: Valid, 2: Expired, 3: Expiring soon, 4: Locked
  string plate = "";

  if (type == 1) {
    while (true) {
      cout << "  Su dung ve thang? (1: Co, 2: Khong): ";
      string choice;
      getline(cin, choice);
      if (choice == "1") {
        cout << "  Nhap ma ve: ";
        getline(cin, identifier);
        identifier = Utils::normalizeString(identifier);
        if (identifier.empty())
          continue;

        if (isVehicleInLot(identifier)) {
          cout << "  [!] Ve thang nay hien dang duoc su dung trong bai!\n";
          continue;
        }
        status = monthlyManager.checkTicketByID(identifier);
        if (status == 0) {
          cout << "  [!] Khong tim thay ve thang '" << identifier << "'!\n";
          continue;
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
    while (true) {
      cout << "  Su dung ve thang? (1: Co, 2: Khong): ";
      string choice;
      getline(cin, choice);
      if (choice == "1") {
        cout << "  Nhap ma ve hoac bien so: ";
        getline(cin, identifier);
        identifier = Utils::normalizeString(identifier);
        if (identifier.empty())
          continue;

        if (isVehicleInLot(identifier)) {
          cout << "  [!] Xe/Ve nay hien dang duoc su dung trong bai!\n";
          continue;
        }

        // Kiểm tra theo biển số trước, sau đó theo mã vé
        status = monthlyManager.checkTicket(identifier);
        if (status != 0) {
          plate = identifier;
        } else {
          status = monthlyManager.checkTicketByID(identifier);
          if (status != 0) {
            plate = monthlyManager.getPlateByID(identifier);
          }
        }

        if (status == 0) {
          cout << "  [!] Khong tim thay ve thang '" << identifier << "'!\n";
          continue;
        }
        break;
      } else if (choice == "2") {
        plate = readPlate(true, type);
        identifier = plate;
        status = 0;
        break;
      } else {
        cout << "  [!] Lua chon khong hop le!\n";
      }
    }
  }

  using Factory = function<Vehicle *(const string &, const string &)>;
  const unordered_map<int, Factory> vehicleFactory = {
      {1, [](const string &p, const string &t) { return new Bicycle(p, t); }},
      {2, [](const string &p, const string &t) { return new Motorbike(p, t); }},
      {3, [](const string &p, const string &t) { return new Car(p, t); }},
  };

  Vehicle *v = vehicleFactory.at(type)(
      plate, (identifier.empty() ? "" : identifier)); // Placeholder ID
  v->getTicket().setEmployeeID(empID);

  // --- KIỂM TRA SỨC CHỨA VÀ ĐIỀU KIỆN VÀO BÃI ---
  bool isMonthly = (status == 1 || status == 3);

  if (isMonthly) {
    // Khách vé tháng: Chỉ chặn nếu bãi ĐÃ ĐẦY THỰC TẾ
    if (currentCount >= cfg.maxCapacity) {
      cout << "\n  [!] LOI: Bai xe da day cung (" << currentCount << "/"
           << cfg.maxCapacity << ")!\n";
      cout << "      Xin loi quy khach ve thang, vui long quay lai sau.\n";
      delete v;
      return true;
    }
    if (status == 3)
      cout << "\n  [!] Canh bao: Ve thang SAP HET HAN!\n";
    v->getTicket().setIsMonthly(true);
    v->getTicket().setExpirationDate(
        type == 1 ? monthlyManager.getExpirationDateByID(identifier)
                  : monthlyManager.getExpirationDate(plate));

    // Lấy lại Ticket ID chuẩn cho vé tháng (XM001, ...)
    string realTID =
        (type == 1 ? identifier : monthlyManager.getTicketID(plate));
    v->getTicket().setId(realTID);
  } else {
    // Khách vé lượt: Chặn nếu hết chỗ HOẶC hết chỗ dự phòng cho vé tháng
    int monthlyInLot = countMonthlyInLot(type);
    int totalMonthly = monthlyManager.countTicketsByType(type);
    int monthlyOut = totalMonthly - monthlyInLot; // Số vé tháng chưa vào bãi

    if (currentCount + monthlyOut >= cfg.maxCapacity) {
      cout << "\n  [!] THONG BAO: HET CHO CHO XE LUOT!\n";
      cout
          << "      Cac vi tri con lai dang duoc uu tien cho khach ve thang.\n";
      delete v;
      return true;
    }

    if (status == 2)
      cout << "\n  [!] Thong bao: Ve thang cua xe nay da HET HAN!\n";
    else if (status == 4)
      cout << "\n  [!] Thong bao: Ve thang cua xe nay hien dang BI KHOA!\n";

    v->getTicket().setIsMonthly(false);

    // Sinh mã vé lượt
    auto formatID = [](const string &prefix, int &counter) {
      stringstream ss;
      ss << prefix << setfill('0') << setw(3) << counter++;
      return ss.str();
    };
    string ticketID;
    if (type == 1)
      ticketID = formatID("VXD", counterVXD);
    else if (type == 2)
      ticketID = formatID("VXM", counterVXM);
    else
      ticketID = formatID("VOT", counterVOT);

    v->getTicket().setId(ticketID);
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
  cout << "\n         PHIEU GUI XE VAO BAI         ";
  cout << "\n========================================\n";

  // Nhóm 1: Thông tin xe
  string typeStr =
      (v->getType() == 1 ? "Xe dap" : (v->getType() == 2 ? "Xe may" : "O to"));
  cout << "  Loai xe    : " << typeStr << endl;
  if (!v->getPlate().empty() && v->getPlate() != "NONE") {
    cout << "  Bien so    : " << v->getPlate() << endl;
  }
  cout << "  Ma ve      : " << v->getTicket().getId() << " ("
       << (v->getTicket().getIsMonthly() ? "VE THANG" : "VE LUOT") << ")"
       << endl;
  cout << "  Vi tri     : " << v->getTicket().getSlotCode() << endl;

  cout << "  ------------------------------------" << endl;

  // Nhóm 2: Thời gian & Đơn giá
  cout << "  Ngay vao   : " << v->getTicket().getDateIn() << endl;
  cout << "  Gio vao    : " << v->formatTime(v->getTicket().getTimeIn())
       << endl;

  VehicleConfig cfg2 = getVehicleConfig(v->getType());
  cout << "  Don gia    : " << cfg2.dayPrice << " / " << cfg2.nightPrice
       << " (Ngay/Dem)" << endl;

  cout << "  ------------------------------------" << endl;
  cout << "  Nhan vien  : " << empID << endl;
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
        cout << "\n         HOA DON XE ROI BAI           ";
        cout << "\n========================================\n";

        // Nhóm 1: Thông tin xe
        string typeStr =
            (v->getType() == 1 ? "Xe dap"
                               : (v->getType() == 2 ? "Xe may" : "O to"));
        cout << "  Loai xe    : " << typeStr << endl;
        if (!v->getPlate().empty() && v->getPlate() != "NONE") {
          cout << "  Bien so    : " << v->getPlate() << endl;
        }
        cout << "  Ma ve      : " << v->getTicket().getId() << " ("
             << (v->getTicket().getIsMonthly() ? "VE THANG" : "VE LUOT") << ")"
             << endl;
        cout << "  Vi tri     : " << v->getTicket().getSlotCode() << endl;

        cout << "  ------------------------------------" << endl;

        // Nhóm 2: Thời gian
        cout << "  Thoi gian vao: " << v->getTicket().getDateIn() << " "
             << v->formatTime(v->getTicket().getTimeIn()) << endl;
        cout << "  Thoi gian ra : " << v->getTicket().getDateOut() << " "
             << v->formatTime(v->getTicket().getTimeOut()) << endl;
        cout << "  Tong t.gian  : "
             << getDurationString(v->getTicket().getTimeIn(),
                                  v->getTicket().getTimeOut())
             << endl;

        cout << "  ------------------------------------" << endl;

        // Nhóm 3: Thanh toán
        cout << "  Don gia    : " << cfgOut.dayPrice << " / "
             << cfgOut.nightPrice << " (Ngay/Dem)" << endl;

        if (v->getTicket().getIsMonthly()) {
          if (v->getTicket().getUpgradedMidStay()) {
            time_t expiry = v->getTicket().getExpirationDate();
            cout << "  [*] Xe dang ky VE THANG luc " << v->formatTime(expiry)
                 << endl;
            cout << "  [*] Chi tinh phi tu luc vao bai den luc dang ky ve "
                    "thang.\n";
            cout << "  [*] Thoi gian tu luc dang ky ve thang den luc ra: MIEN "
                    "PHI.\n";
          } else if (v->getTicket().getTimeOut() >
                     v->getTicket().getExpirationDate()) {
            time_t expiry = v->getTicket().getExpirationDate();
            cout << "  [!] Chu y: Ve thang het han luc "
                 << v->formatTime(expiry) << endl;
            cout << "  [!] Phat sinh phi tu luc het han.\n";
          }
        }

        cout << "  THANH TOAN : " << fee << " VND" << endl;
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
