#pragma once
#include "ParkingStorage.h"
#include <iomanip>
#include <vector>

class ParkingChecker {
private:
  ParkingStorage &storage;
  MonthlyTicketManager &monthlyManager;

public:
  explicit ParkingChecker(ParkingStorage &s, MonthlyTicketManager &m) : storage(s), monthlyManager(m) {}

  void showAvailableSlots(); // Tình trạng chỗ trống
  void showListParking();    // Danh sách xe đang trong bãi
  void display();            // Bảng đầy đủ phân theo loại xe
  bool search();             // Tìm kiếm xe theo mã vé / biển số
};
