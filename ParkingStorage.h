#pragma once
#include "Bicycle.h"
#include "Car.h"
#include "Employee.h"
#include "Motorbike.h"
#include "Vehicle.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

class ParkingStorage {
private:
  queue<Vehicle *> parkingQueue;
  stack<Vehicle *> history;
  int revenue = 0;
  int ticketCounter = 1;

public:
  queue<Vehicle *> &getQueue() { return parkingQueue; }
  stack<Vehicle *> &getHistory() { return history; }
  int getRevenue() const { return revenue; }
  void addRevenue(int amount) { revenue += amount; }

  // Đếm xe theo từng loại
  void countVehicles(int &countBicycle, int &countMotorbike, int &countCar);

  // --- Kiểm tra biển số trùng ---
  bool isDuplicatePlate(const string &plate);
  // --- Nhập biển số hợp lệ --- (vehicleType: 2=xe máy, 3=ôtô)
  string readPlate(bool checkDuplicate, int vehicleType = 0);

  // --- File I/O ---
  void saveToFile();
  void loadFromFile();
  void saveHistoryToFile();
  void loadHistoryFromFile();

  // --- Quản lý xe ---
  bool addVehicle();
  bool removeVehicle();
};
