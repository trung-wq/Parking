#pragma once
#include "Employee.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Module chịu trách nhiệm: quản lý danh sách nhân viên, file I/O nhân viên
class EmployeeManager {
private:
  vector<Employee> emlist;

public:
  // Truy cập danh sách nhân viên (dùng cho login ở main)
  vector<Employee> &getEmployeeList() { return emlist; }

  // --- File I/O ---
  void saveEmployees();
  void loadEmployees();

  // --- CRUD nhân viên ---
  void addEmployee();
  void showEmployees();
  void deleteEmployee();
  void updateEmployee();
};
