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
    cout << "6 Lich su xe ra\n";
    //cout << "7 Tim kiem doanh thu theo ngay\n";
    cout << "7 Doanh thu theo ngay\n";
    cout << "8 Doanh thu theo thang\n";
    cout << "9 Doanh thu theo nam\n";
    cout << "0 Thoat\n";

    cout << "Chon: ";
}
int main() {

    ParkingLot lot;
    int choice;
    lot.loadHistoryFromFile();
    do {
        system("cls");
        lot.showListParking();
        
        menu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) 
        {

        case 1: lot.addVehicle();system("pause"); break;
        case 2: lot.removeVehicle();system("pause"); break;
        case 3: lot.display();system("pause"); break;
        case 4: lot.search();system("pause"); break;
        case 5: lot.showRevenue();system("pause"); break;
        case 6: lot.ShowHistory();system("pause");break;
        //case 7: lot.revenueByDate();system("pause");break;
        case 7: lot.revenueByDay();system("pause"); break;
        case 8: lot.revenueByMonth();system("pause"); break;
        case 9: lot.revenueByYear();system("pause"); break;
        default:
            if (choice != 0) {
                cout << "Lua chon khong hop le. Vui long chon so tu 0 den 9!\n";
                system("pause");
            }
            break;
        }

    } while (choice != 0);

}