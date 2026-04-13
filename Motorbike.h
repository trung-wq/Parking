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

            int h = getHour(temp);

            if (h >= 6 && h < 18)
                fee += 3000;
            else
                fee += 5000;

            temp += 3600;
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