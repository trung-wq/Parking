#pragma once
#include "Bicycle.h"
#include "Car.h"
#include "Employee.h"
#include "MonthlyTicketManager.h"
#include "Motorbike.h"
#include "Vehicle.h"
#include "ParkingSpace.h"
#include <vector>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <map>

struct VehicleConfig {
    int dayPrice;
    int nightPrice;
    int monthlyPrice;
    int maxCapacity;
};

class ParkingStorage {
private:
  queue<Vehicle *> parkingQueue;
  stack<Vehicle *> history;
  int revenue = 0;
  int counterVXD = 1; // Vé lượt xe đạp
  int counterVXM = 1; // Vé lượt xe máy
  int counterVOT = 1; // Vé lượt ô tô
  vector<ParkingZone> zones;
  map<int, VehicleConfig> configs;

public:
  queue<Vehicle *> &getQueue() { return parkingQueue; }
  stack<Vehicle *> &getHistory() { return history; }
  vector<ParkingZone> &getZones() { return zones; }
  int getRevenue() const { return revenue; }
  void addRevenue(int amount) { revenue += amount; }

  // Đếm xe theo từng loại
  void countVehicles(int &countBicycle, int &countMotorbike, int &countCar);
  int countMonthlyInLot(int type);

  // --- Kiểm tra biển số trùng ---
  bool isDuplicatePlate(const string &plate);
  // --- Nhập biển số hợp lệ --- (vehicleType: 2=xe máy, 3=ôtô)
  string readPlate(bool checkDuplicate, int vehicleType = 0);

  // --- File I/O ---
  void saveToFile();
  void loadFromFile(MonthlyTicketManager &monthlyManager);
  bool isVehicleInLot(const string &identifier);
  void saveHistoryToFile();
  void loadHistoryFromFile(MonthlyTicketManager &monthlyManager);
  void saveConfig();
  void loadConfig();

  // --- Quản lý xe ---
  bool addVehicle(string empID, MonthlyTicketManager &monthlyManager);
  bool removeVehicle(string empID);
  // Nâng cấp xe vé lượt đang trong bãi → vé tháng (đặt expirationDate = now)
  bool upgradeVehicleToMonthly(const string &plate, const string &ticketID, time_t registrationTime, const string &newMonthlyTicketID);

  // --- Quản lý vị trí ---
  void initSlots();
  string allocateSlot(int vehicleType, const string &plate, bool isMonthly);
  void releaseSlot(const string &slotCode);
  void updateSlotStatus(const string &slotCode, SlotStatus status);
  // 6. Tính và định dạng thời gian gửi xe (Trả về chuỗi X ngày Y giờ Z phút)
  static string getDurationString(time_t start, time_t end);

  // --- Quản lý cấu hình ---
  void setVehicleConfig(int type, int day, int night, int monthly, int cap);
  VehicleConfig getVehicleConfig(int type);
  void showConfigs();
};
