#ifndef database_h
#define database_h

#include <sqlite3.h>
#include <vector>

extern sqlite3* DB;

bool open_db();
bool restart_db();
void close_db();
bool create_table();
bool load(const std::string& query);
int get_id(const std::string& username);
int get_last_insert_id();
int get_BankID(const unsigned long& acc_num);
int get_UserID_By_BankAccount(const unsigned long& acc_num);
std::vector<unsigned long> Get_All_Bank_Account(const std::string& username);
unsigned long get_AccountNumber(const int& id);
std::string get_name(const unsigned long& account_number);
float get_balance(const int& session_id);
std::string get_phone_number(const int& session_id);
bool check_email(const std::string& email);
bool check_username(const std::string& user_name);
bool check_account_number(const unsigned long& acc_num);
bool Update_Balance_of_Customer(const unsigned long& acc_num, const float& amount, const char& status);
bool Add_Transfer_History_of_Customer(const unsigned long& acc_num, const float& amount, const char& type, const std::string& other_details);

#endif