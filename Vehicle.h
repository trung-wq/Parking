#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Ticket.h"
#include <ctime>
#include <iostream>
using namespace std;

class Vehicle {

protected:
  string plate;
  Ticket ticket;

public:
  Vehicle(string p, string ticketID) : plate(p), ticket(ticketID) {}

  virtual ~Vehicle() {}

  string getPlate() { return plate; }

  Ticket &getTicket() { return ticket; }

  string formatTime(time_t t) {
    tm info;
    localtime_r(&t, &info);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%H:%M", &info);
    return string(buffer);
  }

  string formatDateTime(time_t t) {
    tm info;
    localtime_r(&t, &info);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &info);
    return string(buffer);
  }

  int getHour(time_t t) {
    time_t now;
    time(&now);
    tm *info_t = localtime(&t);
    return info_t->tm_hour;
  }

  virtual int calculateFee(int dayPrice, int nightPrice) = 0;

  static int calculateShiftFee(time_t start, time_t end, int dayPrice,
                               int nightPrice) {
    int fee = 0;
    time_t temp = start;
    while (temp < end) {
      tm info;
      localtime_r(&temp, &info);
      int h = info.tm_hour;
      if (h >= 6 && h < 18) {
        fee += dayPrice;
        info.tm_hour = 18;
        info.tm_min = 0;
        info.tm_sec = 0;
        temp = mktime(&info);
      } else {
        fee += nightPrice;
        if (h >= 18)
          info.tm_mday += 1;
        info.tm_hour = 6;
        info.tm_min = 0;
        info.tm_sec = 0;
        temp = mktime(&info);
      }
    }
    return fee;
  }

  virtual void display(int dayPrice = 0, int nightPrice = 0) {
    string typeStr =
        (getType() == 1 ? "Xe dap" : (getType() == 2 ? "Xe may" : "O to"));
    string priceStr = to_string(dayPrice) + " / " + to_string(nightPrice);
    if (dayPrice == 0 && nightPrice == 0) {
        priceStr = (getType() == 1 ? "2,000 / 3,000"
                                    : (getType() == 2 ? "5,000 / 10,000"
                                                      : "20,000 / 30,000"));
    }
    cout << "Loai xe  : " << typeStr << endl;
    cout << "Don gia  : " << priceStr << " (Ngay/Dem)" << endl;
    ticket.display();
    if (!plate.empty()) {
      cout << "Bien so  : " << plate << endl;
    }
  }
  virtual int getType() = 0;
};