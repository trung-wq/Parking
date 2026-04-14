#pragma once
#include "Vehicle.h"

class Car : public Vehicle {

public:
  Car(string p, string tID) : Vehicle(p, tID) {}

  int calculateFee() {
    int fee = 0;
    time_t temp = ticket.getTimeIn();
    time_t out = ticket.getTimeOut();

    while (temp < out) {
      tm *info = localtime(&temp);
      int h = info->tm_hour;

      if (h >= 6 && h < 18) {
        fee += 10000;
        info->tm_hour = 18;
        info->tm_min = 0;
        info->tm_sec = 0;
        temp = mktime(info);
      } else {
        fee += 15000;
        if (h >= 18) {
          info->tm_mday += 1;
        }
        info->tm_hour = 6;
        info->tm_min = 0;
        info->tm_sec = 0;
        temp = mktime(info);
      }
    }

    return fee;
  }

  void display() {
    cout << "[O to]\n";
    Vehicle::display();
  }
  int getType() { return 3; }
};