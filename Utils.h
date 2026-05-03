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

// 3. Kiểm tra tính hợp lệ của biển số (6-10 ký tự, không ký tự đặc biệt)
bool isValidPlate(const string &plate);

// 4. Nhập và kiểm tra một số nguyên hợp lệ (>= 0)
int readInt();

// 5. Nhập và kiểm tra lựa chọn menu
int readMenuChoice(int lo, int hi);

} // namespace Utils
