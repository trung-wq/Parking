#pragma once
#include "EmployeeManager.h"
#include "MonthlyTicketManager.h"
#include "ParkingChecker.h"
#include "ParkingStorage.h"
#include "RevenueManager.h"
#include "Utils.h"

class ParkingLot {
private:
  ParkingStorage storage;
  ParkingChecker checker;
  RevenueManager revenueManager;
  EmployeeManager employeeManager;
  MonthlyTicketManager monthlyManager;

public:
  ParkingLot() : checker(storage, monthlyManager), revenueManager(storage) {
    storage.initSlots();
    storage.loadConfig();
  }

  // --- Quản lý vé tháng ---
  void addMonthlyTicket(string empID) { monthlyManager.addTicket(empID, storage); }
  void renewMonthlyTicket(string empID) { monthlyManager.renewTicket(empID, storage); }
  void manageMonthlyTicket(string empID) { monthlyManager.manageTicket(empID, storage); }
  void showMonthlyTickets() { monthlyManager.showAllTickets(); }

  void showMonthlyStatistics() { monthlyManager.showStatistics(); }
  MonthlyTicketManager &getMonthlyManager() { return monthlyManager; }

  // --- Quản lý xe ---
  bool addVehicle(string empID) {
    return storage.addVehicle(empID, monthlyManager);
  }
  bool removeVehicle(string empID) { return storage.removeVehicle(empID); }

  // --- Hiển thị & tìm kiếm ---
  void display() { checker.display(); }
  void showListParking() { checker.showListParking(); }
  bool search() { return checker.search(); }
  void showAvailableSlots() { checker.showAvailableSlots(); }

  // --- Doanh thu & lịch sử ---
  void showRevenue() { revenueManager.showRevenue(); }
  void ShowHistory() { revenueManager.ShowHistory(); }
  bool revenueByDay() { return revenueManager.revenueByDay(); }
  bool revenueByMonth() { return revenueManager.revenueByMonth(); }
  bool revenueByYear() { return revenueManager.revenueByYear(); }

  // --- Quản lý nhân viên ---
  vector<Employee> &getEmployeeList() {
    return employeeManager.getEmployeeList();
  }
  void loadEmployees() { employeeManager.loadEmployees(); }
  void addEmployee() { employeeManager.addEmployee(); }
  void showEmployees() { employeeManager.showEmployees(); }
  void deleteEmployee() { employeeManager.deleteEmployee(); }
  void updateEmployee() { employeeManager.updateEmployee(); }

  // --- Quản lý cấu hình ---
  void showConfigs() { storage.showConfigs(); }
  void updateConfig() {
    int type;
    cout << "\n--- CAP NHAT CAU HINH ---\n";
    cout << "1. Xe dap\n2. Xe may\n3. O to\n0. Quay lai\nChon loai xe: ";
    type = Utils::readMenuChoice(0, 3);
    if (type == 0) return;
    
    VehicleConfig current = storage.getVehicleConfig(type);
    int subChoice;
    do {
        cout << "\n--- BAN MUON CAP NHAT GI? ---\n";
        cout << "1. Cap nhat don gia (Ngay, Dem, Thang)\n";
        cout << "2. Cap nhat suc chua (So cho trong)\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        subChoice = Utils::readMenuChoice(0, 2);

        if (subChoice == 1) {
            int day = Utils::readInteger("Gia ngay hien tai: " + to_string(current.dayPrice) + " -> Nhap moi: ");
            int night = Utils::readInteger("Gia dem hien tai : " + to_string(current.nightPrice) + " -> Nhap moi: ");
            int monthly = Utils::readInteger("Gia ve thang hien tai: " + to_string(current.monthlyPrice) + " -> Nhap moi: ");
            storage.setVehicleConfig(type, day, night, monthly, current.maxCapacity);
            current = storage.getVehicleConfig(type); // Update local copy
            cout << "Cap nhat gia thanh cong!\n";
        } else if (subChoice == 2) {
            int countBicycle, countMotorbike, countCar;
            storage.countVehicles(countBicycle, countMotorbike, countCar);
            int currentVehicles = (type == 1 ? countBicycle : (type == 2 ? countMotorbike : countCar));
            int monthlyTickets = monthlyManager.countTicketsByType(type);

            int cap = Utils::readInteger("Suc chua hien tai: " + to_string(current.maxCapacity) + " -> Nhap moi: ");
            
            int totalRequired = currentVehicles + monthlyTickets;
            if (cap < totalRequired) {
                cout << "  [!] LOI: Suc chua moi (" << cap << ") khong the it hon tong so xe trong bai va ve thang (" << totalRequired << ")!\n";
            } else {
                storage.setVehicleConfig(type, current.dayPrice, current.nightPrice, current.monthlyPrice, cap);
                current = storage.getVehicleConfig(type); // Update local copy
                cout << "Cap nhat suc chua thanh cong!\n";
            }
        }
    } while (subChoice != 0);
  }

  // --- File I/O (gọi từ main) ---
  void loadHistoryFromFile() { storage.loadHistoryFromFile(monthlyManager); }
  void loadFromFile() { storage.loadFromFile(monthlyManager); }
  void saveToFile() { storage.saveToFile(); }
};