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

int readInt() {
  int value;
  while (true) {
    if (cin >> value) {
      char next = cin.peek();
      if (next == '\n' || next == EOF) {
        cin.ignore(); // tiêu thụ '\n', buffer sạch khi return
        if (value >= 0)
          return value;
        cout << "  [!] Vui long nhap so duong!\nChon: ";
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

int readMenuChoice(int lo, int hi) {
  int val;
  while (true) {
    if (cin >> val) {
      char next = cin.peek();
      if (next == '\n' || next == EOF) {
        cin.ignore();
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

} // namespace Utils
