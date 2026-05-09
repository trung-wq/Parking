#include "RevenueManager.h"
#include "ParkingStorage.h"
#include "Utils.h"
#include <fstream>
#include <iomanip>
using namespace std;

static void printRecords(const vector<Record> &list) {
  if (list.empty()) {
    cout << "\n  [!] Khong co du lieu trong thoi gian nay.\n";
    return;
  }
  cout << "\n  "
          "+----+------------+----------+------------+------------+------------"
          "+\n";
  cout << "  | STT| Bien so    | Loai xe  | Ngay ra    | Tien (VND) | Nhan "
          "vien  |\n";
  cout << "  "
          "+----+------------+----------+------------+------------+------------"
          "+\n";
  int idx = 1;
  for (auto &r : list) {
    string plate = r.plate.empty() ? "NONE" : r.plate;
    string typeStr =
        (r.type == 1) ? "Xe dap" : (r.type == 2 ? "Xe may" : "O to");
    cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
         << plate << "| " << left << setw(9) << typeStr << "| " << left
         << setw(11) << r.date << "| " << left << setw(11) << r.fee << "| "
         << left << setw(11) << r.empID << "|\n";
  }
  cout << "  "
          "+----+------------+----------+------------+------------+------------"
          "+\n";
}

// ============================================================
//  Đọc doanh thu vé tháng từ monthly_revenue.txt
// ============================================================
static vector<Record> readMonthlyRevenue(int filterDay = -1,
                                         int filterMonth = -1,
                                         int filterYear = -1) {
  vector<Record> result;
  ifstream in("monthly_revenue.txt");
  if (!in)
    return result;
  int type;
  string plate, date, empID;
  int price;
  while (in >> type >> plate >> date >> price >> empID) {
    // date dạng DD/MM/YYYY
    int dd = 0, mm = 0, yy = 0;
    sscanf(date.c_str(), "%d/%d/%d", &dd, &mm, &yy);
    if (filterDay != -1 && dd != filterDay)
      continue;
    if (filterMonth != -1 && mm != filterMonth)
      continue;
    if (filterYear != -1 && yy != filterYear)
      continue;
    result.push_back({plate == "NONE" ? "" : plate, date, price, type, empID});
  }
  in.close();
  return result;
}

// Menu sắp xếp chung
static void sortMenu(vector<Record> &list) {
  int opt;
  do {
    cout << "\nSap xep:\n";
    cout << "1. Tang dan theo tien\n";
    cout << "2. Giam dan theo tien\n";
    cout << "3. Quay lai menu thong ke\n";
    cout << "Chon: ";
    while (!(cin >> opt) || opt < 1 || opt > 3) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Chon lai (1-3): ";
    }
    if (opt == 1)
      sort(list.begin(), list.end(),
           [](const Record &a, const Record &b) { return a.fee < b.fee; });
    else if (opt == 2)
      sort(list.begin(), list.end(),
           [](const Record &a, const Record &b) { return a.fee > b.fee; });
    if (opt != 3)
      printRecords(list);
  } while (opt != 3);
}

// ============================================================
//  Tổng doanh thu
// ============================================================
void RevenueManager::showRevenue() {
  int dailyRevenue = storage.getRevenue();
  int monthlyTotal = 0;
  vector<Record> monthlyList = readMonthlyRevenue(-1, -1);
  for (const auto &r : monthlyList)
    monthlyTotal += r.fee;

  cout << "\n====== TONG DOANH THU HIEU TAI ======\n";
  cout << "  - Doanh thu ve luot  : " << right << setw(10) << dailyRevenue
       << " VND\n";
  cout << "  - Doanh thu ve thang : " << right << setw(10) << monthlyTotal
       << " VND\n";
  cout << "  -------------------------------------\n";
  cout << "  TONG DOANH THU       : " << right << setw(10)
       << (dailyRevenue + monthlyTotal) << " VND\n";
  cout << "=====================================\n";
}

// ============================================================
//  Lịch sử xe đã rời bãi
// ============================================================
void RevenueManager::ShowHistory() {
  stack<Vehicle *> &hist = storage.getHistory();
  if (hist.empty()) {
    cout << "\n  [!] Chua co xe nao roi bai!\n";
    return;
  }

  vector<Vehicle *> bicycles, motorbikes, cars;
  stack<Vehicle *> temp = hist;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    if (v->getType() == 1)
      bicycles.push_back(v);
    else if (v->getType() == 2)
      motorbikes.push_back(v);
    else if (v->getType() == 3)
      cars.push_back(v);
    temp.pop();
  }

  auto printGroup = [this](const string &title, const string &icon,
                           vector<Vehicle *> &list) {
    if (list.empty())
      return;
    cout << "\n  " << icon << " " << title << " (" << list.size() << " luot)\n";
    cout << "  +----+------------+------------+------------+-------+------------+-------+---------+-----------+------------+\n";
    cout << "  | STT| Bien so    | Ma ve      | Ngay vao   | Gio   | Ngay ra    | Gio   | T.Gian  | Phi (VND) | Nhan vien  |\n";
    cout << "  +----+------------+------------+------------+-------+------------+-------+---------+-----------+------------+\n";
    int idx = 1;
    for (Vehicle *v : list) {
      string plate = v->getPlate().empty() ? "(khong co)" : v->getPlate();
      string id = v->getTicket().getId();
      string dateIn = v->getTicket().getDateIn();
      string timeIn = v->formatTime(v->getTicket().getTimeIn());
      string dateOut = v->getTicket().getDateOut();
      string timeOut = v->formatTime(v->getTicket().getTimeOut());
      string duration = Utils::formatDuration(v->getTicket().getTimeOut() - v->getTicket().getTimeIn());
      VehicleConfig cfgH = storage.getVehicleConfig(v->getType());
      int fee = v->calculateFee(cfgH.dayPrice, cfgH.nightPrice);
      string eid = v->getTicket().getEmployeeID();
      cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
           << plate << "| " << left << setw(11) << id << "| " << left
           << setw(11) << dateIn << "| " << left << setw(6) << timeIn << "| "
           << left << setw(11) << dateOut << "| " << left << setw(6) << timeOut
           << "| " << left << setw(8) << duration << "| " << left << setw(10) << fee << "| " << left << setw(11) << eid
           << "|\n";
    }
    cout << "  +----+------------+------------+------------+-------+------------+-------+---------+-----------+------------+\n";
  };

  cout << "\n=================================================================="
          "======================\n";
  cout << "                           LICH SU XE DA ROI BAI\n";
  cout << "===================================================================="
          "====================\n";
  printGroup("XE DAP", "[DAP]", bicycles);
  printGroup("XE MAY", "[MAY]", motorbikes);
  printGroup("O TO", "[OTO]", cars);

  int total = bicycles.size() + motorbikes.size() + cars.size();
  cout << "\n  Tong cong: " << total << " luot xe da roi bai.\n";
  cout << "===================================================================="
          "====================\n";
}

// ============================================================
//  Doanh thu theo ngày (nhập DD/MM/YYYY) + sắp xếp
// ============================================================
bool RevenueManager::revenueByDay() {
  string inputStr;
  int d, m, y;
  while (true) {
    cout << "Nhap ngay thang nam (DD/MM/YYYY): ";
    cin >> inputStr;
    if (Utils::isValidDate(inputStr)) {
      sscanf(inputStr.c_str(), "%d/%d/%d", &d, &m, &y);
      break;
    }
    cout << "Ngay thang khong hop le (Dinh dang: DD/MM/YYYY) hoac lon hon hien "
            "tai!\n";
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    Utils::getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (dd == d && mm == m && yy == y) {
      VehicleConfig cfgH = storage.getVehicleConfig(v->getType());
      int fee = v->calculateFee(cfgH.dayPrice, cfgH.nightPrice);
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee,
                      v->getType(), v->getTicket().getEmployeeID()});
      total += fee;
    }
    temp.pop();
  }

  int dailyTotal = total;
  vector<Record> monthlyList = readMonthlyRevenue(d, m, y);
  int monthlyTotal = 0;
  for (auto &r : monthlyList) {
    monthlyTotal += r.fee;
    list.push_back(r);
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU NGAY: " << d << "/" << m << "/" << y << endl;
    cout << "========================================\n";
    cout << "  - Doanh thu ve luot  : " << right << setw(10) << dailyTotal
         << " VND\n";
    cout << "  - Doanh thu ve thang : " << right << setw(10) << monthlyTotal
         << " VND\n";
    cout << "  --------------------------------------\n";
    cout << "  TONG DOANH THU       : " << right << setw(10)
         << (dailyTotal + monthlyTotal) << " VND\n";
    printRecords(list);
    sortMenu(list);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
  }
  cout << "\n  [!] Khong tim thay du lieu doanh thu trong thoi gian nay!\n";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return true;
}

// ============================================================
//  Doanh thu theo tháng + sắp xếp
// ============================================================
bool RevenueManager::revenueByMonth() {
  string inputStr;
  int m, y;
  while (true) {
    cout << "Nhap thang nam (MM/YYYY): ";
    cin >> inputStr;
    if (sscanf(inputStr.c_str(), "%d/%d", &m, &y) == 2 &&
        Utils::isValidMonth(m, y)) {
      break;
    }
    cout << "Thang nam khong hop le (Dinh dang: MM/YYYY) hoac lon hon hien "
            "tai!\n";
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    Utils::getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (mm == m && yy == y) {
      VehicleConfig cfgH = storage.getVehicleConfig(v->getType());
      int fee = v->calculateFee(cfgH.dayPrice, cfgH.nightPrice);
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee,
                      v->getType(), v->getTicket().getEmployeeID()});
      total += fee;
    }
    temp.pop();
  }

  int dailyTotal = total;
  vector<Record> monthlyList = readMonthlyRevenue(-1, m, y);
  int monthlyTotal = 0;
  for (auto &r : monthlyList) {
    monthlyTotal += r.fee;
    list.push_back(r);
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU THANG: " << m << "/" << y << endl;
    cout << "========================================\n";
    cout << "  - Doanh thu ve luot  : " << right << setw(10) << dailyTotal
         << " VND\n";
    cout << "  - Doanh thu ve thang : " << right << setw(10) << monthlyTotal
         << " VND\n";
    cout << "  --------------------------------------\n";
    cout << "  TONG DOANH THU       : " << right << setw(10)
         << (dailyTotal + monthlyTotal) << " VND\n";
    printRecords(list);
    sortMenu(list);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
  }
  cout << "\n  [!] Khong tim thay du lieu doanh thu trong thoi gian nay!\n";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return true;
}

// ============================================================
//  Doanh thu theo năm + sắp xếp
// ============================================================
bool RevenueManager::revenueByYear() {
  string inputStr;
  int y;
  while (true) {
    cout << "Nhap nam (YYYY): ";
    cin >> inputStr;
    if (sscanf(inputStr.c_str(), "%d", &y) == 1 && Utils::isValidYear(y)) {
      break;
    }
    cout << "Nam khong hop le (Dinh dang: YYYY) hoac lon hon hien tai!\n";
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    Utils::getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (yy == y) {
      VehicleConfig cfgH = storage.getVehicleConfig(v->getType());
      int fee = v->calculateFee(cfgH.dayPrice, cfgH.nightPrice);
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee,
                      v->getType(), v->getTicket().getEmployeeID()});
      total += fee;
    }
    temp.pop();
  }

  int dailyTotal = total;
  vector<Record> monthlyList = readMonthlyRevenue(-1, -1, y);
  int monthlyTotal = 0;
  for (auto &r : monthlyList) {
    monthlyTotal += r.fee;
    list.push_back(r);
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU NAM: " << y << endl;
    cout << "========================================\n";
    cout << "  - Doanh thu ve luot  : " << right << setw(10) << dailyTotal
         << " VND\n";
    cout << "  - Doanh thu ve thang : " << right << setw(10) << monthlyTotal
         << " VND\n";
    cout << "  --------------------------------------\n";
    cout << "  TONG DOANH THU       : " << right << setw(10)
         << (dailyTotal + monthlyTotal) << " VND\n";
    printRecords(list);
    sortMenu(list);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
  }
  cout << "\n  [!] Khong tim thay du lieu doanh thu trong thoi gian nay!\n";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return true;
}