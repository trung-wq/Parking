#include <iostream>
#include <cstdlib>
#include "ParkingLot.h"

using namespace std;
void menu()
{
    cout << "\n===== QUAN LY BAI GIU XE =====\n";
    cout << "1 Them xe\n";
    cout << "2 Xe roi bai\n";
    cout << "3 Danh sach xe\n";
    cout << "4 Tim xe\n";
    cout << "5 Doanh thu\n";
    cout << "6 Luu file\n";
    cout << "7 Doc file\n";
    cout << "8 Thoat\n";

    cout << "Chon: ";
}
int main() {

    ParkingLot lot;
    int choice;

    do {
        system("cls");
        menu();
        //cin.ignore();
        cin >> choice;

        switch (choice) 
        {

        case 1: lot.addVehicle();system("pause"); break;
        case 2: lot.removeVehicle();system("pause"); break;
        case 3: lot.display();system("pause"); break;
        case 4: lot.search();system("pause"); break;
        case 5: lot.showRevenue();system("pause"); break;
        case 6: lot.saveToFile();system("pause");break;
        case 7: lot.loadFromFile();system("pause");break;
        }

    } while (choice != 8);

}