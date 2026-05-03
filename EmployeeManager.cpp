#include "EmployeeManager.h"
#include "Utils.h"
using namespace std;

// ============================================================
//  File I/O
// ============================================================
void EmployeeManager::saveEmployees() {
  ofstream out("employee.txt");
  for (auto &e : emlist) {
    out << e.getID() << " " << e.getName() << " " << e.getPassword() << " "
        << e.getRole() << endl;
  }
}

void EmployeeManager::loadEmployees() {
  ifstream in("employee.txt");
  if (!in)
    return;
  emlist.clear();
  string id, name, pass;
  int role;
  while (in >> id >> name >> pass >> role) {
    emlist.push_back(Employee(id, name, pass, role));
  }
}

// ============================================================
//  Hiển thị danh sách nhân viên
// ============================================================
void EmployeeManager::showEmployees() {
  if (emlist.empty()) {
    cout << "\n  [!] Chua co nhan viên nao trong danh sach.\n";
    return;
  }
  cout << "\n======================================================\n";
  cout << "               DANH SACH NHAN VIEN                    \n";
  cout << "======================================================\n";
  cout << "  +----+------------+----------------------+----------+\n";
  cout << "  | STT| ID         | Ho ten               | Chuc vu  |\n";
  cout << "  +----+------------+----------------------+----------+\n";
  int idx = 1;
  for (auto &e : emlist) {
    string roleStr = (e.getRole() == 1) ? "Admin" : "Staff";
    cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
         << e.getID() << "| " << left << setw(21) << e.getName() << "| "
         << left << setw(9) << roleStr << "|\n";
  }
  cout << "  +----+------------+----------------------+----------+\n";
}

// ============================================================
//  Thêm nhân viên mới
// ============================================================
void EmployeeManager::addEmployee() {
  string id, name, pass;

  while (true) {
    cout << "Nhap ID cua nhan vien: ";
    getline(cin, id);
    if (id.empty()) {
      cout << "  [!] Khong duoc de trong!\n";
      continue;
    }
    if (id.length() > 6) {
      cout << "  [!] ID nhan vien toi da 6 ky tu!\n";
      continue;
    }
    if (Utils::hasInvalidChar(id)) {
      cout << "  [!] ID khong duoc chua khoang trang hay ky tu dac biet!\n";
      continue;
    }
    bool dup = false;
    for (auto &e : emlist) {
      if (e.getID() == id) {
        dup = true;
        break;
      }
    }
    if (dup) {
      cout << "  [!] ID nay da ton tai!\n";
      continue;
    }
    break;
  }

  while (true) {
    cout << "Nhap vao ho ten nhan vien: ";
    getline(cin, name);
    if (name.empty()) {
      cout << "  [!] Khong duoc de trong!\n";
      continue;
    }
    if (name.length() > 50) {
      cout << "  [!] Ho ten toi da 50 ky tu!\n";
      continue;
    }
    break;
  }

  while (true) {
    cout << "Nhap vao mat khau (6->12 ky tu): ";
    getline(cin, pass);
    if (pass.length() < 6 || pass.length() > 12) {
      cout << "  [!] Mat khau phai tu 6 den 12 ky tu!\n";
      continue;
    }
    bool hasSpace = false;
    for (char c : pass) {
      if (isspace((unsigned char)c)) {
        hasSpace = true;
        break;
      }
    }
    if (hasSpace) {
      cout << "  [!] Mat khau khong duoc chua khoang trang!\n";
      continue;
    }
    break;
  }

  Employee e(id, name, pass, 2);
  emlist.push_back(e);
  saveEmployees();
  cout << "\n  [+] Them nhan vien thanh cong!\n";
}

// ============================================================
//  Xoá nhân viên theo ID
// ============================================================
void EmployeeManager::deleteEmployee() {
  string id;
  while (true) {
    cout << "Nhap ma NV can xoa: ";
    getline(cin, id);
    if (id.empty()) {
      cout << "  [!] Khong duoc de trong!\n";
      continue;
    }
    if (Utils::hasInvalidChar(id)) {
      cout << "  [!] ID khong hop le (khong chua khoang trang/ky tu dac biet)!\n";
      continue;
    }
    break;
  }
  for (auto it = emlist.begin(); it != emlist.end(); ++it) {
    if (it->getID() == id) {
      emlist.erase(it);
      saveEmployees();
      cout << "  [+] Da xoa nhan vien!\n";
      return;
    }
  }
  cout << "  [!] Khong tim thay nhan vien!\n";
}

// ============================================================
//  Cập nhật thông tin nhân viên
// ============================================================
void EmployeeManager::updateEmployee() {
  string id;
  while (true) {
    cout << "Nhap ma nhan vien can cap nhat: ";
    getline(cin, id);
    if (id.empty()) {
      cout << "  [!] Khong duoc de trong!\n";
      continue;
    }
    if (Utils::hasInvalidChar(id)) {
      cout << "  [!] ID khong hop le (khong chua khoang trang/ky tu dac biet)!\n";
      continue;
    }
    break;
  }
  for (auto &e : emlist) {
    if (e.getID() == id) {
      cout << "  [+] Tim thay nhan vien: " << e.getName() << "!\n";
      string name, pass;

      while (true) {
        cout << "Ten moi: ";
        getline(cin, name);
        if (name.empty()) {
          cout << "  [!] Khong duoc de trong!\n";
          continue;
        }
        if (name.length() > 50) {
          cout << "  [!] Ho ten toi da 50 ky tu!\n";
          continue;
        }
        break;
      }

      while (true) {
        cout << "Password moi (6->12 ky tu): ";
        getline(cin, pass);
        if (pass.length() < 6 || pass.length() > 12) {
          cout << "  [!] Mat khau phai tu 6 den 12 ky tu!\n";
          continue;
        }
        bool hasSpace = false;
        for (char c : pass) {
          if (isspace((unsigned char)c)) {
            hasSpace = true;
            break;
          }
        }
        if (hasSpace) {
          cout << "  [!] Mat khau khong duoc chua khoang trang!\n";
          continue;
        }
        break;
      }

      e.setName(name);
      e.setPassword(pass);
      saveEmployees();
      cout << "  [+] Cap nhat thanh cong!\n";
      return;
    }
  }
  cout << "  [!] Khong tim thay nhan vien!\n";
}
