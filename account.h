#ifndef account_h
#define account_h

#include <iostream>

#define CodeNumber "+62"
#define ZipCode "777911"

extern bool isExit;

enum PRESS 
{ 
    // ASCII CODE FOR BACKSPACE AND ENTER
    IN_BACK = 127,
    IN_RET = 13
};

struct UserAccount
{
    std::string m_FirstName;
    std::string m_LastName;
    std::string m_PlaceOfBirth;
    unsigned int m_Day;
    unsigned int m_Month;
    unsigned int m_Year;
    std::string m_Gender;
    std::string m_Address;
    std::string m_PhoneNumber;
    unsigned long m_BankAccount;
    float m_Balance;
    std::string m_City;
    std::string m_Email;
    std::string m_Username;
    std::string m_Password;
};

// Class of User Account
class Person
{
private:
    UserAccount* m_Person;
public:
    // Defalut Constructor
    Person()
        : m_Person(new UserAccount())
    {
    }

    // Destructor
    ~Person()
    {
        delete m_Person;
    }

    void Exit();
    bool Login();
    void Register();
    void Check_Balance();
    void Deposit();
    void Withdraw();
    void Transfer();
    void Show_History();
    void AccountInformation();
    void OpenAccount();
    void Loading();
    bool Load_To_Database();
    bool Load_From_Database();
    bool Validate_Username_and_Password(const std::string& username, const std::string& password);
    bool Is_Number(const std::string& s);
    unsigned int Generate_Account_Number();
    void Take_DoB_From_User();
    std::string Take_Password_From_User();
    std::string Format_User_Phone_Number(std::string& phone);
};

#endif