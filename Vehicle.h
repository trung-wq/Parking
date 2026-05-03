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

  int getHour(time_t t) {
    time_t now;
    time(&now);
    tm *info_t = localtime(&t);
    return info_t->tm_hour;
  }

  virtual int calculateFee() = 0;

  virtual void display() {
    ticket.display();
    if (!plate.empty()) {
      cout << "Bien so  : " << plate << endl;
    }
  }
  virtual int getType() = 0;
};