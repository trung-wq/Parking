#include "ParkingLot.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void waitEnter() { cin.get(); }

void menuEmployee() {
  cout << "\n=== QUAN LY NHAN VIEN ===\n";
  cout << "1 Danh sach nhan vien\n";
  cout << "2 Them nhan vien\n";
  cout << "3 Xoa nhan vien\n";
  cout << "4 Cap nhat nhan vien\n";
  cout << "0 Quay lai menu chinh\n";
  cout << "Chon: ";
}

void menuRevenue() {
  cout << "\n=== QUAN LY DOANH THU ===\n";
  cout << "1 Tong doanh thu\n";
  cout << "2 Doanh thu theo ngay\n";
  cout << "3 Doanh thu theo thang\n";
  cout << "4 Doanh thu theo nam\n";
  cout << "0 Quay lai menu chinh\n";
  cout << "Chon: ";
}

void menuParking() {
  cout << "\n=== QUAN LY BAI XE ===\n";
  cout << "1 Them xe moi\n";
  cout << "2 Xe ra khoi bai\n";
  cout << "3 Danh sach xe hien tai\n";
  cout << "4 Tim kiem thong tin xe trong bai\n";
  cout << "5 Danh sach cac xe da ra khoi bai\n";
  cout << "6 Thong ke vi tri trong\n";
  cout << "0 Quay lai menu chinh\n";
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
  cout << "1 Them xe moi\n";
  cout << "2 Xe ra khoi bai\n";
  cout << "3 Danh sach xe hien tai\n";
  cout << "4 Tim kiem thong tin xe trong bai\n";
  cout << "5 Thong ke vi tri trong\n";
  cout << "0 Dang xuat\n";
  cout << "Chon: ";
}

// ============================================================
//  Đăng nhập
// ============================================================
Employee *login(vector<Employee> &users) {
  bool firstTime = true;
  while (true) {
    system("clear");
    if (!firstTime) {
      cout << "  [!] Username hoac Password khong dung. Vui long thu lai.\n";
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

int main() {
  ParkingLot lot;
  lot.loadEmployees();
  lot.loadFromFile();
  lot.loadHistoryFromFile();

  while (true) {
    Employee *currentUser = login(lot.getEmployeeList());

    if (currentUser->getRole() == 2) {
      // -------- NHÂN VIÊN --------
      int choice;
      do {
        system("clear");
        // lot.showListParking();
        menuNV();
        choice = Utils::readMenuChoice(0, 5);

        switch (choice) {
        case 1:
          if (lot.addVehicle(currentUser->getID())) {
            cout << "\nNhan Enter de tiep tuc...";
            waitEnter();
          }
          break;
        case 2:
          if (lot.removeVehicle(currentUser->getID())) {
            cout << "\nNhan Enter de tiep tuc...";
            waitEnter();
          }
          break;
        case 3:
          lot.display();
          cout << "\nNhan Enter de tiep tuc...";
          waitEnter();
          break;
        case 4:
          if (lot.search()) {
            cout << "\nNhan Enter de tiep tuc...";
            waitEnter();
          }
          break;
        case 5:
          lot.showAvailableSlots();
          cout << "\nNhan Enter de tiep tuc...";
          waitEnter();
          break;
        case 0:
          cout << "Dang xuat thanh cong!\n";
          cout << "Nhan Enter de tiep tuc...";
          waitEnter();
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
        choice = Utils::readMenuChoice(1, 4);

        switch (choice) {
        case 4:
          cout << "Dang xuat thanh cong!\n";
          cout << "Nhan Enter de tiep tuc...";
          waitEnter();
          break;

        case 1: {
          int c;
          do {
            system("clear");
            menuParking();
            c = Utils::readMenuChoice(0, 6);
            switch (c) {
            case 1:
              if (lot.addVehicle(currentUser->getID())) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 2:
              if (lot.removeVehicle(currentUser->getID())) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 3:
              lot.display();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 4:
              if (lot.search()) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 5:
              lot.ShowHistory();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 6:
              lot.showAvailableSlots();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 0:
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
            c = Utils::readMenuChoice(0, 4);
            switch (c) {
            case 1:
              lot.showRevenue();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 2:
              if (lot.revenueByDay()) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 3:
              if (lot.revenueByMonth()) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 4:
              if (lot.revenueByYear()) {
                cout << "\nNhan Enter de tiep tuc...";
                waitEnter();
              }
              break;
            case 0:
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
            c = Utils::readMenuChoice(0, 4);
            switch (c) {
            case 1:
              lot.showEmployees();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 2:
              lot.addEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 3:
              lot.deleteEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 4:
              lot.updateEmployee();
              cout << "\nNhan Enter de tiep tuc...";
              waitEnter();
              break;
            case 0:
              break;
            }
          } while (c != 0);
          break;
        }
        }
      } while (choice != 4);
    }
  }
}