#pragma once
#include "Vehicle.h"

class Car : public Vehicle {

public:
  Car(string p, string tID) : Vehicle(p, tID) {}
  int calculateFee(int dayPrice, int nightPrice) {
    time_t start = ticket.getTimeIn();
    if (ticket.getIsMonthly()) {
      if (ticket.getTimeOut() <= ticket.getExpirationDate())
        return 0;
      start = ticket.getExpirationDate();
    }
    return Vehicle::calculateShiftFee(start, ticket.getTimeOut(), dayPrice, nightPrice);
  }

  void display() { Vehicle::display(); }
  int getType() { return 3; }
};