#pragma once
#include <string>
#include <ctime>

using namespace std;

struct MonthlyTicket {
    string ticketID; // XD001, XM001, OT001
    string plate;
    string ownerName;
    string phoneNumber;
    time_t registrationDate;
    time_t expirationDate;
    int vehicleType; // 1=Dap, 2=May, 3=Oto
    int price;
    bool isLocked;
};
