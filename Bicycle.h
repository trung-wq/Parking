#pragma once
#include "Vehicle.h"

class Bicycle : public Vehicle {

public:

    Bicycle(string p, string tID)
        : Vehicle(p, tID) {}

    int calculateFee() {

        int fee = 0;

        time_t temp = ticket.getTimeIn();
        time_t out = ticket.getTimeOut();

        while (temp < out) {

            int h = getHour(temp);

            if (h >= 6 && h < 18)
                fee += 1000;
            else
                fee += 2000;

            temp += 3600;
        }

        return fee;
    }

    void display() {
        cout << "[Xe dap]\n";
        Vehicle::display();
    }
    int getType()
    {
        return 1;
    }
};