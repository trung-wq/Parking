#include "ParkingLot.h"
#include <algorithm>
#include <cctype>

void ParkingLot::saveToFile()
{
    ofstream out("data.txt");
    //out << revenue << endl;

    std::deque<Vehicle*> temp = parkingQueue;
    while (!temp.empty())
    {
        Vehicle* v = temp.front();

        //out << v->getPlate() << endl;
        out << v->getType() << " "
            << v->getPlate() << " "
            << v->getTicket().getID() << " "
            << v->getTicket().getTimeIn() << " ";
            //<< v->getTicket().getTimeOut() << endl;

        temp.pop_front();
    }
    out.close();
    cout << "Da luu file!\n";
}
void ParkingLot::saveHistoryToFile()
{
    ofstream out("history.txt");

    if (!out) {
        cout << "Khong mo duoc file!\n";
        return;
    }

    stack<Vehicle*> temp = history;
    while (!temp.empty()) {
        Vehicle* v = temp.top();

        out << v->getType() << " "
            << v->getPlate() << " "
            << v->getTicket().getTimeIn() << " "
            << v->getTicket().getTimeOut() << " "
            << v->calculateFee() << endl;

        temp.pop();
    }

    out.close();
    cout << "Da luu lich su xe roi bai!\n";
}
void ParkingLot::loadFromFile() {
    ifstream in("data.txt");

    if (!in) {
        cout << "Khong co file!\n";
        return;
    }

    // Đọc doanh thu
    in >> revenue;

    int type;
    string plate;
    time_t inTime, outTime;

    string ticketID;

    while (in >> type >> plate >> ticketID >> inTime) {

        Vehicle* v;

        if (type == 1) v = new Bicycle(plate, ticketID);
        else if (type == 2) v = new Motorbike(plate, ticketID);
        else v = new Car(plate, ticketID);

        // ❗ GÁN LẠI THỜI GIAN
        v->getTicket().setTimeIn(inTime);
        //v->getTicket()._setTimeOut(outTime);

        parkingQueue.push_back(v);
    }


    in.close();
    cout << "Da tai du lieu!\n";
}
void ParkingLot::addVehicle() {

    int type;
    string plate;

    cout << "1 Xe dap\n2 Xe may\n3 O to\nChon: ";
    cin >> type;

    string ticketID;

    if (type != 1) {
        while (true) {
            cin.ignore();
            cout << "Nhap bien so: ";
            getline(cin, plate);
            if (plate.length() < 7 || plate.length() > 9) {
                cout << "Bien so phai tu 7 den 9 ki tu. Nhap lai.\n";
                continue;
            }
            bool valid = true;
            for (char c : plate) {
                if (!isalnum(c)) {
                    valid = false;
                    break;
                }
            }
            if (!valid) {
                cout << "Bien so chi duoc chua chu va so. Nhap lai.\n";
                continue;
            }
            bool duplicate = false;
            for (Vehicle* veh : parkingQueue) {
                if (veh->getPlate() == plate) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                cout << "Bien so da ton tai trong bai xe. Nhap lai.\n";
                continue;
            }
            break;
        }
        ticketID = "T" + plate;
    } else {
        plate = "";
        ticketID = "T" + std::to_string(bicycleTicketCounter++);
    }

    Vehicle* v;

    if (type == 1)
        v = new Bicycle(plate, ticketID);
    else if (type == 2)
        v = new Motorbike(plate, ticketID);
    else
        v = new Car(plate, ticketID);

    parkingQueue.push_back(v);

    emp.display();
    v->getTicket().display();

    cout << "Them xe thanh cong!\n";
}

void ParkingLot::removeVehicle() {

    if (parkingQueue.empty()) {
        cout << "Bai xe rong\n";
        return;
    }

    string ticketID;
    cin.ignore(); // to handle newline from previous input
    while (true) {
        cout << "Nhap ve: ";
        getline(cin, ticketID);

        // Validate: only alphanumeric
        bool valid = true;
        for (char c : ticketID) {
            if (!isalnum(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Ve chi duoc chua chu va so. Nhap lai.\n";
            continue;
        }

        // Find the vehicle
        auto it = std::find_if(parkingQueue.begin(), parkingQueue.end(), [&](Vehicle* v) {
            return v->getTicket().getID() == ticketID;
        });
        if (it != parkingQueue.end()) {
            Vehicle* v = *it;
            parkingQueue.erase(it);
            v->getTicket().setTimeOut();
            int fee = v->calculateFee();
            cout << "Tien gui: " << fee << " VND\n";
            revenue += fee;
            history.push(v);
            break;
        } else {
            cout << "Khong tim thay ve. Nhap lai.\n";
        }
    }
}

void ParkingLot::display() {

    if (parkingQueue.empty()) {
        cout << "Danh sach xe rong\n";
        return;
    }

    std::deque<Vehicle*> temp = parkingQueue;

    while (!temp.empty()) {

        temp.front()->display();
        cout << "----------------\n";
        temp.pop_front();
    }
}

void ParkingLot::search() {

    if (parkingQueue.empty()) {
        cout << "Danh sach xe rong, khong co xe de tim.\n";
        return;
    }

    string plate;
    cout << "Nhap bien so: ";
    cin >> plate;

    std::deque<Vehicle*> temp = parkingQueue;

    while (!temp.empty()) {

        if (temp.front()->getPlate() == plate) {

            cout << "Tim thay:\n";
            temp.front()->display();
            return;
        }

        temp.pop_front();
    }

    cout << "Khong tim thay\n";
}

void ParkingLot::showRevenue() {

    cout << "Tong doanh thu: " << revenue << " VND\n";
}