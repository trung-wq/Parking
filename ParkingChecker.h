#pragma once
#include "ParkingStorage.h"
#include <iomanip>
#include <vector>

// Module chịu trách nhiệm: hiển thị danh sách xe, tìm kiếm, kiểm tra slot
class ParkingChecker {
private:
  ParkingStorage &storage;

public:
  explicit ParkingChecker(ParkingStorage &s) : storage(s) {}

  void showAvailableSlots(); // Tình trạng chỗ trống
  void showListParking();    // Danh sách xe đơn giản
  void display();            // Bảng đầy đủ phân theo loại xe
  bool search();             // Tìm kiếm xe theo mã vé / biển số
};
