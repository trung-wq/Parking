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
    string dateIn;
    string dateOut;

public:
    // Constructor với biển số
    Ticket(string i) {
        id = i;
        time(&timeIn);
        dateIn = setDate(timeIn);
    }

    Ticket() {
        id = "BICYCLE"; // hoặc sinh ID tự động
        time(&timeIn);
    }
    string setDate(time_t d)
    {
        tm info;
        localtime_s(&info, &d);
        char buffer[50];
        strftime(buffer,sizeof(buffer), "%d/%m/%Y", &info);
        return string(buffer);
        //date = string(buffer);
    }
    void _setDateIn(string _dateIn)
    {
        dateIn = _dateIn;
    }
    void _setDateOut(string _dateOut)
    {
        dateOut = _dateOut;
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
        dateOut = setDate(timeOut);
    }

    time_t getTimeIn() {
        return timeIn;
    }

    time_t getTimeOut() {
        return timeOut;
    }
    string formatTime(time_t t)
    {
        tm info;
        localtime_s(&info, &t);

        char buffer[50];

        //strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &info);
        strftime(buffer, sizeof(buffer), "%H:%M", &info);

        return string(buffer);
    }
    
    string getDateIn() { return dateIn; }
    string getDateOut() { return dateOut; }
    void display() {
        tm t;
        localtime_s(&t, &timeIn);


        cout << "Ve: " << id << endl;
        cout << "Ngay vao: " << dateIn << endl;
        cout << "Gio vao: " << formatTime(timeIn) << endl;
            /*<< t.tm_hour << ":"
            << t.tm_min << endl;*/
    }
};