#pragma once
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Namespace chứa các hàm tiện ích dùng chung
namespace Utils {

// 1. Chuẩn hóa chuỗi (Xóa khoảng trắng thừa & In hoa toàn bộ)
string normalizeString(const string &str);

// 2. Kiểm tra chuỗi có chứa khoảng trắng hay ký tự đặc biệt không (chỉ cho phép
// chữ và số)
bool hasInvalidChar(const string &str);

// 3. Kiểm tra tính hợp lệ của biển số chung (6-10 ký tự, không ký tự đặc biệt)
bool isValidPlate(const string &plate);

// 3a. Kiểm tra biển số xe máy Việt Nam: 2 số + 1-2 chữ cái + 4-5 số (VD:
// 29B12345)
bool isValidMotorbikePlate(const string &plate);

// 3b. Kiểm tra biển số ôtô Việt Nam: 2 số + 1-2 chữ cái + 4-5 số (VD: 51F1234)
bool isValidCarPlate(const string &plate);

// 5. Nhập và kiểm tra lựa chọn menu
int readMenuChoice(int lo, int hi);

// 6. Nhập số nguyên an toàn
int readInteger(const string &prompt, int minVal = 1);

// 7. Kiểm tra số điện thoại (10 số)
bool isValidPhone(const string &phone);

// 8. Kiểm tra tên (không rỗng, tối đa 50 ký tự)
bool isValidName(const string &name);

// 9. Định dạng thời gian trôi qua (giây -> X giờ Y phút)
string formatDuration(time_t seconds);

// 10. Kiểm tra định dạng ngày (DD/MM/YYYY)
bool isValidDate(const string &date);

// 11. Các hàm xử lý và kiểm tra ngày tháng năm
void getDateParts(time_t t, int &d, int &m, int &y);
bool isValidDate(int d, int m, int y);
bool isValidMonth(int m, int y);
bool isValidYear(int y);

} // namespace Utils
