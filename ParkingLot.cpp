#include "ParkingLot.h"

void ParkingLot::saveToFile()
{
    ofstream out("data.txt");
    //out << revenue << endl;

    queue<Vehicle*> temp = parkingQueue;
    while (!temp.empty())
    {
        Vehicle* v = temp.front();

        //out << v->getPlate() << endl;
        out << v->getType() << " "
            << v->getPlate() << " "
            << v->getTicket().getDateIn() << " "
            << v->getTicket().getTimeIn() << endl;
            //<< v->getTicket().getTimeOut() << endl;

        temp.pop();
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
            << v->getTicket().getDateIn()<<" "
            << v->getTicket().getTimeIn()<< " "
            << v->getTicket().getDateOut()<<" "
            << v->getTicket().getTimeOut() << " "
            << v->calculateFee() << endl;

        temp.pop();
    }

    out.close();
    cout << "Da luu lich su xe roi bai!\n";
    loadHistoryFromFile();
}
void ParkingLot::ShowHistory()
{
    
    if (history.empty()) 
    {
        cout << "Chua co xe nao roi bai!\n";
        return;
    }
    stack<Vehicle*> temp = history;
    cout << "\n===== LICH SU XE ROI BAI =====\n";

    while (!temp.empty()) 
    {
        Vehicle* v = temp.top();

        v->display();

        /*cout << "Gio vao: "
            << v->formatTime(v->getTicket().getTimeIn()) << endl;*/
        cout << "Gio ra: "
            << v->formatTime(v->getTicket().getTimeOut()) << endl;
        cout << "Ngay ra: "
            << v->getTicket().getDateOut() << endl;

        


        cout << "Tien: "
            << v->calculateFee() << " VND\n";

        cout << "----------------------\n";

        temp.pop();
    }
}
void ParkingLot::loadHistoryFromFile()
{
    ifstream in("history.txt");
    if (!in)
    {
        cout << "Khong mo duoc file history!\n";
        return;
    }
    history = stack<Vehicle*>();
    int type, fee;
    string plate;
    time_t timeIn, timeOut;
    string Indate, Outdate;
    while (in >> type >> plate >>Indate>> timeIn >>Outdate>> timeOut >> fee) {

        Vehicle* v;

        if (type == 1) v = new Bicycle(plate, "T" + plate);
        else if (type == 2) v = new Motorbike(plate, "T" + plate);
        else v = new Car(plate, "T" + plate);

        // Gán lại thời gian
        v->getTicket()._setDateIn(Indate);
        v->getTicket().setTimeIn(timeIn);
        v->getTicket()._setDateOut(Outdate);
        v->getTicket()._setTimeOut(timeOut);

        // Đưa vào stack history
        history.push(v);
    }
    in.close();
    cout << "Da tai lich su tu file!\n";
}
void ParkingLot::loadFromFile() {
    ifstream in("data.txt");

    if (!in) {
        cout << "Khong co file!\n";
        return;
    }
    while (!parkingQueue.empty())
    {
        parkingQueue.pop();
    }
    // Đọc doanh thu
    //in >> revenue;

    int type;
    string plate;
    time_t inTime;
    string inDate;

    while (in >> type >> plate >>inDate>> inTime) {

        Vehicle* v;

        if (type == 1) v = new Bicycle(plate, "T" + plate);
        else if (type == 2) v = new Motorbike(plate, "T" + plate);
        else v = new Car(plate, "T" + plate);

        // ❗ GÁN LẠI THỜI GIAN
        v->getTicket()._setDateIn(inDate);
        v->getTicket().setTimeIn(inTime);
        
        
        //v->getTicket()._setTimeOut(outTime);

        parkingQueue.push(v);
    }


    in.close();
    cout << "Da tai du lieu!\n";
}
void ParkingLot::addVehicle() {

    int type;
    string plate;

    cout << "1 Xe dap\n2 Xe may\n3 O to\nChon: ";
    cin >> type;

    cin.ignore();
    cout << "Nhap bien so: ";
    getline(cin,plate);

    string ticketID = "T" + plate;

    Vehicle* v;

    if (type == 1)
        v = new Bicycle(plate, ticketID);
    else if (type == 2)
        v = new Motorbike(plate, ticketID);
    else
        v = new Car(plate, ticketID);

    parkingQueue.push(v);

    emp.display();
    v->getTicket().display();
    saveToFile();
    cout << "Them xe thanh cong!\n";
}

void ParkingLot::removeVehicle() {

    if (parkingQueue.empty()) {
        cout << "Bai xe rong\n";
        return;
    }
    string _plate;
    cout << "Nhap vao bien so xe: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, _plate);

    queue<Vehicle*> temp;
    bool found = false;
    while (!parkingQueue.empty())
    {
        Vehicle* v = parkingQueue.front();
        parkingQueue.pop();
        if (v->getPlate() == _plate && !found)
        {
            v->getTicket().setTimeOut();

            int fee = v->calculateFee();
            cout << "Xe ra" << endl;
            cout << "Tien gui: " << fee << " VND\n";

            revenue += fee;
            
            history.push(v);
           
            //loadFromFile();
            found = true;
        }
        else
        {
            temp.push(v);
        }
    }
    parkingQueue = temp;
    
    if (!found) {
        cout << "Khong tim thay xe co bien so: " << _plate << endl;
    }
    else {
        saveToFile();
        saveHistoryToFile();
    }
   
}
void ParkingLot::showListParking()
{
    loadFromFile();
    queue<Vehicle*> temp = parkingQueue;
    while (!temp.empty())
    {
        Vehicle* v = temp.front();
        if (v->getType() == 1)
        {
            cout << "[Xe Dap]" << " "
                << v->getPlate() << " "
                //<< v->getPlate() <<" "
                << v->getTicket().getDateIn()<<" "
                << v->formatTime(v->getTicket().getTimeIn()) << endl;
        }
        else if (v->getType() == 2)
        {
            cout << "[Xe May]" << " "
                << v->getPlate() << " "
                << v->getTicket().getDateIn() << " "
                << v->formatTime(v->getTicket().getTimeIn()) << endl;
        }
        else
        {
            cout << "[O TO]" << " "
                << v->getPlate() << " "
                << v->getTicket().getDateIn() << " "
                << v->formatTime(v->getTicket().getTimeIn()) << endl;
        }
        //out << v->getPlate() << endl;
        temp.pop();
    }

}
void ParkingLot::display() {

    queue<Vehicle*> temp = parkingQueue;

    while (!temp.empty()) {

        temp.front()->display();
        cout << "----------------\n";
        temp.pop();
    }
}

void ParkingLot::search() {

    string plate;
    cout << "Nhap bien so: ";
    cin >> plate;

    queue<Vehicle*> temp = parkingQueue;

    while (!temp.empty()) {

        if (temp.front()->getPlate() == plate) {

            cout << "Tim thay:\n";
            temp.front()->display();
            return;
        }

        temp.pop();
    }

    cout << "Khong tim thay\n";
}

void ParkingLot::showRevenue() {

    cout << "Tong doanh thu: " << revenue << " VND\n";
}
void ParkingLot::revenueByDate()
{
    if (history.empty())
    {
        cout << "Chua co du lieu!!\n";
        return;
    }
    string inputDate;
    cout << "Nhap ngay (dd/mm/yyyy): ";
    cin >> inputDate;
    stack<Vehicle*> temp = history;
    int total = 0;
    bool found = false;
    while (!temp.empty())
    {
        Vehicle* v = temp.top();
        string dateOut = v->getTicket().getDateOut();
        if (dateOut == inputDate)
        {
            total += v->calculateFee();
            found = true;
        }
        temp.pop();
    }
    if (found)
    {
        cout << "Doanh thu thu ngay " << inputDate << " la: " << total << "VND\n";
    }
    else {
        cout << "Ngay hom do khong co doanh thu!\n";
    }
    
}
void getDateParts(time_t t, int& d, int& m, int& y)
{
    tm info;
    localtime_s(&info, &t);

    d = info.tm_mday;
    m = info.tm_mon + 1;
    y = info.tm_year + 1900;
}
bool isValidDate(int d, int m, int y) {
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    // Năm nhuận
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    if (d > daysInMonth[m - 1]) return false;

    return true;
}
bool isValidMonth( int m, int y) {
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    return true;
}
bool isValidYear( int y) {
    if (y < 1900 || y > 2100) return false;

    return true;
}
void ParkingLot::revenueByDay() {
    int d, m, y;
   
    

    while (true) {
        cout << "Nhap ngay thang nam (dd mm yyyy): ";
        cin >> d >> m >> y;

       
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Nhap sai! Vui long nhap so.\n";
            continue;
        }

        
        if (!isValidDate(d, m, y)) {
            cout << "Ngay thang khong hop le!\n";
            continue;
        }

        break; // hợp lệ
    }

    stack<Vehicle*> temp = history;

    int total = 0;

    while (!temp.empty()) {
        Vehicle* v = temp.top();

        int dd, mm, yy;
        getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);

        if (dd == d && mm == m && yy == y) {
            v->display();
            cout << "Gio ra: "
                << v->formatTime(v->getTicket().getTimeOut()) << endl;
            cout << "Ngay ra: "
                << v->getTicket().getDateOut() << endl;
            cout << "----------------------\n";
            total += v->calculateFee();
        }

        temp.pop();
    }

    cout << "Doanh thu ngay " << d << "/" << m << "/" << y
        << " = " << total << " VND\n";
}
void ParkingLot::revenueByMonth() 
{
    int m, y;
   
    while (true) {
        cout << "Nhap thang nam ( mm yyyy): ";
        cin >> m >> y;


        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Nhap sai! Vui long nhap so.\n";
            continue;
        }


        if (!isValidMonth( m, y)) {
            cout << "Ngay thang khong hop le!\n";
            continue;
        }

        break; // hợp lệ
    }

    stack<Vehicle*> temp = history;

    int total = 0;

    while (!temp.empty()) {
        Vehicle* v = temp.top();

        int dd, mm, yy;
        getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);

        if (mm == m && yy == y) {
            v->display();
            cout << "Gio ra: "
                << v->formatTime(v->getTicket().getTimeOut()) << endl;
            cout << "Ngay ra: "
                << v->getTicket().getDateOut() << endl;
            cout << "----------------------\n";
            total += v->calculateFee();
        }

        temp.pop();
    }

    cout << "Doanh thu thang " << m << "/" << y
        << " = " << total << " VND\n";
}
void ParkingLot::revenueByYear() 
{
    int y;
    cout << "Nhap nam: ";
    while (true) {
        cout << "Nhap nam: ";
        cin >> y;


        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Nhap sai! Vui long nhap so.\n";
            continue;
        }


        if (!isValidYear(y)) {
            cout << "Ngay thang khong hop le!\n";
            continue;
        }

        break; // hợp lệ
    }

    stack<Vehicle*> temp = history;

    int total = 0;

    while (!temp.empty()) {
        Vehicle* v = temp.top();

        int dd, mm, yy;
        getDateParts(v->getTicket().getTimeOut(), dd, mm, yy);

        if (yy == y) {
            v->display();
            cout << "Gio ra: "
                << v->formatTime(v->getTicket().getTimeOut()) << endl;
            cout << "Ngay ra: "
                << v->getTicket().getDateOut() << endl;
            cout << "----------------------\n";
            
            total += v->calculateFee();
        }

        temp.pop();
    }

    cout << "Doanh thu nam " << y
        << " = " << total << " VND\n";
}