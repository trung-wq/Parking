#pragma once
#define _CRT_SECURE_NO_WARNINGS
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

public:
  Ticket(string i) {
    id = i;
    time(&timeIn);
    dateIn = setDate(timeIn);
  }

  Ticket() {
    id = "BICYCLE";
    time(&timeIn);
  }
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
  void display() {
    cout << "Ma ve    : " << id << endl;
    cout << "Ngay vao : " << dateIn << endl;
    cout << "Gio vao  : " << formatTime(timeIn) << endl;
  }
};