#include "MonthlyTicketManager.h"
#include "ParkingStorage.h"
#include "Utils.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

MonthlyTicketManager::MonthlyTicketManager() { loadFromFile(); }

void MonthlyTicketManager::loadFromFile() {
  tickets.clear();
  ifstream f(filename);
  if (!f.is_open())
    return;

  string line;
  while (getline(f, line)) {
    if (line.empty())
      continue;
    stringstream ss(line);
    MonthlyTicket t;
    int locked;

    ss >> t.ticketID >> t.plate >> t.vehicleType >> t.registrationDate >>
        t.expirationDate >> t.price >> locked;
    ss.ignore();
    getline(ss, t.ownerName, '|');
    getline(ss, t.phoneNumber);
    t.isLocked = (locked == 1);

    // Cập nhật counter dựa trên ID cao nhất
    if (t.ticketID.length() >= 5) {
      try {
        int num = stoi(t.ticketID.substr(2));
        if (t.ticketID.substr(0, 2) == "XD" && num >= counterXD)
          counterXD = num + 1;
        else if (t.ticketID.substr(0, 2) == "XM" && num >= counterXM)
          counterXM = num + 1;
        else if (t.ticketID.substr(0, 2) == "OT" && num >= counterOT)
          counterOT = num + 1;
      } catch (...) {
      }
    }

    tickets.push_back(t);
  }
  f.close();
}

void MonthlyTicketManager::saveToFile() {
  ofstream f(filename);
  for (const auto &t : tickets) {
    f << t.ticketID << " " << t.plate << " " << t.vehicleType << " "
      << t.registrationDate << " " << t.expirationDate << " " << t.price << " "
      << (t.isLocked ? 1 : 0) << " " << t.ownerName << "|" << t.phoneNumber
      << endl;
  }
  f.close();
}

void MonthlyTicketManager::logRevenue(const MonthlyTicket &t, string empID) {
  ofstream f("monthly_revenue.txt", ios::app);
  time_t now = time(0);
  tm *info = localtime(&now);
  char dateBuf[20];
  strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", info);

  f << t.vehicleType << " " << t.plate << " " << dateBuf << " " << t.price
    << " " << empID << endl;
  f.close();
}

void MonthlyTicketManager::addTicket(string empID, ParkingStorage &storage) {
  MonthlyTicket t;
  cout << "\n--- DANG KY VE THANG ---" << endl;
  cout << "Chon loai xe (1: Xe dap, 2: Xe may, 3: O to, 0: Quay lai): ";
  t.vehicleType = Utils::readMenuChoice(0, 3);

  if (t.vehicleType == 0) {
    return;
  }
  string regularTicketIDToUpgrade = "";
  if (t.vehicleType == 1) {
    t.plate = "NONE";
    cout << "  Xe dap nay hien dang gui trong bai bang ve luot khong? (1: Co, 2: Khong): ";
    int choice = Utils::readMenuChoice(1, 2);
    if (choice == 1) {
      cout << "  Nhap ma ve luot (VD: VXD001): ";
      cin >> regularTicketIDToUpgrade;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      regularTicketIDToUpgrade = Utils::normalizeString(regularTicketIDToUpgrade);
    }
  } else {
    while (true) {
      cout << "  Nhap bien so: ";
      cin >> t.plate;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      t.plate = Utils::normalizeString(t.plate);

      if (Utils::hasInvalidChar(t.plate)) {
        cout << "  [!] Bien so khong duoc chua ky tu dac biet!\n";
        continue;
      }

      bool valid = (t.vehicleType == 2) ? Utils::isValidMotorbikePlate(t.plate)
                                        : Utils::isValidCarPlate(t.plate);
      if (!valid) {
        cout << "  [!] Bien so khong dung dinh dang! (VD: "
             << (t.vehicleType == 2 ? "29B12345" : "29A12345") << ")\n";
        continue;
      }

      // Check if exists
      bool exists = false;
      for (const auto &exist : tickets) {
        if (exist.plate == t.plate && exist.plate != "NONE") {
          exists = true;
          break;
        }
      }
      if (exists) {
        cout << "  [!] Bien so nay da dang ky ve thang!\n";
        continue;
      }
      break;
    }
  }

  while (true) {
    cout << "  Nhap ten chu xe: ";
    getline(cin, t.ownerName);
    if (Utils::isValidName(t.ownerName))
      break;
    cout << "  [!] Ten khong hop le (khong duoc de trong, toi da 50 ky tu)!\n";
  }

  while (true) {
    cout << "  Nhap so dien thoai: ";
    getline(cin, t.phoneNumber);
    if (Utils::isValidPhone(t.phoneNumber))
      break;
    cout << "  [!] So dien thoai khong hop le (phai co toi thieu 10 chu so)!\n";
  }

  // Sinh ID Ve
  stringstream ss;
  if (t.vehicleType == 1) {
    ss << "XD" << setfill('0') << setw(3) << counterXD++;
  } else if (t.vehicleType == 2) {
    ss << "XM" << setfill('0') << setw(3) << counterXM++;
  } else {
    ss << "OT" << setfill('0') << setw(3) << counterOT++;
  }
  t.ticketID = ss.str();

  VehicleConfig cfg = storage.getVehicleConfig(t.vehicleType);

  // Kiểm tra sức chứa tổng hợp (Vé tháng đã đăng ký + Xe lượt đang trong bãi)
  int registeredTickets = countTicketsByType(t.vehicleType);
  int countB, countM, countC;
  storage.countVehicles(countB, countM, countC);
  int totalInLot =
      (t.vehicleType == 1 ? countB : (t.vehicleType == 2 ? countM : countC));
  int monthlyInLot = storage.countMonthlyInLot(t.vehicleType);
  int regularInLot = totalInLot - monthlyInLot;

  if (registeredTickets + regularInLot >= cfg.maxCapacity) {
    string typeName =
        (t.vehicleType == 1 ? "Xe dap"
                            : (t.vehicleType == 2 ? "Xe may" : "O to"));
    cout << "\n  [!] LOI: Khong the dang ky them ve thang cho " << typeName
         << "!" << endl;
    cout << "      Tong so cho da dat truoc (" << registeredTickets
         << ") va xe luot dang do (" << regularInLot << ") " << endl;
    cout << "      da dat gioi han suc chua cua bai (" << cfg.maxCapacity
         << ")." << endl;
    return;
  }

  t.price = cfg.monthlyPrice;

  t.registrationDate = time(0);
  tm *ltm = localtime(&t.registrationDate);
  ltm->tm_mon += 1;
  ltm->tm_mday = 1;
  ltm->tm_hour = 0;
  ltm->tm_min = 0;
  ltm->tm_sec = 0;
  t.expirationDate = mktime(ltm) - 1;
  t.isLocked = false;

  tickets.push_back(t);
  saveToFile();
  logRevenue(t, empID);

  // --- Nâng cấp xe đang trong bãi (vé lượt → vé tháng mid-stay) ---
  // expirationDate của ticket trong bãi = registrationDate (T2)
  // → khi ra: tính phí từ T2 đến lúc ra (như quá hạn), T1→T2 miễn phí
  bool upgraded = false;
  if (t.vehicleType == 1 && !regularTicketIDToUpgrade.empty()) {
    // Với xe đạp, tìm bằng mã vé lượt và đổi mã vé thành mã vé tháng mới
    upgraded = storage.upgradeVehicleToMonthly("", regularTicketIDToUpgrade, t.registrationDate, t.ticketID);
  } else {
    // Với ô tô, xe máy, tìm bằng biển số và đổi mã vé thành mã vé tháng mới
    upgraded = storage.upgradeVehicleToMonthly(t.plate, "", t.registrationDate, t.ticketID);
  }

  if (upgraded) {
    cout << "\n  [*] Xe dang trong bai da duoc chuyen sang VE THANG.\n";
    cout << "      Chi tinh phi gui xe luot tu luc vao bai den thoi diem dang ky nay.\n";
    cout << "      Thoi gian tu bay gio den luc ra bai se duoc MIEN PHI.\n";
  }


  cout << "\n========================================";
  cout << "\n       DANG KY VE THANG THANH CONG      ";
  cout << "\n========================================\n";

  // Nhóm 1: Thông tin xe & Chủ xe
  string typeStr =
      (t.vehicleType == 1 ? "Xe dap"
                          : (t.vehicleType == 2 ? "Xe may" : "O to"));
  cout << "  Loai xe    : " << typeStr << endl;
  if (t.vehicleType != 1) {
    cout << "  Bien so    : " << t.plate << endl;
  }
  cout << "  Ma ve (ID) : " << t.ticketID << endl;
  cout << "  Chu xe     : " << t.ownerName << endl;
  cout << "  SDT        : " << t.phoneNumber << endl;

  cout << "  ------------------------------------" << endl;

  // Nhóm 2: Thời hạn
  cout << "  Ngay BD    : " << formatTime(t.registrationDate) << endl;
  cout << "  Ngay HH    : " << formatTime(t.expirationDate) << endl;

  cout << "  ------------------------------------" << endl;

  // Nhóm 3: Thanh toán
  cout << "  Gia ve     : " << t.price << " VND" << endl;
  cout << "  Nhan vien  : " << empID << endl;
  cout << "========================================\n";
}

void MonthlyTicketManager::renewTicket(string empID, ParkingStorage &storage) {
  manageTicket(empID, storage);
}

void MonthlyTicketManager::showAllTickets() {
  if (tickets.empty()) {
    cout << "  [!] Danh sach ve thang trong." << endl;
    return;
  }

  cout << "\n" << string(110, '=') << endl;
  cout << "                           DANH SACH VE THANG" << endl;
  cout << string(110, '=') << endl;

  auto printHeader = []() {
    cout << left << setw(10) << "ID Ve" << setw(12) << "Bien so" << setw(18)
         << "Chu xe" << setw(12) << "SDT" << setw(10) << "Loai" << setw(12)
         << "Ngay BD" << setw(12) << "Ngay HH" << setw(12) << "Gia (VND)"
         << setw(12) << "Trang thai" << endl;
    cout << string(110, '-') << endl;
  };

  vector<string> typeNames = {"", "XE DAP", "XE MAY", "O TO"};

  for (int type = 1; type <= 3; ++type) {
    bool hasTickets = false;
    for (const auto &t : tickets) {
      if (t.vehicleType == type) {
        if (!hasTickets) {
          cout << "\n--- " << typeNames[type] << " ---\n";
          printHeader();
          hasTickets = true;
        }
        string typeStr =
            (type == 1 ? "Xe dap" : (type == 2 ? "Xe may" : "O to"));
        string statusStr = (t.isLocked ? "DA KHOA" : "CON HAN");
        string displayPlate = (t.plate == "NONE" ? "(khong co)" : t.plate);
        cout << left << setw(10) << t.ticketID << setw(12) << displayPlate
             << setw(18)
             << (t.ownerName.length() > 17 ? t.ownerName.substr(0, 14) + "..."
                                           : t.ownerName)
             << setw(12) << t.phoneNumber << setw(10) << typeStr << setw(12)
             << formatTime(t.registrationDate) << setw(12)
             << formatTime(t.expirationDate) << setw(12) << t.price << setw(12)
             << statusStr << endl;
      }
    }
  }
  cout << "\n" << string(110, '=') << endl;
}

void MonthlyTicketManager::showExpiringSoon() {
  time_t now = time(0);
  time_t threshold = now + (3 * 24 * 60 * 60);
  bool found = false;

  cout << "\n--- VE THANG SAP HET HAN (TRONG 3 NGAY) ---" << endl;
  cout << left << setw(12) << "Bien so" << setw(18) << "Chu xe" << setw(12)
       << "Ngay HH" << endl;
  cout << string(50, '-') << endl;

  for (const auto &t : tickets) {
    if (!t.isLocked && t.expirationDate > now &&
        t.expirationDate <= threshold) {
      cout << left << setw(12) << t.plate << setw(18) << t.ownerName << setw(12)
           << formatTime(t.expirationDate) << endl;
      found = true;
    }
  }
  if (!found)
    cout << "  [+] Khong co ve nao sap het han." << endl;
}

void MonthlyTicketManager::showStatistics() {
  int total = tickets.size();
  int active = 0, soon = 0, locked = 0;
  long long totalRevenue = 0;
  time_t now = time(0);
  time_t threshold = now + (3 * 24 * 60 * 60);

  for (const auto &t : tickets) {
    if (t.isLocked) {
      locked++;
    }
    if (!t.isLocked && t.expirationDate > now) {
      active++;
      if (t.expirationDate <= threshold)
        soon++;
    }
    totalRevenue += t.price;
  }

  cout << "\n  +-------------------------------------------+\n";
  cout << "  |           THONG KE VE THANG               |\n";
  cout << "  +-------------------------------------------+\n";
  cout << "  | Tong ve thang        | " << left << setw(19) << total << "|\n";
  cout << "  | Ve con han           | " << left << setw(19) << active << "|\n";
  cout << "  | Ve da khoa           | " << left << setw(19) << locked << "|\n";
  cout << "  | Ve sap het han       | " << left << setw(19) << soon << "|\n";
  cout << "  | Doanh thu ve thang   | " << left << setw(19)
       << (to_string(totalRevenue) + " VND") << "|\n";
  cout << "  +----------------------+--------------------+\n";
}

int MonthlyTicketManager::countTicketsByType(int type) {
  int count = 0;
  time_t now = time(0);
  for (const auto &t : tickets) {
    if (t.vehicleType == type && !t.isLocked && t.expirationDate > now) {
      count++;
    }
  }
  return count;
}

bool MonthlyTicketManager::isValid(const string &plate) {
  int status = checkTicket(plate);
  return status == 1 || status == 3;
}

int MonthlyTicketManager::checkTicket(const string &plate) {
  time_t now = time(0);
  time_t threshold = now + (3 * 24 * 60 * 60);
  string p = Utils::normalizeString(plate);
  for (const auto &t : tickets) {
    if (t.plate == p) {
      if (t.isLocked)
        return 4;
      if (t.expirationDate <= now)
        return 2;
      if (t.expirationDate <= threshold)
        return 3;
      return 1;
    }
  }
  return 0;
}

time_t MonthlyTicketManager::getExpirationDate(const string &plate) {
  string p = Utils::normalizeString(plate);
  for (const auto &t : tickets) {
    if (t.plate == p)
      return t.expirationDate;
  }
  return 0;
}

string MonthlyTicketManager::getTicketID(const string &plate) {
  string p = Utils::normalizeString(plate);
  for (const auto &t : tickets) {
    if (t.plate == p && t.plate != "NONE")
      return t.ticketID;
  }
  return "";
}

string MonthlyTicketManager::formatTime(time_t t) {
  tm *info = localtime(&t);
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y", info);
  return string(buffer);
}

int MonthlyTicketManager::checkTicketByID(const string &id) {
  time_t now = time(0);
  time_t threshold = now + (3 * 24 * 60 * 60);
  string upperID = Utils::normalizeString(id);
  for (const auto &t : tickets) {
    if (Utils::normalizeString(t.ticketID) == upperID) {
      if (t.isLocked)
        return 4;
      if (t.expirationDate <= now)
        return 2;
      if (t.expirationDate <= threshold)
        return 3;
      return 1;
    }
  }
  return 0;
}

time_t MonthlyTicketManager::getExpirationDateByID(const string &id) {
  string upperID = Utils::normalizeString(id);
  for (const auto &t : tickets) {
    if (Utils::normalizeString(t.ticketID) == upperID)
      return t.expirationDate;
  }
  return 0;
}

string MonthlyTicketManager::getPlateByID(const string &id) {
  string upperID = Utils::normalizeString(id);
  for (const auto &t : tickets) {
    if (Utils::normalizeString(t.ticketID) == upperID)
      return t.plate;
  }
  return "";
}

void MonthlyTicketManager::manageTicket(string empID, ParkingStorage &storage) {
  cout << "\n--- TRA CUU & QUAN LY VE THANG ---" << endl;
  cout << "1. Tim theo Bien so" << endl;
  cout << "2. Tim theo Ma ve" << endl;
  cout << "0. Quay lai" << endl;
  cout << "Chon: ";
  int choice = Utils::readMenuChoice(0, 2);
  if (choice == 0)
    return;

  string input;
  MonthlyTicket *foundTicket = nullptr;
  if (choice == 1) {
    while (true) {
      cout << "Nhap bien so: ";
      cin >> input;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      input = Utils::normalizeString(input);
      if (!Utils::isValidMotorbikePlate(input) &&
          !Utils::isValidCarPlate(input)) {
        cout << "  [!] Bien so khong dung dinh dang! (VD: 29B12345 hoac "
                "29A12345)\n";
        continue;
      }
      break;
    }
    for (auto &t : tickets) {
      if (t.plate == input && t.plate != "NONE") {
        foundTicket = &t;
        break;
      }
    }
  } else {
    while (true) {
      cout << "Nhap ma ve: ";
      cin >> input;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      input = Utils::normalizeString(input);
      if (Utils::hasInvalidChar(input)) {
        cout << "  [!] Ma ve khong duoc chua ky tu dac biet!\n";
        continue;
      }
      break;
    }
    for (auto &t : tickets) {
      if (t.ticketID == input) {
        foundTicket = &t;
        break;
      }
    }
  }

  if (!foundTicket) {
    cout << "  [!] Khong tim thay ve thang!" << endl;
    cout << "  An Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
  }

  int subChoice;
  do {
    cout << "\n========================================" << endl;
    cout << "        CHI TIET VE THANG" << endl;
    cout << "========================================" << endl;

    // Nhóm 1: Thông tin xe & Chủ xe
    string typeStr =
        (foundTicket->vehicleType == 1
             ? "Xe dap"
             : (foundTicket->vehicleType == 2 ? "Xe may" : "O to"));
    cout << "  Loai xe    : " << typeStr << endl;
    cout << "  Bien so    : "
         << (foundTicket->plate == "NONE" ? "(Khong co)" : foundTicket->plate)
         << endl;
    cout << "  Ma ve (ID) : " << foundTicket->ticketID << endl;
    cout << "  Chu xe     : " << foundTicket->ownerName << endl;
    cout << "  SDT        : " << foundTicket->phoneNumber << endl;

    cout << "  ------------------------------------" << endl;

    // Nhóm 2: Thời hạn & Trạng thái
    cout << "  Ngay DK    : " << formatTime(foundTicket->registrationDate)
         << endl;
    cout << "  Han dung   : " << formatTime(foundTicket->expirationDate)
         << endl;
    cout << "  Trang thai : "
         << (foundTicket->isLocked ? "DANG KHOA" : "DANG HOAT DONG") << endl;

    time_t now = time(0);
    if (foundTicket->expirationDate < now) {
      cout << "  [!] CANH BAO: VE DA HET HAN!" << endl;
    } else if (foundTicket->expirationDate < now + 3 * 24 * 3600) {
      cout << "  [!] CANH BAO: VE SAP HET HAN!" << endl;
    }
    cout << "========================================" << endl;
    cout << "1. Gia han ve" << endl;
    cout << "2. " << (foundTicket->isLocked ? "Mo khoa ve" : "Khoa ve") << endl;
    cout << "3. Xoa ve" << endl;
    cout << "0. Quay lai" << endl;
    cout << "Chon: ";
    subChoice = Utils::readMenuChoice(0, 3);

    if (subChoice == 1) {
      // Gia han
      time_t now = time(0);

      // Nếu vé đã hết hạn, cần kiểm tra sức chứa trước khi cho phép gia hạn lại
      if (foundTicket->expirationDate < now) {
        VehicleConfig cfg = storage.getVehicleConfig(foundTicket->vehicleType);
        int registeredTickets = countTicketsByType(foundTicket->vehicleType);
        int countB, countM, countC;
        storage.countVehicles(countB, countM, countC);
        int totalInLot =
            (foundTicket->vehicleType == 1
                 ? countB
                 : (foundTicket->vehicleType == 2 ? countM : countC));
        int monthlyInLot = storage.countMonthlyInLot(foundTicket->vehicleType);
        int regularInLot = totalInLot - monthlyInLot;

        if (registeredTickets + regularInLot >= cfg.maxCapacity) {
          cout << "\n  [!] LOI: Khong the gia han ve do bai da het cho trong "
                  "an toan!"
               << endl;
          cout << "      Vui long cho xe luot ra bot truoc khi gia han lai."
               << endl;
          continue;
        }
      }

      time_t baseTime = foundTicket->expirationDate;
      int monthsToAdd = 2;

      if (baseTime < now) {
        baseTime = now;
        monthsToAdd = 1;
      }

      VehicleConfig cfg = storage.getVehicleConfig(foundTicket->vehicleType);
      tm *ltm = localtime(&baseTime);
      ltm->tm_mon += monthsToAdd;
      ltm->tm_mday = 1;
      ltm->tm_hour = 0;
      ltm->tm_min = 0;
      ltm->tm_sec = 0;
      foundTicket->expirationDate = mktime(ltm) - 1;
      foundTicket->price = cfg.monthlyPrice;
      logRevenue(*foundTicket, empID);
      saveToFile();

      // Nâng cấp xe trong bãi nếu xe đang gửi vé lượt (do trước đó hết hạn)
      storage.upgradeVehicleToMonthly(foundTicket->plate, foundTicket->ticketID, now, foundTicket->ticketID);

      cout << "\n========================================";
      cout << "\n       GIA HAN VE THANG THANH CONG      ";
      cout << "\n========================================\n";

      // Nhóm 1: Thông tin xe & Chủ xe
      string typeStr =
          (foundTicket->vehicleType == 1
               ? "Xe dap"
               : (foundTicket->vehicleType == 2 ? "Xe may" : "O to"));
      cout << "  Loai xe    : " << typeStr << endl;
      if (foundTicket->vehicleType != 1) {
        cout << "  Bien so    : " << foundTicket->plate << endl;
      }
      cout << "  Ma ve (ID) : " << foundTicket->ticketID << endl;
      cout << "  Chu xe     : " << foundTicket->ownerName << endl;

      cout << "  ------------------------------------" << endl;

      // Nhóm 2: Thời hạn mới
      cout << "  Han dung moi: " << formatTime(foundTicket->expirationDate)
           << endl;

      cout << "  ------------------------------------" << endl;

      // Nhóm 3: Thanh toán
      cout << "  THANH TOAN : " << foundTicket->price << " VND" << endl;
      cout << "  Nhan vien  : " << empID << endl;
      cout << "========================================\n";
      cout << "  An Enter de quay lai...";
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return;
    } else if (subChoice == 2) {
      // Khoa/Mo khoa
      foundTicket->isLocked = !foundTicket->isLocked;
      saveToFile();

      // Nếu vừa mở khóa, kiểm tra nâng cấp xe trong bãi (nếu xe đó đang gửi vé lượt)
      if (!foundTicket->isLocked) {
        time_t now = time(0);
        storage.upgradeVehicleToMonthly(foundTicket->plate, foundTicket->ticketID, now, foundTicket->ticketID);
      }

      cout << "\n========================================";
      cout << "\n    CAP NHAT TRANG THAI THANH CONG      ";
      cout << "\n========================================\n";

      // Nhóm 1: Thông tin xe & Chủ xe
      string typeStr = (foundTicket->vehicleType == 1
                            ? "Xe dap"
                            : (foundTicket->vehicleType == 2 ? "Xe may" : "O to"));
      cout << "  Loai xe    : " << typeStr << endl;
      cout << "  Bien so    : "
           << (foundTicket->plate == "NONE" ? "(Khong co)" : foundTicket->plate)
           << endl;
      cout << "  Ma ve (ID) : " << foundTicket->ticketID << endl;
      cout << "  Chu xe     : " << foundTicket->ownerName << endl;
      cout << "  SDT        : " << foundTicket->phoneNumber << endl;

      cout << "  ------------------------------------" << endl;

      // Nhóm 2: Trạng thái mới
      cout << "  Trang thai : "
           << (foundTicket->isLocked ? "DA KHOA" : "DANG HOAT DONG") << endl;
      cout << "========================================\n";
      cout << "  An Enter de quay lai...";
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return;
    } else if (subChoice == 3) {
      // Xoa
      if (storage.isVehicleInLot(foundTicket->plate) ||
          storage.isVehicleInLot(foundTicket->ticketID)) {
        cout << "  [!] KHONG THE XOA: Xe dang trong bai!" << endl;
        cout << "  Vui long cho xe ra khoi bai truoc khi xoa ve thang." << endl;
        continue;
      }

      cout << "  [?] Ban co chac muon xoa ve nay? (1: Co, 0: Khong): ";
      if (Utils::readMenuChoice(0, 1) == 1) {
        for (auto it = tickets.begin(); it != tickets.end(); ++it) {
          if (it->ticketID == foundTicket->ticketID) {
            string deletedID = it->ticketID;
            string deletedPlate = it->plate;
            tickets.erase(it);
            saveToFile();

            cout << "\n========================================";
            cout << "\n        XOA VE THANG THANH CONG         ";
            cout << "\n========================================\n";
            cout << "  Ma ve (ID) : " << deletedID << endl;
            cout << "  Bien so    : " << deletedPlate << endl;
            cout << "========================================\n";
            return;
          }
        }
      }
    }
  } while (subChoice != 0);
}
