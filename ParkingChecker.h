#pragma once
#include "ParkingStorage.h"
#include <iomanip>
#include <vector>

class ParkingChecker {
private:
  ParkingStorage &storage;

public:
  explicit ParkingChecker(ParkingStorage &s) : storage(s) {}

  void showAvailableSlots(); // Tình trạng chỗ trống
  void showListParking();    // Danh sách xe đang trong bãi
  void display();            // Bảng đầy đủ phân theo loại xe
  bool search();             // Tìm kiếm xe theo mã vé / biển số
};
