#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

enum class SlotStatus {
    Available,
    Occupied
};

class ParkingSlot {
public:
    string code;        // e.g., A01, B01, C01
    SlotStatus status;
    string plate;       // Biển số xe đang đỗ (nếu có)
    int vehicleType;    // 1: Bicycle, 2: Motorbike, 3: Car
    int distance;       // Để tìm vị trí gần nhất

    ParkingSlot(string c, int type, int dist) 
        : code(c), status(SlotStatus::Available), plate(""), vehicleType(type), distance(dist) {}

    string getStatusString() const {
        return (status == SlotStatus::Available) ? "Trong" : "Dang dung";
    }
};

class ParkingZone {
public:
    string name;        // Khu A, Khu B...
    int vehicleType;    // Loại xe (1: Xe dap, 2: Xe may, 3: O to)
    vector<ParkingSlot> slots;

    ParkingZone(string n, int type) : name(n), vehicleType(type) {}

    void addSlot(const string& code, int dist) {
        slots.emplace_back(code, vehicleType, dist);
    }
};
