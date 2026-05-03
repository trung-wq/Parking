#include "RevenueManager.h"
#include <iomanip>
using namespace std;

static void getDateParts(time_t t, int &d, int &m, int &y) {
  tm info;
  localtime_r(&t, &info);
  d = info.tm_mday;
  m = info.tm_mon + 1;
  y = info.tm_year + 1900;
}

static bool isValidDate(int d, int m, int y) {
  if (y < 1900 || y > 2100)
    return false;
  if (m < 1 || m > 12)
    return false;
  if (d < 1)
    return false;
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
    daysInMonth[1] = 29;
  if (d > daysInMonth[m - 1])
    return false;

  int cur_d, cur_m, cur_y;
  getDateParts(time(nullptr), cur_d, cur_m, cur_y);
  if (y > cur_y)
    return false;
  if (y == cur_y && m > cur_m)
    return false;
  if (y == cur_y && m == cur_m && d > cur_d)
    return false;

  return true;
}

static bool isValidMonth(int m, int y) {
  if (y < 1900 || y > 2100 || m < 1 || m > 12)
    return false;
  int cur_d, cur_m, cur_y;
  getDateParts(time(nullptr), cur_d, cur_m, cur_y);
  if (y > cur_y)
    return false;
  if (y == cur_y && m > cur_m)
    return false;
  return true;
}

static bool isValidYear(int y) {
  if (y < 1900 || y > 2100)
    return false;
  int cur_d, cur_m, cur_y;
  getDateParts(time(nullptr), cur_d, cur_m, cur_y);
  return y <= cur_y;
}

static void printSummary(const vector<Record> &list) {
  int dCount = 0, mCount = 0, cCount = 0;
  int dFee = 0, mFee = 0, cFee = 0;
  for (const auto &r : list) {
    if (r.type == 1) {
      dCount++;
      dFee += r.fee;
    } else if (r.type == 2) {
      mCount++;
      mFee += r.fee;
    } else if (r.type == 3) {
      cCount++;
      cFee += r.fee;
    }
  }
  cout << "\n  --- TOM TAT THEO LOAI XE ---\n";
  cout << "  Xe dap : " << left << setw(3) << dCount << " luot | " << right
       << setw(10) << dFee << " VND\n";
  cout << "  Xe may : " << left << setw(3) << mCount << " luot | " << right
       << setw(10) << mFee << " VND\n";
  cout << "  O to   : " << left << setw(3) << cCount << " luot | " << right
       << setw(10) << cFee << " VND\n";
  cout << "  ----------------------------\n";
  cout << "  TONG   : " << left << setw(3) << (dCount + mCount + cCount)
       << " luot | " << right << setw(10) << (dFee + mFee + cFee) << " VND\n";
}

static void printRecords(const vector<Record> &list) {
  if (list.empty()) {
    cout << "\n  [!] Khong co du lieu trong thoi gian nay.\n";
    return;
  }
  cout << "\n  +----+------------+----------+------------+------------+\n";
  cout << "  | STT| Bien so    | Loai xe  | Ngay ra    | Tien (VND) |\n";
  cout << "  +----+------------+----------+------------+------------+\n";
  int idx = 1;
  for (auto &r : list) {
    string plate = r.plate.empty() ? "NONE" : r.plate;
    string typeStr =
        (r.type == 1) ? "Xe dap" : (r.type == 2 ? "Xe may" : "O to");
    cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
         << plate << "| " << left << setw(9) << typeStr << "| " << left
         << setw(11) << r.date << "| " << left << setw(11) << r.fee << "|\n";
  }
  cout << "  +----+------------+----------+------------+------------+\n";
}

// Menu sắp xếp chung
static void sortMenu(vector<Record> &list) {
  int opt;
  do {
    cout << "\nSap xep:\n";
    cout << "1 Tang dan theo tien\n";
    cout << "2 Giam dan theo tien\n";
    cout << "3 Quay lai menu thong ke\n";
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
  cout << "\n====== KET QUA ======\n";
  cout << "Tong doanh thu: " << storage.getRevenue() << " VND\n";
  cout << "=====================\n";
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

  auto printGroup = [](const string &title, const string &icon,
                       vector<Vehicle *> &list) {
    if (list.empty())
      return;
    cout << "\n  " << icon << " " << title << " (" << list.size() << " luot)\n";
    cout << "  "
            "+----+------------+------------+------------+-------+------------+"
            "-------+-----------+\n";
    cout << "  | STT| Bien so    | Ma ve      | Ngay vao   | Gio   | Ngay ra   "
            " | Gio   | Phi (VND) |\n";
    cout << "  "
            "+----+------------+------------+------------+-------+------------+"
            "-------+-----------+\n";
    int idx = 1;
    for (Vehicle *v : list) {
      string plate = v->getPlate().empty() ? "NONE" : v->getPlate();
      string id = v->getTicket().getId();
      string dateIn = v->getTicket().getDateIn();
      string timeIn = v->formatTime(v->getTicket().getTimeIn());
      string dateOut = v->getTicket().getDateOut();
      string timeOut = v->formatTime(v->getTicket().getTimeOut());
      int fee = v->calculateFee();
      cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
           << plate << "| " << left << setw(11) << id << "| " << left
           << setw(11) << dateIn << "| " << left << setw(6) << timeIn << "| "
           << left << setw(11) << dateOut << "| " << left << setw(6) << timeOut
           << "| " << left << setw(10) << fee << "|\n";
    }
    cout << "  "
            "+----+------------+------------+------------+-------+------------+"
            "-------+-----------+\n";
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
//  Doanh thu theo chuỗi ngày nhập tay (dd/mm/yyyy)
// ============================================================
bool RevenueManager::revenueByDate() {
  stack<Vehicle *> &hist = storage.getHistory();
  if (hist.empty()) {
    return false;
  }
  string inputDate;
  cout << "Nhap ngay (dd/mm/yyyy): ";
  cin >> inputDate;

  vector<Record> list;
  stack<Vehicle *> temp = hist;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    if (v->getTicket().getDateOut() == inputDate) {
      int fee = v->calculateFee();
      list.push_back(
          {v->getPlate(), v->getTicket().getDateOut(), fee, v->getType()});
      total += fee;
    }
    temp.pop();
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU NGAY: " << inputDate << endl;
    cout << "========================================\n";
    printSummary(list);
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
//  Doanh thu theo ngày (nhập d m y) + sắp xếp
// ============================================================
bool RevenueManager::revenueByDay() {
  int d, m, y;
  while (true) {
    cout << "Nhap ngay thang nam (dd mm yyyy): ";
    cin >> d >> m >> y;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Nhap sai! Vui long nhap so.\n";
      continue;
    }
    if (!isValidDate(d, m, y)) {
      cout << "Ngay thang khong hop le hoac lon hon hien tai!\n";
      continue;
    }
    break;
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (dd == d && mm == m && yy == y) {
      int fee = v->calculateFee();
      list.push_back(
          {v->getPlate(), v->getTicket().getDateOut(), fee, v->getType()});
      total += fee;
    }
    temp.pop();
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU NGAY: " << d << "/" << m << "/" << y << endl;
    cout << "========================================\n";
    printSummary(list);
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
  int m, y;
  while (true) {
    cout << "Nhap thang nam (mm yyyy): ";
    cin >> m >> y;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Nhap sai! Vui long nhap so.\n";
      continue;
    }
    if (!isValidMonth(m, y)) {
      cout << "Thang nam khong hop le hoac lon hon hien tai!\n";
      continue;
    }
    break;
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (mm == m && yy == y) {
      int fee = v->calculateFee();
      list.push_back(
          {v->getPlate(), v->getTicket().getDateOut(), fee, v->getType()});
      total += fee;
    }
    temp.pop();
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU THANG: " << m << "/" << y << endl;
    cout << "========================================\n";
    printSummary(list);
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
  int y;
  while (true) {
    cout << "Nhap nam: ";
    cin >> y;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Nhap sai! Vui long nhap so.\n";
      continue;
    }
    if (!isValidYear(y)) {
      cout << "Nam khong hop le hoac lon hon hien tai!\n";
      continue;
    }
    break;
  }

  stack<Vehicle *> temp = storage.getHistory();
  vector<Record> list;
  int total = 0;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    int dd, mm, yy;
    getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);
    if (yy == y) {
      int fee = v->calculateFee();
      list.push_back(
          {v->getPlate(), v->getTicket().getDateOut(), fee, v->getType()});
      total += fee;
    }
    temp.pop();
  }

  if (!list.empty()) {
    cout << "\n========================================\n";
    cout << "   DOANH THU NAM: " << y << endl;
    cout << "========================================\n";
    printSummary(list);
    printRecords(list);
    sortMenu(list);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
  }
  cout << "\n  [!] Khong tim thay du lieu doanh thu trong thoi gian nay!\n";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return true;
}
