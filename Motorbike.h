#pragma once
#include "Vehicle.h"

class Motorbike : public Vehicle {

public:
  Motorbike(string p, string tID) : Vehicle(p, tID) {}

  int calculateFee(int dayPrice, int nightPrice) {
    time_t start = ticket.getTimeIn();
    if (ticket.getIsMonthly()) {
      if (ticket.getUpgradedMidStay()) {
        return Vehicle::calculateShiftFee(start, ticket.getExpirationDate(),
                                          dayPrice, nightPrice);
      }
      if (ticket.getTimeOut() <= ticket.getExpirationDate())
        return 0;
      start = ticket.getExpirationDate();
    }
    return Vehicle::calculateShiftFee(start, ticket.getTimeOut(), dayPrice,
                                      nightPrice);
  }

  void display() { Vehicle::display(); }
  int getType() { return 2; }
};