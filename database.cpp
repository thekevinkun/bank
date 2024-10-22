#include <iostream>
#include <string.h>
#include <unistd.h>

#include "database.h"

sqlite3* DB;

// OPEN or CREATE THE DATABESES //
bool open_db()
{
    const char* filename = "bank.db";   

    // Create and Open the bank.db databases
    int exit = sqlite3_open(filename, &DB);
    // If return 0, means database opened
    if (exit)
    {
        std::cerr << "\tFailed to open " <<  sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return false;   
    }

    return true;
}

// RESTART DATABASE //
bool restart_db()
{
    close_db();
    bool isOpen = open_db();
    if (!isOpen)
        return false;

    return true;
}

// CLOSE DATABASE
void close_db()
{
    sqlite3_close(DB);
}

// CREATE TABLE IN DATABESES //
bool create_table()
{
    int exit = 0;
    char* messageError;
    std::string query;

    // CREATE CUSTOMERS TABLE that contain people information
    query = "CREATE TABLE IF NOT EXISTS Customers (\n"
            "\tid INTEGER PRIMARY KEY AUTOINCREMENT,\n"
            "\tfirst_name VARCHAR(255) NOT NULL,\n"
            "\tlast_name VARCHAR(255) NOT NULL,\n"
            "\tbirth_place VARCHAR(25) NOT NULL,\n"
            "\tbirth_date DATE NOT NULL,\n"
            "\tgender VARCHAR(10) NOT NULL,\n"
            "\taddress VARCHAR(255) NOT NULL,\n"
            "\thandphone VARCHAR(20) NOT NULL\n"
        ");";  

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create Customers table: "<< messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // CREATE BANK ACCOUNT TABLE that contains all information about user account
    query = "CREATE TABLE IF NOT EXISTS BankAccount (\n"
            "\tid INTEGER PRIMARY KEY AUTOINCREMENT,\n"
            "\taccount_number INT NOT NULL,\n"
            "\tbalance DECIMAL(15,2) NOT NULL,\n"
            "\tcreation_year DATE NOT NULL DEFAULT CURRENT_DATE\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create BankAccount table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // CREATE CUSTOMER-ACCOUNT TABLE that connects Customers with BankAccount via id
    query = "CREATE TABLE IF NOT EXISTS CustomerAccount (\n"
            "\tcustomer_id INTEGER,\n"
            "\taccount_id INTEGER,\n"
            "\tFOREIGN KEY (customer_id) REFERENCES Customers(id),\n"
            "\tFOREIGN KEY (account_id) REFERENCES BankAccount(id)\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create CustomerAccount table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // CREATE USER ACCOUNT TABLE that contains all information about user login
    query = "CREATE TABLE IF NOT EXISTS LoginProfile (\n"
            "\tid INTEGER PRIMARY KEY AUTOINCREMENT,\n"
            "\temail VARCHAR(50) NOT NULL,\n"
            "\tusername VARCHAR(50) NOT NULL,\n"
            "\tpassword CHAR(60) NOT NULL\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create LoginProfile table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // CREATE LOGIN-ACCOUNT TABLE that will connects LoginProfile with BankAccount via id
    query = "CREATE TABLE IF NOT EXISTS LoginAccount (\n"
            "\taccount_id INTEGER,\n"
            "\tlogin_id INTEGER,\n"
            "\tFOREIGN KEY (account_id) REFERENCES BankAccount(id),\n"
            "\tFOREIGN KEY (login_id) REFERENCES LoginProfile(id)\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create Login-Account table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }
    
    // CREATE CITY TABLE that contains all information about customer city lived in
    query = "CREATE TABLE IF NOT EXISTS Citizen (\n"
            "\tcustomer_id INTEGER,\n"
            "\tcountry VARCHAR(255) DEFAULT 'INDONESIA',\n"
            "\tcity VARCHAR(255) NOT NULL,\n"
            "\tzip_code CHAR(10) NOT NULL,\n"
            "\tcountry_code CHAR(10) NOT NULL,\n"
            "\tFOREIGN KEY (customer_id) REFERENCES customers(id)\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create City table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // CREATE TRANSACTION HISTORY TABLE
    query = "CREATE TABLE IF NOT EXISTS Transactions (\n"
            "\tid INTEGER PRIMARY KEY AUTOINCREMENT,\n"
            "\ttransaction_type VARCHAR(15) NOT NULL,\n"
            "\tamount DECIMAL(15,2) NOT NULL,\n"
            "\tother_details VARCHAR(40) DEFAULT '(none)',\n"
            "\ttimestamp DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP\n"
        ");";
    
    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create Transactions table: " << messageError << std::endl;
        sqlite3_free(messageError);
        sleep(5);
        return false;
    }

    // CREATE TRANSACTIONLOG TABLE that will connects Transactions with Customers and BankAccount via id
    query = "CREATE TABLE IF NOT EXISTS TransactionLog (\n"
            "\ttransaction_id INTEGER,\n"
            "\tcustomer_id INTEGER,\n"
            "\taccount_id INTEGER,\n"
            "\tFOREIGN KEY (transaction_id) REFERENCES Transactions(id)\n"
            "\tFOREIGN KEY (customer_id) REFERENCES Customers(id),\n"
            "\tFOREIGN KEY (account_id) REFERENCES BankAccount(id)\n"
        ");";

    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tFailed to create TransactionLog table: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    return true;
}

// LOAD INFROMATION TO DATABASE, TAKE QUERY AS PARAMATERS //
bool load(const std::string& query)
{
    char* messageError;

    int exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tError insert: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    return true;
}

// GET ALL BANK ACOOUNT FROM LOGIN USERNAME //
std::vector<unsigned long> Get_All_Bank_Account(const std::string& username)
{
    std::string query = "SELECT account_number FROM BankAccount WHERE id IN (SELECT account_id FROM LoginAccount WHERE login_id = (SELECT id FROM LoginProfile WHERE username = '"+ username +"'))";
    std::vector<unsigned long> acc;

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError get all account: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return acc;
    }

    int nCols = sqlite3_column_count(stmt);
    // Iterate to query's rows, count on every account number
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // Store every account number to vector, this will stop until last row or last account number
        for (int i = 0; i < nCols; i++)
            acc.push_back(sqlite3_column_int(stmt, i));
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
    }

    sqlite3_finalize(stmt);
    return acc;
}

// GET ACCOUNT NUMBER BY ACTIVE ID //
unsigned long get_AccountNumber(const int& id)
{
    std::string query = "SELECT account_number FROM BankAccount WHERE id = "+ std::to_string(id) +"";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return -1;
    }
   
    rc = sqlite3_step(stmt);

    unsigned long acc_num = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    if (acc_num == 0)
    {
        std::cerr << "\tFailed to get Accout Number" << std::endl;
        sleep(5);
        return -1;
    }

    return acc_num;
}

// GET ID OF LOGGED IN CUSTOMER //
int get_id(const std::string& username)
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT id FROM Customers WHERE id IN (SELECT customer_id FROM CustomerAccount WHERE account_id IN (SELECT account_id FROM LoginAccount WHERE login_id = (SELECT id FROM LoginProfile WHERE username = '"+ username +"')))";
    

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return -1;
    }
   
    rc = sqlite3_step(stmt);
    
    int id = sqlite3_column_int(stmt, 0);
    
    sqlite3_finalize(stmt);

    // If there's no customer in databese yet, sqlite3 will return 0
    if (id == 0)
    {
        std::cerr << "\tInvalid: Get Customer ID" << std::endl;
        sleep(5);
        return -1;
    }
    
    return id;
}

// GET ID OF ACTIVE ACCOUNT NUMBER //
int get_BankID(const unsigned long& acc_num)
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT id FROM BankAccount WHERE account_number = "+ std::to_string(acc_num) +"";

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(4);
        return -1;
    }
   
    rc = sqlite3_step(stmt);

    int the_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    if (the_id <= 0)
    {
        std::cerr << "\tInvalid: Get Account Number ID" << std::endl;
        sleep(5);
        return -1;
    }

    return the_id;
}

// GET Customers ID with Account Number as arguments //
int get_UserID_By_BankAccount(const unsigned long& acc_num)
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT customer_id FROM CustomerAccount WHERE account_id = (SELECT id FROM BankAccount WHERE account_number = "+ std::to_string(acc_num) +")";

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(4);
        return -1;
    }
   
    rc = sqlite3_step(stmt);

    int the_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    if (the_id <= 0)
    {
        std::cerr << "\tInvalid: Get Customer ID" << std::endl;
        sleep(5);
        return -1;
    }

    return the_id;
}

// GET ID OF LAST INSERT CUSTOMER //
int get_last_insert_id()
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT last_insert_rowid()";

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(4);
        return -1;
    }
   
    rc = sqlite3_step(stmt);
    
    int id = sqlite3_column_int(stmt, 0);
    
    sqlite3_finalize(stmt);

    // If there's no customer in databese yet, sqlite3 will return 0
    if (id == 0)
    {
        std::cerr << "\tCan't get last inserted id." << std::endl;
        sleep(4);
        return -1;
    }
    
    return id;
}

// GET NAME OF USER BASED ON THEY ACCOUNT NUMBER //
std::string get_name(const unsigned long& acc_num)
{
    std::string query = "SELECT first_name, last_name FROM Customers WHERE id IN (SELECT customer_id FROM CustomerAccount WHERE account_id = (SELECT id FROM BankAccount WHERE account_number = "+ std::to_string(acc_num) +"))";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "Error: " << sqlite3_errmsg(DB) << std::endl;
        exit(-1);
    }
    
    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the value from query
    std::string first_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string last_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string full_name = first_name + " " + last_name;

    sqlite3_finalize(stmt);

    return full_name;
}

// Get balance of account after login
float get_balance(const int& session_id)
{
    std::string query = "SELECT balance FROM BankAccount WHERE id = "+ std::to_string(session_id) +"";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(4);
        return false;
    }
    
    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the column value
    float balance = static_cast<float>(sqlite3_column_int(stmt, 0));

    sqlite3_finalize(stmt);

    return balance;
}

// Check if email already exist or not //
bool check_email(const std::string& email)
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT email FROM LoginProfile WHERE email = '"+ email +"';";
   
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return false;
    }
    
    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the column value
    const unsigned char* result = sqlite3_column_text(stmt, 0);

    bool isAvailable = true;

    // Check if there's no result (null), otherwise it will create error when we do comparison
    if (result == nullptr)
        isAvailable = true;
    // Check if email already been used
    else if (strcmp(&email[0], reinterpret_cast<const char*>(result)) == 0)
        isAvailable = false;
    
    sqlite3_finalize(stmt);
    
    return isAvailable;    
}

// Check if username already registered or not
bool check_username(const std::string& username)
{
    sqlite3_stmt* stmt;
    std::string query = "SELECT username FROM LoginProfile WHERE username = '"+ username +"';";
   
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "Error: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }
    
    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the column value
    const unsigned char* result = sqlite3_column_text(stmt, 0);
    
    bool isAvailable = true;

    // Check if there's no result (null), otherwise it will create error when we do comparison
    if (result == nullptr)
        isAvailable = true;
    // Check if username already been used
    else if (strcmp(&username[0], reinterpret_cast<const char*>(result)) == 0)
        isAvailable = false;
    
    sqlite3_finalize(stmt);
    
    return isAvailable;
}

// CHECK ACCOUNT NUMBER IF EXIST //
bool check_account_number(const unsigned long& acc_num)
{
    std::string query = "SELECT account_number FROM BankAccount WHERE account_number = "+ std::to_string(acc_num) +"";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "Error: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the value from query
    unsigned long account_num = sqlite3_column_int(stmt, 0);
    
    sqlite3_finalize(stmt);

    // Check if account number not registered
    if (account_num == 0)
        return false;
    
    return true;
}

// UPDATE BALANCE OF CUSTOMER BASED ON STATUS //
// Status: T (Transfer) and W (Withdrawer) | D (Deposit) and R (Receive)
bool Update_Balance_of_Customer(const unsigned long& acc_num, const float& amount, const char& status)
{
    std::string query;
    query = "SELECT balance FROM BankAccount WHERE account_number = "+ std::to_string(acc_num) +"";

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "Error: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return false;
    }
    
    // Move to result of query's row
    rc = sqlite3_step(stmt);

    // Get the value from query
    int balance = sqlite3_column_int(stmt, 0);
    int new_balance = 0;

    // IF status is W or T, change the balance of withdrawer or transferrer: substract it
    if (status == 'W' || status == 'T')
        new_balance = balance - amount;
    // IF status if D or R, change the balance of depositor or receiver: add
    else if (status == 'D' || status == 'R')
        new_balance = balance + amount;
    
    // UPDATE THE BALANCE OF CUSTOMER BASED ON GIVEN NAME //
    char* messageError;
    query = "UPDATE BankAccount SET balance = "+ std::to_string(new_balance) +" WHERE account_number = "+ std::to_string(acc_num) +"";
    
    int exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "Error Update: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// UPDATE TRANSFER HISTORY OF CUSTOMER BASED ON STATUS //
// Status: T (Transfer) and W (Withdrawer) | D (Deposit) and R (Receive)
// FOR TRANSFERRER, WE PASS RECEIVER ACCOUNT NUMBER || FOR RECEIVER, WE PASS TRANSFERRER ACCOUNT NUMBER //
bool Add_Transfer_History_of_Customer(const unsigned long& acc_num, const float& amount, const char& type, const std::string& other_details)
{   
    char *messageError;
    std::string query;

    // If transaction type is W, means transcation_type column is WITHDRAW
    if (type == 'W')
        query = "INSERT INTO Transactions(transaction_type, amount, other_details) VALUES('WITHDRAW', "+ std::to_string(amount) +", '"+ other_details +"')";
    // If transaction type is T, means transcation_type column is TRANSFER
    else if (type == 'T')
        query = "INSERT INTO Transactions(transaction_type, amount, other_details) VALUES('TRANSFER', "+ std::to_string(amount) +", '"+ other_details +"')";
    // If transaction type is D, means transcation_type column is DEPOSIT
    else if (type == 'D')
        query = "INSERT INTO Transactions(transaction_type, amount, other_details) VALUES('DEPOSIT', "+ std::to_string(amount) +", '"+ other_details +"')";
    // If transaction type is R, means transcation_type column is RECEIVE
    else if (type == 'R')
        query = "INSERT INTO Transactions(transaction_type, amount, other_details) VALUES('RECEIVE', "+ std::to_string(amount) +", '"+ other_details +"')";
    
    int exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tError Update: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    // ALSO UPDATE TransactionLog table which connect Customers, BankAccount, and Transactions Table //
    // First, get last insert id from Transactions table
    int transaction_id = get_last_insert_id();
    // Get customer id we want to update
    int customer_id = get_UserID_By_BankAccount(acc_num);
    // Get bank account id of customer we want to update
    int account_id = get_BankID(acc_num);

    if (transaction_id == -1 || customer_id == -1 || account_id == -1)
    {
        std::cerr << "\tFailed to insert into TransactionLog Table" << std::endl;
        sleep(4);
        return false;
    }

    query = "INSERT INTO TransactionLog(transaction_id, customer_id, account_id) VALUES("+ std::to_string(transaction_id) +", "+ std::to_string(customer_id) +", "+ std::to_string(account_id) +")";
    exit = sqlite3_exec(DB, query.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK)
    {
        std::cerr << "\tError Update: " << messageError << std::endl;
        sleep(5);
        sqlite3_free(messageError);
        return false;
    }

    return true;
}