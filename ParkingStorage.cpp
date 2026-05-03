#include "ParkingStorage.h"
#include "Utils.h"
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
      // Kiểm tra chung: chấp nhận cả định dạng xe máy hoặc ô tô
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
    string savedPlate = v->getPlate();
    if (v->getType() == 1)
      savedPlate = v->getTicket().getId();
    if (savedPlate.empty())
      savedPlate = "NONE";
    out << v->getType() << " " << savedPlate << " "
        << v->getTicket().getDateIn() << " " << v->getTicket().getTimeIn()
        << " " << v->getTicket().getEmployeeID() << endl;
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
    string savedPlate = v->getPlate();
    if (v->getType() == 1)
      savedPlate = v->getTicket().getId();
    if (savedPlate.empty())
      savedPlate = "NONE";
    out << v->getType() << " " << savedPlate << " "
        << v->getTicket().getDateIn() << " " << v->getTicket().getTimeIn()
        << " " << v->getTicket().getDateOut() << " "
        << v->getTicket().getTimeOut() << " " << v->calculateFee() << " "
        << v->getTicket().getEmployeeID() << endl;
    temp.pop();
  }
  out.close();
  cout << "Da luu lich su xe roi bai!\n";
  loadHistoryFromFile();
}

void ParkingStorage::loadHistoryFromFile() {
  ifstream in("history.txt");
  if (!in) {
    cout << "Khong mo duoc file history!\n";
    return;
  }
  history = stack<Vehicle *>();
  revenue = 0; // Reset doanh thu để tính lại từ đầu file lịch sử
  int type, fee;
  string plate, Indate, Outdate, eid;
  time_t timeIn, timeOut;
  while (in >> type >> plate >> Indate >> timeIn >> Outdate >> timeOut >> fee >>
         eid) {
    Vehicle *v;
    if (type == 1) {
      string tid = (plate == "NONE") ? "T" : plate;
      v = new Bicycle("", tid);
    } else if (type == 2) {
      if (plate == "NONE")
        plate = "";
      v = new Motorbike(plate, "T" + plate);
    } else {
      if (plate == "NONE")
        plate = "";
      v = new Car(plate, "T" + plate);
    }
    v->getTicket()._setDateIn(Indate);
    v->getTicket().setTimeIn(timeIn);
    v->getTicket()._setDateOut(Outdate);
    v->getTicket()._setTimeOut(timeOut);
    v->getTicket().setEmployeeID(eid);
    history.push(v);
    revenue += fee; // Khôi phục doanh thu
  }
  in.close();
  // cout << "Da tai lich su tu file!\n";
}

void ParkingStorage::loadFromFile() {
  ifstream in("data.txt");
  if (!in) {
    cout << "Khong co file!\n";
    return;
  }
  while (!parkingQueue.empty())
    parkingQueue.pop();
  int type;
  string plate, inDate, eid;
  time_t inTime;
  while (in >> type >> plate >> inDate >> inTime >> eid) {
    Vehicle *v;
    if (type == 1) {
      string tid = (plate == "NONE") ? "T" : plate;
      v = new Bicycle("", tid);
      if (tid.size() > 1 && tid[0] == 'T') {
        try {
          int num = stoi(tid.substr(1));
          if (num >= ticketCounter)
            ticketCounter = num + 1;
        } catch (...) {
        }
      }
    } else if (type == 2) {
      if (plate == "NONE")
        plate = "";
      v = new Motorbike(plate, "T" + plate);
    } else {
      if (plate == "NONE")
        plate = "";
      v = new Car(plate, "T" + plate);
    }
    v->getTicket()._setDateIn(inDate);
    v->getTicket().setTimeIn(inTime);
    v->getTicket().setEmployeeID(eid);
    parkingQueue.push(v);
  }
  in.close();
  // cout << "Da tai du lieu!\n";
}

// ============================================================
//  Quản lý xe: thêm xe
// ============================================================
bool ParkingStorage::addVehicle(string empID) {

  cout << "\n---------- THEM XE VAO BAI GIU XE ----------\n";
  cout << "  1. Xe dap\n";
  cout << "  2. Xe may\n";
  cout << "  3. O to\n";
  cout << "  4. Quay lai\n";
  cout << "--------------------------------------------\n";
  cout << "  Chon loai xe: ";
  int type = Utils::readMenuChoice(1, 4);
  if (type == 4)
    return false;

  // Kiểm tra đầy chỗ
  int countBicycle, countMotorbike, countCar;
  countVehicles(countBicycle, countMotorbike, countCar);
  const int maxBicycle = 20, maxMotorbike = 40, maxCar = 30;

  bool isFull = false;
  if (type == 1 && countBicycle >= maxBicycle)
    isFull = true;
  else if (type == 2 && countMotorbike >= maxMotorbike)
    isFull = true;
  else if (type == 3 && countCar >= maxCar)
    isFull = true;

  if (isFull) {
    cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    cout << "          THONG BAO: HET CHO!\n";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    if (type == 1)
      cout << "  Xe dap da day (" << maxBicycle << "/" << maxBicycle << ")\n";
    else if (type == 2)
      cout << "  Xe may da day (" << maxMotorbike << "/" << maxMotorbike
           << ")\n";
    else
      cout << "  O to da day (" << maxCar << "/" << maxCar << ")\n";

    cout << "\n  --- Vi tri trong con lai ---\n";
    cout << "  Xe dap : " << (maxBicycle - countBicycle) << " cho trong\n";
    cout << "  Xe may : " << (maxMotorbike - countMotorbike) << " cho trong\n";
    cout << "  O to   : " << (maxCar - countCar) << " cho trong\n";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    return true;
  }

  // Bước 2: Nhập biển số
  string plate;
  if (type != 1) {
    cout << "  Vui long nhap thong tin xe:\n";
    plate = readPlate(true, type); // truyền type để validate đúng định dạng
  }

  using Factory = function<Vehicle *(const string &, const string &)>;
  const unordered_map<int, Factory> vehicleFactory = {
      {1, [](const string &p, const string &t) { return new Bicycle(p, t); }},
      {2, [](const string &p, const string &t) { return new Motorbike(p, t); }},
      {3, [](const string &p, const string &t) { return new Car(p, t); }},
  };
  string ticketID;
  if (type == 1) {
    ticketID = "T" + to_string(ticketCounter++);
  } else {
    ticketID = "T" + plate;
  }
  Vehicle *v = vehicleFactory.at(type)(plate, ticketID);
  v->getTicket().setEmployeeID(empID);

  parkingQueue.push(v);

  cout << "\n========================================";
  cout << "\n           THEM XE THANH CONG!          ";
  cout << "\n========================================\n";
  v->display();
  cout << "========================================\n";
  saveToFile();
  return true;
}

// ============================================================
//  Quản lý xe: xóa xe (xe rời bãi)
// ============================================================
bool ParkingStorage::removeVehicle() {
  while (true) {
    if (parkingQueue.empty()) {
      cout << "Bai xe rong\n";
      return true;
    }

    cout << "\n---------- XE ROI BAI ----------\n";
    cout << "  1. Nhap ma ve\n";
    cout << "  2. Nhap bien so\n";
    cout << "  3. Quay lai\n";
    cout << "--------------------------------\n";
    cout << "  Chon: ";
    int opt = Utils::readMenuChoice(1, 3);
    if (opt == 3)
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
        int fee = v->calculateFee();
        cout << "\n========================================";
        cout << "\n         XE ROI BAI THANH CONG        ";
        cout << "\n========================================\n";
        v->display();
        cout << "Ngay ra  : " << v->getTicket().getDateOut() << endl;
        cout << "Gio ra   : " << v->formatTime(v->getTicket().getTimeOut())
             << endl;
        cout << "Tien gui : " << fee << " VND\n";
        cout << "========================================\n";
        revenue += fee;

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
