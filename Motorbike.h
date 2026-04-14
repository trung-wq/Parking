#pragma once
#include "Vehicle.h"

class Motorbike : public Vehicle {

public:

    Motorbike(string p, string tID)
        : Vehicle(p, tID) {}

    int calculateFee() {
        int fee = 0;
        time_t temp = ticket.getTimeIn();
        time_t out = ticket.getTimeOut();

        while (temp < out) {
            tm* info = localtime(&temp);
            int h = info->tm_hour;

            if (h >= 6 && h < 18) {
                fee += 3000;
                info->tm_hour = 18;
                info->tm_min = 0;
                info->tm_sec = 0;
                temp = mktime(info);
            } else {
                fee += 5000;
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
        cout << "[Xe may]\n";
        Vehicle::display();
    }
    int getType()
    {
        return 2;
    }
};