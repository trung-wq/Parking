#pragma once
#include "EmployeeManager.h"
#include "ParkingChecker.h"
#include "ParkingStorage.h"
#include "RevenueManager.h"

class ParkingLot {
private:
  ParkingStorage storage;
  ParkingChecker checker;
  RevenueManager revenueManager;
  EmployeeManager employeeManager;

public:
  ParkingLot() : checker(storage), revenueManager(storage) {}

  // --- Quản lý xe ---
  bool addVehicle() { return storage.addVehicle(); }
  bool removeVehicle() { return storage.removeVehicle(); }

  // --- Hiển thị & tìm kiếm ---
  void display() { checker.display(); }
  void showListParking() { checker.showListParking(); }
  bool search() { return checker.search(); }
  void showAvailableSlots() { checker.showAvailableSlots(); }

  // --- Doanh thu & lịch sử ---
  void showRevenue() { revenueManager.showRevenue(); }
  void ShowHistory() { revenueManager.ShowHistory(); }
  void revenueByDate() { revenueManager.revenueByDate(); }
  void revenueByDay() { revenueManager.revenueByDay(); }
  void revenueByMonth() { revenueManager.revenueByMonth(); }
  void revenueByYear() { revenueManager.revenueByYear(); }

  // --- Quản lý nhân viên ---
  vector<Employee> &getEmployeeList() {
    return employeeManager.getEmployeeList();
  }
  void loadEmployees() { employeeManager.loadEmployees(); }
  void addEmployee() { employeeManager.addEmployee(); }
  void showEmployees() { employeeManager.showEmployees(); }
  void deleteEmployee() { employeeManager.deleteEmployee(); }
  void updateEmployee() { employeeManager.updateEmployee(); }

  // --- File I/O (gọi từ main) ---
  void loadHistoryFromFile() { storage.loadHistoryFromFile(); }
  void loadFromFile() { storage.loadFromFile(); }
  void saveToFile() { storage.saveToFile(); }
};