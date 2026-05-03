#include "RevenueManager.h"
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

// In danh sách kết quả thống kê
static void printRecords(const vector<Record> &list) {
  cout << "\n===== KET QUA =====\n";
  for (auto &r : list) {
    cout << r.plate << " | " << r.date << " | " << r.fee << " VND\n";
  }
}

// Menu sắp xếp chung
static void sortMenu(vector<Record> &list) {
  int opt;
  do {
    cout << "\nSap xep:\n";
    cout << "1 Tang dan theo tien\n";
    cout << "2 Giam dan theo tien\n";
    cout << "3 Thoat\n";
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
  cout << "Tong doanh thu: " << storage.getRevenue() << " VND\n";
}

// ============================================================
//  Lịch sử xe đã rời bãi
// ============================================================
void RevenueManager::ShowHistory() {
  stack<Vehicle *> &hist = storage.getHistory();
  if (hist.empty()) {
    cout << "Chua co xe nao roi bai!\n";
    return;
  }
  stack<Vehicle *> temp = hist;
  cout << "\n===== LICH SU XE ROI BAI =====\n";
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    v->display();
    cout << "Gio ra: " << v->formatTime(v->getTicket().getTimeOut()) << endl;
    cout << "Ngay ra: " << v->getTicket().getDateOut() << endl;
    cout << "Tien: " << v->calculateFee() << " VND\n";
    cout << "----------------------\n";
    temp.pop();
  }
}

// ============================================================
//  Doanh thu theo chuỗi ngày nhập tay (dd/mm/yyyy)
// ============================================================
void RevenueManager::revenueByDate() {
  stack<Vehicle *> &hist = storage.getHistory();
  if (hist.empty()) {
    cout << "Chua co du lieu!!\n";
    return;
  }
  string inputDate;
  cout << "Nhap ngay (dd/mm/yyyy): ";
  cin >> inputDate;
  stack<Vehicle *> temp = hist;
  int total = 0;
  bool found = false;
  while (!temp.empty()) {
    Vehicle *v = temp.top();
    if (v->getTicket().getDateOut() == inputDate) {
      total += v->calculateFee();
      found = true;
    }
    temp.pop();
  }
  if (found)
    cout << "Doanh thu ngay " << inputDate << " la: " << total << " VND\n";
  else
    cout << "Ngay hom do khong co doanh thu!\n";
}

// ============================================================
//  Doanh thu theo ngày (nhập d m y) + sắp xếp
// ============================================================
void RevenueManager::revenueByDay() {
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
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee});
      total += fee;
    }
    temp.pop();
  }

  printRecords(list);
  cout << "Doanh thu ngay " << d << "/" << m << "/" << y << " = " << total
       << " VND\n";
  sortMenu(list);
}

// ============================================================
//  Doanh thu theo tháng + sắp xếp
// ============================================================
void RevenueManager::revenueByMonth() {
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
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee});
      total += fee;
    }
    temp.pop();
  }

  printRecords(list);
  cout << "Doanh thu thang " << m << "/" << y << " = " << total << " VND\n";
  sortMenu(list);
}

// ============================================================
//  Doanh thu theo năm + sắp xếp
// ============================================================
void RevenueManager::revenueByYear() {
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
      list.push_back({v->getPlate(), v->getTicket().getDateOut(), fee});
      total += fee;
    }
    temp.pop();
  }

  printRecords(list);
  cout << "Doanh thu nam " << y << " = " << total << " VND\n";
  sortMenu(list);
}
