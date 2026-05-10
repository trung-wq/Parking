#pragma once
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

class Ticket {

private:
  string id;
  time_t timeIn;
  time_t timeOut;
  string dateIn;
  string dateOut;
  string employeeID;
  bool isMonthly;
  time_t expirationDate;
  string slotCode;
  bool upgradedMidStay; // true = vao bang ve luot, dang ky ve thang trong khi dang gui

public:
  Ticket(string i) {
    id = i;
    time(&timeIn);
    dateIn = setDate(timeIn);
    employeeID = "NONE";
    isMonthly = false;
    expirationDate = 0;
    slotCode = "N/A";
    upgradedMidStay = false;
  }

  Ticket() {
    id = "BICYCLE";
    time(&timeIn);
    employeeID = "NONE";
    isMonthly = false;
    expirationDate = 0;
    slotCode = "N/A";
    upgradedMidStay = false;
  }
  void setId(string i) { id = i; }
  void setExpirationDate(time_t e) { expirationDate = e; }
  time_t getExpirationDate() { return expirationDate; }
  void setIsMonthly(bool m) { isMonthly = m; }
  bool getIsMonthly() { return isMonthly; }
  void setUpgradedMidStay(bool u) { upgradedMidStay = u; }
  bool getUpgradedMidStay() { return upgradedMidStay; }
  string setDate(time_t d) {
    tm info;
    localtime_r(&d, &info);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &info);
    return string(buffer);
  }
  void _setDateIn(string _dateIn) { dateIn = _dateIn; }
  void _setDateOut(string _dateOut) { dateOut = _dateOut; }
  void setTimeIn(time_t t) { timeIn = t; }
  void _setTimeOut(time_t t) { timeOut = t; }
  void setEmployeeID(string eid) { employeeID = eid; }
  void setTimeOut() {
    time(&timeOut);
    dateOut = setDate(timeOut);
  }

  time_t getTimeIn() { return timeIn; }

  time_t getTimeOut() { return timeOut; }
  string formatTime(time_t t) {
    tm info;
    localtime_r(&t, &info);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%H:%M", &info);
    return string(buffer);
  }

  string getDateIn() { return dateIn; }
  string getDateOut() { return dateOut; }
  string getId() { return id; }
  string getEmployeeID() { return employeeID; }
  void setSlotCode(string sc) { slotCode = sc; }
  string getSlotCode() { return slotCode; }
  void display() {
    cout << "Ma ve    : " << id << endl;
    cout << "Loai ve  : " << (isMonthly ? "VE THANG" : "VE LUOT") << endl;
    cout << "Ngay vao : " << dateIn << endl;
    cout << "Gio vao  : " << formatTime(timeIn) << endl;
    cout << "Vi tri   : " << slotCode << endl;
    cout << "Nhan vien: " << employeeID << endl;
  }
};