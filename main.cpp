#include <iostream>
#include <limits>

#include "database.h"
#include "account.h"

// FUNCTION TO DRAW LOGIN AND REGISTER MENU //
int Draw_Login_Menu()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t      WELCOME TO BANK VIRTUALITY" << std::endl;
    std::cout << "\t======================================" << std::endl;

    std::cout << std::endl;

    std::cout << "\t[1] Login" << std::endl;
    std::cout << "\t[2] Register" << std::endl;
    std::cout << "\t[0] Exit" << std::endl;

    std::cout <<std::endl;
    std::cout << "\t======================================" << std::endl;

    int choice;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    // Remove new line or remain character from std::cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    return choice;
}

int Draw_Main_Menu()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t              BANK VIRTUAL SYSTEM" << std::endl;
    std::cout << "\t================================================" << std::endl;
    std::cout << "\t       PLEASE SELECT MENU FOR TRANSACTION" << std::endl;
    std::cout << "\t           *LEFT 0 AS INPUT TO EXIT*" << std::endl;

    std::cout << std::endl;

    std::cout << "\t[1] BALANCE" << "\t\t\t [2] DEPOSIT" << std::endl;
    std::cout << "\t[3] WITHDRAW" << "\t\t\t [4] TRANSFER" << std::endl;
    std::cout << "\t[5] HISTORY" << "\t\t\t [6] INFORMATION" << std::endl;
    std::cout << "\t[7] OPEN NEW ACCOUNT" << std::endl;

    std::cout << std::endl;
    std::cout << "\t================================================" << std::endl;

    int choice;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    // Remove new line or remain character from std::cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    return choice;

    return 0;
}

int main()
{
    // Create UserAccount object
    Person myAccount;

    // Open the database
    bool isOpen = open_db();
    if (!isOpen)
    {
        return(-1);
    }

    // Create the table by passing address of DB as arguments
    bool isCreate = create_table();
    if (!isCreate)
    {
        return(-2);
    }

    bool login_valid = false;

    // LOGIN AND REGISTER MENU //
    do
    {
        int choice = Draw_Login_Menu();
        switch (choice)
        {
        case 0:
            system("clear");
            return(-1);
            break;
        case 1:
            login_valid = myAccount.Login();
            break;
        case 2:
            myAccount.Register();
            break;
        }
    }
    while (!login_valid);
    
    // BANK MAIN MENU //
    do
    {
        int choice = Draw_Main_Menu();
        switch (choice)
        {
        case 0:
            isExit = true;
            break;
        case 1:
            myAccount.Check_Balance();
            break;
        case 2:
            myAccount.Deposit();
            break;
        case 3:
            myAccount.Withdraw();
            break;
        case 4:
            myAccount.Transfer();
            break;
        case 5:
            myAccount.Show_History();
            break;
        case 6:
            myAccount.AccountInformation();
            break;
        case 7:
            myAccount.OpenAccount();
            break;
        }

    } while (!isExit);
    
    myAccount.Exit();

    // Close the database
    close_db();

    system("clear");
    return 0;
}