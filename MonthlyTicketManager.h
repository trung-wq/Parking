#pragma once
#include "MonthlyTicket.h"
#include <string>
#include <vector>

class ParkingStorage;

class MonthlyTicketManager {
private:
  vector<MonthlyTicket> tickets;
  const string filename = "MonthlyTickets.txt";
  int counterXD = 1;
  int counterXM = 1;
  int counterOT = 1;

public:
  MonthlyTicketManager();
  void addTicket(string empID, ParkingStorage &storage);
  void renewTicket(string empID, ParkingStorage &storage);
  void manageTicket(string empID, ParkingStorage &storage);
  void showAllTickets();
  void showExpiringSoon();
  void showStatistics();
  void logRevenue(const MonthlyTicket &t, string empID);
  bool isValid(const string &plate);
  int checkTicket(const string &plate); // 0: No ticket, 1: Valid, 2: Expired,
                                        // 3: Expiring soon, 4: Locked
  int checkTicketByID(const string &id);
  string getTicketID(const string &plate);
  time_t getExpirationDate(const string &plate);
  time_t getExpirationDateByID(const string &id);
  string getPlateByID(const string &id);

  int countTicketsByType(int type);

  void loadFromFile();
  void saveToFile();

  // Helpers
  string formatTime(time_t t);
};
