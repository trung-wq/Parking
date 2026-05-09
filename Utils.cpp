#include "Utils.h"
#include <regex>

namespace Utils {

string normalizeString(const string &str) {
  auto start = find_if(str.begin(), str.end(),
                       [](unsigned char c) { return !isspace(c); });
  auto end = find_if(str.rbegin(), str.rend(), [](unsigned char c) {
               return !isspace(c);
             }).base();

  if (start >= end)
    return "";
  string s(start, end);
  transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return toupper(c); });
  return s;
}

bool hasInvalidChar(const string &str) {
  for (char c : str) {
    if (!isalnum((unsigned char)c)) {
      return true;
    }
  }
  return false;
}

bool isValidPlate(const string &plate) {
  if (plate.length() < 6 || plate.length() > 10)
    return false;
  return !hasInvalidChar(plate);
}

// Biển số xe máy Việt Nam: 2 số + 1-2 chữ cái + 4-5 số
// Ví dụ hợp lệ: 29B12345, 51G112345, 30AB1234
bool isValidMotorbikePlate(const string &plate) {
  static const regex pattern(R"(^\d{2}[A-Z]{1,2}\d{4,5}$)");
  return regex_match(plate, pattern);
}

// Biển số ôtô Việt Nam: 2 số + đúng 1 chữ cái + 4-5 số
// Ví dụ hợp lệ: 29A12345, 51F1234, 43K56789
bool isValidCarPlate(const string &plate) {
  static const regex pattern(R"(^\d{2}[A-Z]\d{4,5}$)");
  return regex_match(plate, pattern);
}

int readMenuChoice(int lo, int hi) {
  int val;
  while (true) {
    if (cin >> val) {
      char next = cin.peek();
      if (next == '\n' || next == EOF) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (val >= lo && val <= hi)
          return val;
        cout << "  [!] Vui long chon tu " << lo << " den " << hi << "!\nChon: ";
        continue;
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "  [!] Lua chon khong hop le! Vui long chi nhap so nguyen.\nChon: ";
  }
}

int readInteger(const string &prompt, int minVal) {
  int val;
  while (true) {
    cout << prompt;
    if (cin >> val) {
      char next = cin.peek();
      if (next == '\n' || next == EOF) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (val >= minVal)
          return val;
        cout << "  [!] Gia tri phai lon hon hoac bang " << minVal << "!\n";
        continue;
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "  [!] Gia tri khong hop le! Vui long chi nhap so nguyen.\n";
  }
}

bool isValidPhone(const string &phone) {
  if (phone.length() < 10 || phone.length() > 11)
    return false;
  for (char c : phone) {
    if (!isdigit(c))
      return false;
  }
  return true;
}

bool isValidName(const string &name) {
  if (name.empty() || name.length() > 50)
    return false;
  return true;
}

string formatDuration(time_t seconds) {
  if (seconds < 0)
    return "0p";
  int h = seconds / 3600;
  int m = (seconds % 3600) / 60;
  if (h > 0) {
    return to_string(h) + "h " + to_string(m) + "p";
  }
  return to_string(m) + "p";
}

void getDateParts(time_t t, int &d, int &m, int &y) {
  tm info;
  localtime_r(&t, &info);
  d = info.tm_mday;
  m = info.tm_mon + 1;
  y = info.tm_year + 1900;
}

bool isValidDate(int d, int m, int y) {
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

bool isValidDate(const string &date) {
  // Regex kiểm tra định dạng DD/MM/YYYY
  static const regex pattern(
      R"(^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[012])/(19|20)\d\d$)");
  if (!regex_match(date, pattern))
    return false;

  int d, m, y;
  if (sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y) == 3) {
    return isValidDate(d, m, y);
  }
  return false;
}

bool isValidMonth(int m, int y) {
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

bool isValidYear(int y) {
  if (y < 1900 || y > 2100)
    return false;
  int cur_d, cur_m, cur_y;
  getDateParts(time(nullptr), cur_d, cur_m, cur_y);
  return y <= cur_y;
}

} // namespace Utils
