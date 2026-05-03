#pragma once
#include <iostream>
using namespace std;

class Employee {
private:
  string id;
  string name;
  string password;
  int role; // 1: admin, 2: staff

public:
  Employee(string i = "", string n = "", string p = "", int r = 2) {
    id = i;
    name = n;
    password = p;
    role = r;
  }

  string getID() { return id; }
  string getName() { return name; }
  string getPassword() { return password; }
  int getRole() { return role; }

  void setName(string n) { name = n; }
  void setPassword(string p) { password = p; }

  bool login(string i, string p) { return (i == id && p == password); }

  void display() {
    cout << id << " | " << name << " | " << (role == 1 ? "Admin" : "Staff")
         << endl;
  }
};