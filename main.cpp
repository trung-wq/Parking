#include "ParkingLot.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>

using namespace std;

// ============================================================
//  Menus
// ============================================================
void menuEmployee() {
  cout << "\n=== QUAN LY NHAN VIEN ===\n";
  cout << "1 Danh sach\n";
  cout << "2 Them nhan vien\n";
  cout << "3 Xoa nhan vien\n";
  cout << "4 Cap nhat nhan vien\n";
  cout << "0 Quay lai\n";
  cout << "Chon: ";
}

void menuRevenue() {
  cout << "\n=== QUAN LY DOANH THU ===\n";
  cout << "1 Tong doanh thu\n";
  cout << "2 Theo ngay\n";
  cout << "3 Theo thang\n";
  cout << "4 Theo nam\n";
  cout << "0 Quay lai\n";
  cout << "Chon: ";
}

void menuParking() {
  cout << "\n=== QUAN LY BAI XE ===\n";
  cout << "1 Them xe\n";
  cout << "2 Xe roi bai\n";
  cout << "3 Danh sach xe\n";
  cout << "4 Tim xe\n";
  cout << "5 Lich su xe ra vao\n";
  cout << "6 Vi tri trong con lai\n";
  cout << "0 Quay lai\n";
  cout << "Chon: ";
}

void menuAdmin() {
  cout << "\n===== ADMIN =====\n";
  cout << "1 Quan ly bai xe\n";
  cout << "2 Quan ly doanh thu\n";
  cout << "3 Quan ly nhan vien\n";
  cout << "4 Dang xuat\n";
  cout << "Chon: ";
}

void menuNV() {
  cout << "\n===== QUAN LY BAI GIU XE =====\n";
  cout << "1 Them xe\n";
  cout << "2 Xe roi bai\n";
  cout << "3 Danh sach xe\n";
  cout << "4 Tim xe\n";
  cout << "5 Vi tri trong con lai\n";
  cout << "0 Dang xuat\n";
  cout << "Chon: ";
}

// ============================================================
//  Đăng nhập — tự vòng lặp đến khi đúng
// ============================================================
Employee *login(vector<Employee> &users) {
  bool firstTime = true;
  while (true) {
    system("clear");
    if (!firstTime) {
      cout << "  [!] Sai tai khoan hoac mat khau! Vui long thu lai.\n";
    }
    firstTime = false;

    string u, p;
    cout << "\n===== DANG NHAP =====\n";
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    for (auto &emp : users) {
      if (emp.login(u, p)) {
        cout << "Dang nhap thanh cong!\n";
        return &emp;
      }
    }
  }
}

// ============================================================
//  main
// ============================================================
int main() {
  ParkingLot lot;
  lot.loadEmployees();

  while (true) {
    Employee *currentUser = login(lot.getEmployeeList());
    lot.loadHistoryFromFile();

    if (currentUser->getRole() == 2) {
      // -------- NHÂN VIÊN --------
      int choice;
      do {
        system("clear");
        // lot.showListParking();
        menuNV();
        choice = Utils::readInt();

        switch (choice) {
        case 1:
          if (lot.addVehicle()) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.ignore();
            cin.get();
          }
          break;
        case 2:
          if (lot.removeVehicle()) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.ignore();
            cin.get();
          }
          break;
        case 3:
          lot.display();
          cout << "\nNhan Enter de tiep tuc...";
          cin.ignore();
          cin.get();
          break;
        case 4:
          if (lot.search()) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.ignore();
            cin.get();
          }
          break;
        case 5:
          lot.showAvailableSlots();
          cout << "\nNhan Enter de tiep tuc...";
          cin.ignore();
          cin.get();
          break;
        case 0:
          cout << "Dang xuat...\n";
          break;
        default:
          cout << "  [!] Lua chon khong hop le!\n";
          cin.ignore();
          cin.get();
          break;
        }
      } while (choice != 0);

    } else {
      // -------- ADMIN --------
      int choice;
      do {
        system("clear");
        // lot.showListParking();
        menuAdmin();
        choice = Utils::readInt();

        switch (choice) {
        case 4:
          cout << "Dang xuat...\n";
          break;

        case 1: {
          int c;
          do {
            system("clear");
            menuParking();
            c = Utils::readInt();
            switch (c) {
            case 1:
              if (lot.addVehicle()) {
                cout << "\nNhan Enter de tiep tuc...";
                cin.ignore();
                cin.get();
              }
              break;
            case 2:
              if (lot.removeVehicle()) {
                cout << "\nNhan Enter de tiep tuc...";
                cin.ignore();
                cin.get();
              }
              break;
            case 3:
              lot.display();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 4:
              if (lot.search()) {
                cout << "\nNhan Enter de tiep tuc...";
                cin.ignore();
                cin.get();
              }
              break;
            case 5:
              lot.ShowHistory();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 6:
              lot.showAvailableSlots();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 0:
              break;
            default:
              cout << "  [!] Lua chon khong hop le!\n";
              cin.ignore();
              cin.get();
              break;
            }
          } while (c != 0);
          break;
        }

        case 2: {
          int c;
          do {
            system("clear");
            menuRevenue();
            c = Utils::readInt();
            switch (c) {
            case 1:
              lot.showRevenue();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 2:
              lot.revenueByDay();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 3:
              lot.revenueByMonth();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 4:
              lot.revenueByYear();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 0:
              break;
            default:
              cout << "  [!] Lua chon khong hop le!\n";
              cin.ignore();
              cin.get();
              break;
            }
          } while (c != 0);
          break;
        }

        case 3: {
          int c;
          do {
            system("clear");
            menuEmployee();
            c = Utils::readInt();
            switch (c) {
            case 1:
              lot.showEmployees();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 2:
              lot.addEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 3:
              lot.deleteEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 4:
              lot.updateEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              cin.ignore();
              cin.get();
              break;
            case 0:
              break;
            default:
              cout << "  [!] Lua chon khong hop le!\n";
              cin.ignore();
              cin.get();
              break;
            }
          } while (c != 0);
          break;
        }

        default:
          cout << "  [!] Lua chon khong hop le!\n";
          cin.ignore();
          cin.get();
          break;
        }

      } while (choice != 4);
    }
  }
}