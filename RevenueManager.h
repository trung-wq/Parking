#pragma once
#include "ParkingStorage.h"
#include <algorithm>
#include <vector>

// Cấu trúc lưu kết quả thống kê doanh thu
struct Record {
  string plate;
  string date;
  int fee;
};

// Module chịu trách nhiệm: thống kê doanh thu, lịch sử xe ra
class RevenueManager {
private:
  ParkingStorage &storage;

public:
  explicit RevenueManager(ParkingStorage &s) : storage(s) {}

  void showRevenue();    // Tổng doanh thu hiện tại
  void ShowHistory();    // Lịch sử xe đã rời bãi
  void revenueByDate();  // Doanh thu theo chuỗi ngày (dd/mm/yyyy)
  void revenueByDay();   // Doanh thu theo ngày + sắp xếp
  void revenueByMonth(); // Doanh thu theo tháng + sắp xếp
  void revenueByYear();  // Doanh thu theo năm + sắp xếp
};
