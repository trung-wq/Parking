#pragma once
#include <deque>
#include <stack>
#include "Bicycle.h"
#include "Motorbike.h"
#include "Car.h"
#include "Employee.h"
#include <string>
#include "Vehicle.h"
#include<fstream>
class ParkingLot {

private:

    std::deque<Vehicle*> parkingQueue;
    stack<Vehicle*> history;

    int revenue = 0;

    Employee emp;

    int bicycleTicketCounter = 0;

public:
    void saveHistoryToFile();
    void saveToFile();
    void loadFromFile();
    void addVehicle();
    void removeVehicle();
    void display();
    void search();
    void showRevenue();
};