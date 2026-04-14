#pragma once
#include <queue>
#include <stack>
#include "Bicycle.h"
#include "Motorbike.h"
#include "Car.h"
#include "Employee.h"
#include <string>
#include "Vehicle.h"
#include<fstream>
#include <limits>
class ParkingLot {

private:

    queue<Vehicle*> parkingQueue;
    stack<Vehicle*> history;

    int revenue = 0;

    Employee emp;

public:
    void saveHistoryToFile();
    void saveToFile();
    void loadFromFile();
    void addVehicle();
    void removeVehicle();
    void display();
    void search();
    void showRevenue();
    void showListParking();
    void ShowHistory();
    void loadHistoryFromFile();
    void revenueByDate();
    //void getDateParts(time_t t, int& d, int& m, int& y);
    void revenueByDay();
    void revenueByMonth();
    void revenueByYear();
};