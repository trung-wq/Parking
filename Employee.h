#pragma once
#include <iostream>
using namespace std;

class Employee {

private:

    string id;
    string name;

public:

    Employee(string i = "NV01", string n = "Admin") {
        id = i;
        name = n;
    }

    void display() {
        cout << "Nhan vien: " << name << " (" << id << ")\n";
    }
};