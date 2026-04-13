#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

class Ticket {

private:
    string id;
    time_t timeIn;
    time_t timeOut;

public:
    // Constructor với biển số
    Ticket(string i) {
        id = i;
        time(&timeIn);
    }

    Ticket() {
        id = "BICYCLE"; // hoặc sinh ID tự động
        time(&timeIn);
    }
    void setTimeIn(time_t t)
    {
        timeIn = t;
    }
    void _setTimeOut(time_t t)
    {
        timeOut = t;
    }
    void setTimeOut() {
        time(&timeOut);
    }

    time_t getTimeIn() {
        return timeIn;
    }

    time_t getTimeOut() {
        return timeOut;
    }

    string getID() {
        return id;
    }

    void display() {
        tm* t = localtime(&timeIn);


        cout << "Ve: " << id << endl;
        cout << "Ngay va gio vao: "
            << (t->tm_year + 1900) << "/"
            << (t->tm_mon + 1) << "/"
            << t->tm_mday << " "
            << t->tm_hour << ":"
            << t->tm_min << endl;
    }
};