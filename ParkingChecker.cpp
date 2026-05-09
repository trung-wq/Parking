#include "ParkingChecker.h"
#include "Utils.h"
using namespace std;

// ============================================================
//  Hiển thị tình trạng slot còn trống
// ============================================================
void ParkingChecker::showAvailableSlots() {
  int countBicycle, countMotorbike, countCar;
  storage.countVehicles(countBicycle, countMotorbike, countCar);

  VehicleConfig cfgB = storage.getVehicleConfig(1);
  VehicleConfig cfgM = storage.getVehicleConfig(2);
  VehicleConfig cfgC = storage.getVehicleConfig(3);

  int remBicycle = cfgB.maxCapacity - countBicycle;
  int remMotorbike = cfgM.maxCapacity - countMotorbike;
  int remCar = cfgC.maxCapacity - countCar;
  int remTotal = remBicycle + remMotorbike + remCar;
  int usedTotal = countBicycle + countMotorbike + countCar;
  int maxTotal = cfgB.maxCapacity + cfgM.maxCapacity + cfgC.maxCapacity;

  cout << "\n========================================\n";
  cout << "        TINH TRANG BAI GIU XE\n";
  cout << "========================================\n";
  cout << "  Loai xe   | Da dung | Con trong | Tong\n";
  cout << "----------------------------------------\n";
  cout << "  Xe dap    |   " << countBicycle << "    |    " << remBicycle
       << "      |  " << cfgB.maxCapacity << "\n";
  cout << "  Xe may    |   " << countMotorbike << "     |    " << remMotorbike
       << "     |  " << cfgM.maxCapacity << "\n";
  cout << "  O to      |   " << countCar << "     |    " << remCar << "     |  "
       << cfgC.maxCapacity << "\n";
  cout << "----------------------------------------\n";
  cout << "  Tong cong |   " << usedTotal << "    |    " << remTotal
       << "     |  " << maxTotal << "\n";
  cout << "========================================\n";

  // Hiển thị chi tiết từng khu vực
  cout << "\n  CHI TIET CAC KHU VUC:\n";
  for (const auto &zone : storage.getZones()) {
    int avail = 0;
    for (const auto &slot : zone.slots) {
      if (slot.status == SlotStatus::Available)
        avail++;
    }
    cout << "  - " << left << setw(20) << zone.name << ": " << avail
         << " cho trong\n";
  }
}

// ============================================================
//  Danh sách xe đang trong bãi
// ============================================================
void ParkingChecker::showListParking() {
  storage.loadFromFile(monthlyManager);
  queue<Vehicle *> &pq = storage.getQueue();
  if (pq.empty()) {
    cout << "Hien tai khong co xe nao trong bai!\n";
    return;
  }
  queue<Vehicle *> temp = pq;
  while (!temp.empty()) {
    Vehicle *v = temp.front();
    if (v->getType() == 1)
      cout << "[Xe Dap] " << v->getPlate() << " " << v->getTicket().getDateIn()
           << " " << v->formatTime(v->getTicket().getTimeIn()) << endl;
    else if (v->getType() == 2)
      cout << "[Xe May] " << v->getPlate() << " " << v->getTicket().getDateIn()
           << " " << v->formatTime(v->getTicket().getTimeIn()) << endl;
    else
      cout << "[O TO]   " << v->getPlate() << " " << v->getTicket().getDateIn()
           << " " << v->formatTime(v->getTicket().getTimeIn()) << endl;
    temp.pop();
  }
}

// ============================================================
//  Bảng hiển thị đầy đủ, phân nhóm theo loại xe
// ============================================================
void ParkingChecker::display() {
  int countBicycle, countMotorbike, countCar;
  storage.countVehicles(countBicycle, countMotorbike, countCar);

  VehicleConfig cfgB = storage.getVehicleConfig(1);
  VehicleConfig cfgM = storage.getVehicleConfig(2);
  VehicleConfig cfgC = storage.getVehicleConfig(3);

  int remBicycle = cfgB.maxCapacity - countBicycle;
  int remMotorbike = cfgM.maxCapacity - countMotorbike;
  int remCar = cfgC.maxCapacity - countCar;
  int remTotal = remBicycle + remMotorbike + remCar;
  int usedTotal = countBicycle + countMotorbike + countCar;
  int maxTotal = cfgB.maxCapacity + cfgM.maxCapacity + cfgC.maxCapacity;

  /* (Phan thong ke vi tri da duoc luoc bo theo yeu cau) */

  queue<Vehicle *> &pq = storage.getQueue();
  if (pq.empty()) {
    cout << "\n  [!] Hien tai khong co xe nao trong bai!\n";
    return;
  }

  vector<Vehicle *> bicycles, motorbikes, cars;
  queue<Vehicle *> temp = pq;
  while (!temp.empty()) {
    Vehicle *v = temp.front();
    if (v->getType() == 1)
      bicycles.push_back(v);
    else if (v->getType() == 2)
      motorbikes.push_back(v);
    else
      cars.push_back(v);
    temp.pop();
  }

  auto printGroup = [](const string &title, const string &icon,
                       vector<Vehicle *> &list, int maxSlot) {
    if (list.empty())
      return;
    cout << "\n  " << icon << " " << title << " (" << list.size() << "/"
         << maxSlot << " cho dang dung)\n";
    cout << "  "
            "+----+------------+------------+------------+-------+----------+--"
            "------+----------+\n";
    cout << "  | STT| Bien so    | Ma ve      | Ngay vao   | Gio   | Loai     "
            "| Vi tri | T.Gian   |\n";
    cout << "  "
            "+----+------------+------------+------------+-------+----------+--"
            "------+----------+\n";
    int idx = 1;
    time_t now = time(0);
    for (Vehicle *v : list) {
      string plate = v->getPlate().empty() ? "(khong co)" : v->getPlate();
      string id = v->getTicket().getId();
      string date = v->getTicket().getDateIn();
      string timeStr = v->formatTime(v->getTicket().getTimeIn());
      string type = v->getTicket().getIsMonthly() ? "Ve thang" : "Ve luot";
      string slot = v->getTicket().getSlotCode();
      string duration = Utils::formatDuration(now - v->getTicket().getTimeIn());

      cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
           << plate << "| " << left << setw(11) << id << "| " << left
           << setw(11) << date << "| " << left << setw(6) << timeStr << "| "
           << left << setw(9) << type << "| " << left << setw(7) << slot
           << "| " << left << setw(9) << duration << "|\n";
    }
    cout << "  "
            "+----+------------+------------+------------+-------+----------+--"
            "------+----------+\n";
  };

  cout << "\n           >>> CHI TIET DANH SACH XE DANG GUI <<<\n";
  printGroup("XE DAP", "[DAP]", bicycles, cfgB.maxCapacity);
  printGroup("XE MAY", "[MAY]", motorbikes, cfgM.maxCapacity);
  printGroup("O TO", "[OTO]", cars, cfgC.maxCapacity);
  cout << "\n=================================================================="
          "======\n";
}

// ============================================================
//  Tìm kiếm xe theo mã vé hoặc biển số
// ============================================================
bool ParkingChecker::search() {
  while (true) {
    queue<Vehicle *> &pq = storage.getQueue();
    if (pq.empty()) {
      cout << "\n  [!] Hien tai khong co xe nao trong bai!\n";
      return true;
    }

    cout << "\n---------- TIM KIEM XE ----------\n";
    cout << "  1. Theo ma ve\n";
    cout << "  2. Theo bien so\n";
    cout << "  3. Theo ngay vao (DD/MM/YYYY)\n";
    cout << "  0. Quay lai\n";
    cout << "-------------------------------\n";
    cout << "  Chon: ";
    int opt = Utils::readMenuChoice(0, 3);
    if (opt == 0)
      return false;

    string inputStr;
    if (opt == 1) {
      while (true) {
        cout << "  Nhap ma ve: ";
        getline(cin, inputStr);
        inputStr = Utils::normalizeString(inputStr);
        if (inputStr.empty()) {
          cout << "  [!] Khong duoc de trong!\n";
          continue;
        }
        break;
      }
    } else if (opt == 2) {
      inputStr = storage.readPlate(false);
    } else {
      while (true) {
        cout << "  Nhap ngay (DD/MM/YYYY): ";
        getline(cin, inputStr);
        if (Utils::isValidDate(inputStr))
          break;
        cout << "  [!] Ngay khong hop le (Dinh dang dung: DD/MM/YYYY)!\n";
      }
    }

    queue<Vehicle *> temp = pq;
    bool found = false;
    while (!temp.empty()) {
      Vehicle *v = temp.front();
      bool isMatch = false;
      if (opt == 1 && v->getTicket().getId() == inputStr)
        isMatch = true;
      else if (opt == 2 && v->getPlate() == inputStr && !v->getPlate().empty())
        isMatch = true;
      else if (opt == 3 && v->getTicket().getDateIn() == inputStr)
        isMatch = true;

      if (isMatch) {
        if (!found) {
          cout << "\n========================================\n";
          cout << "         THONG TIN XE TIM THAY\n";
          cout << "========================================\n";
        } else {
          cout << "----------------------------------------\n";
        }
        string typeStr =
            (v->getType() == 1 ? "Xe dap"
                               : (v->getType() == 2 ? "Xe may" : "O to"));
        string ticketType =
            v->getTicket().getIsMonthly() ? "Ve thang" : "Ve luot";
        string plate = v->getPlate().empty() ? "(khong co)" : v->getPlate();
        string duration = storage.getDurationString(v->getTicket().getTimeIn(),
                                                    time(nullptr));

        cout << "  - Bien so    : " << plate << endl;
        cout << "  - Ma ve      : " << v->getTicket().getId() << endl;
        cout << "  - Loai xe    : " << typeStr << endl;
        cout << "  - Loai ve    : " << ticketType << endl;
        cout << "  - Ngay vao   : " << v->getTicket().getDateIn() << endl;
        cout << "  - Gio vao    : " << v->formatTime(v->getTicket().getTimeIn())
             << endl;
        cout << "  - Vi tri do  : " << v->getTicket().getSlotCode() << endl;
        cout << "  - Thoi gian da gui: " << duration << endl;
        found = true;
        
        // Neu tim theo ma ve hoac bien so thi return ngay (vi duy nhat)
        // Con tim theo ngay thi tiep tuc de tim tat ca cac xe trong ngay do
        if (opt == 1 || opt == 2) {
          cout << "========================================\n";
          return true;
        }
      }
      temp.pop();
    }
    
    if (found) {
      cout << "========================================\n";
      return true;
    } else {
      cout << "\n  [!] KHONG TIM THAY XE PHU HOP: " << inputStr << "\n";
      cout << "\n  1. Tiep tuc tim xe khac\n";
      cout << "  2. Quay lai menu\n";
      cout << "  Chon: ";
      int retryOpt = Utils::readMenuChoice(1, 2);
      if (retryOpt == 2)
        return false;
    }
  }
}
