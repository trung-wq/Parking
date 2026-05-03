#include "ParkingChecker.h"
#include "Utils.h"
using namespace std;

// ============================================================
//  Hiển thị tình trạng slot còn trống
// ============================================================
void ParkingChecker::showAvailableSlots() {
  int countBicycle, countMotorbike, countCar;
  storage.countVehicles(countBicycle, countMotorbike, countCar);

  const int maxBicycle = 20, maxMotorbike = 40, maxCar = 30, maxTotal = 90;
  int remBicycle = maxBicycle - countBicycle;
  int remMotorbike = maxMotorbike - countMotorbike;
  int remCar = maxCar - countCar;
  int remTotal = remBicycle + remMotorbike + remCar;
  int usedTotal = countBicycle + countMotorbike + countCar;

  cout << "\n========================================\n";
  cout << "        TINH TRANG BAI GIU XE\n";
  cout << "========================================\n";
  cout << "  Loai xe   | Da dung | Con trong | Tong\n";
  cout << "----------------------------------------\n";
  cout << "  Xe dap    |   " << countBicycle << "    |    " << remBicycle
       << "      |  " << maxBicycle << "\n";
  cout << "  Xe may    |   " << countMotorbike << "     |    " << remMotorbike
       << "     |  " << maxMotorbike << "\n";
  cout << "  O to      |   " << countCar << "     |    " << remCar << "     |  "
       << maxCar << "\n";
  cout << "----------------------------------------\n";
  cout << "  Tong cong |   " << usedTotal << "    |    " << remTotal
       << "     |  " << maxTotal << "\n";
  cout << "========================================\n";
}

// ============================================================
//  Danh sách xe đơn giản (dùng trong menu chính)
// ============================================================
void ParkingChecker::showListParking() {
  storage.loadFromFile();
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
  queue<Vehicle *> &pq = storage.getQueue();
  if (pq.empty()) {
    cout << "\n  Hien tai khong co xe nao trong bai!\n";
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
    cout << "\n  " << icon << " " << title << " (" << list.size() << "/"
         << maxSlot << " cho)\n";
    cout << "  +----+------------+------------+------------+-------+\n";
    cout << "  | STT| Bien so    | Ma ve      | Ngay vao   | Gio   |\n";
    cout << "  +----+------------+------------+------------+-------+\n";
    int idx = 1;
    for (Vehicle *v : list) {
      string plate = v->getPlate().empty() ? "(khong co)" : v->getPlate();
      string id = v->getTicket().getId();
      string date = v->getTicket().getDateIn();
      string time = v->formatTime(v->getTicket().getTimeIn());
      cout << "  | " << left << setw(3) << idx++ << "| " << left << setw(11)
           << plate << "| " << left << setw(11) << id << "| " << left
           << setw(11) << date << "| " << left << setw(6) << time << "|\n";
    }
    cout << "  +----+------------+------------+------------+-------+\n";
  };

  cout << "\n========================================\n";
  cout << "       DANH SACH XE DANG TRONG BAI\n";
  cout << "========================================\n";
  printGroup("XE DAP", "[DAP]", bicycles, 20);
  printGroup("XE MAY", "[MAY]", motorbikes, 40);
  printGroup("O TO", "[OTO]", cars, 30);

  int total = bicycles.size() + motorbikes.size() + cars.size();
  cout << "\n  Tong so xe dang gui: " << total << "/90\n";
  cout << "========================================\n";
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
    cout << "  3. Quay lai\n";
    cout << "-------------------------------\n";
    cout << "  Chon: ";
    int opt = Utils::readMenuChoice(1, 3);
    if (opt == 3)
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
        if (Utils::hasInvalidChar(inputStr)) {
          cout << "  [!] Chi chap nhan chu cai va so!\n";
          continue;
        }
        break;
      }
    } else {
      inputStr = storage.readPlate(false);
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

      if (isMatch) {
        cout << "\n========================================\n";
        cout << "         THONG TIN XE TIM THAY\n";
        cout << "========================================\n";
        v->display();
        cout << "========================================\n";
        found = true;
        return true;
      }
      temp.pop();
    }
    if (!found) {
      cout << "\n  [!] KHONG TIM THAY XE: " << inputStr << "\n";
      cout << "\n  1. Tiep tuc tim xe khac\n";
      cout << "  2. Quay lai menu\n";
      cout << "  Chon: ";
      int retryOpt = Utils::readMenuChoice(1, 2);
      if (retryOpt == 2)
        return false;
    }
  }
}
