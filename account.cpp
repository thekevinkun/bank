#include <bits/stdc++.h>
#include <iomanip>
#include <limits>
#include <unistd.h>
#include <string>

#include "database.h"
#include "encrypt.h"
#include "account.h"

unsigned int session_userID = 0;
unsigned int session_bankID = 0;
bool isExit = false;

void Person::Exit()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t======================================" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "\t   THANK YOU FOR YOUR PARTICIPATION" << std::endl;

    std::cout << std::endl << std::endl;
    std::cout << "\t======================================" << std::endl;
    
    sleep(5);
}

// Login Menu for user enter the bank system
bool Person::Login()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t        LOGIN MENU" << std::endl;
    std::cout << "\t==========================" << std::endl;

    std::cout << std::endl;

    std::cout << "\tUsername : ";
    std::getline(std::cin, m_Person->m_Username);

    std::cout << "\tPassword : ";
    m_Person->m_Password = Take_Password_From_User();

    // Validate username and password
    bool isValid = Validate_Username_and_Password(m_Person->m_Username, m_Person->m_Password);
    if (!isValid)
    {
        return false;
    }

    // Get id of loggin customer
    session_userID = get_id(m_Person->m_Username);
    if (session_userID == -1)
        return false;
    
    // Get all user bank account to be choosed //
    // TO get current use AccountNumber ID for future operation // 
    std::vector<unsigned long> Account = Get_All_Bank_Account(m_Person->m_Username);
    unsigned long acc = 0;
    int choice = 0;

    // If there's only one bank account, no need to ask, just use it
    if (Account.size() == 1)
    {
        acc = Account.front();
        session_bankID = get_BankID(acc);
        if (session_bankID <= 0)
            return false;
    }
    // If there's more than one account, show at prompt to picked
    else
    {
        printf("\033[2A\033[J");
        std::cout << "\tCHOOSE YOUR ACCOUNT" << std::endl;
        
        for (size_t i = 0; i < Account.size(); i++)
        {
            std::cout << "\t" << i+1 << "." << Account[i] << std::endl;
        }

        std::cout << std::endl;
        std::cout << "\tEnter: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');  

        // Get the ID of active account
        acc = Account[choice - 1];
        session_bankID = get_BankID(acc);
        if (session_bankID == -1)
            return false;
    }

    // After login, load all user information in database to Struct, use ID of active account as condition //
    bool isLoad = Load_From_Database();
    if (!isLoad)
        return false;

    return isValid;
}

// Register //
void Person::Register()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t             REGISTER MENU" << std::endl;
    std::cout << "\t=======================================" << std::endl;

    std::cout << std::endl << std::endl;
    
    // Ask for first name
    std::cout << "\tFirst Name : ";
    std::getline(std::cin, m_Person->m_FirstName);

    printf("\033[A\033[2K");

    // Ask for last name
    std::cout << "\tLast Name : ";
    std::getline(std::cin, m_Person->m_LastName);

    printf("\033[A\033[2K");

    std::cout << "\tPlace Of Birth: ";
    std::getline(std::cin, m_Person->m_PlaceOfBirth);

    // Ask for user birthday, calling function
    Take_DoB_From_User();

    printf("\033[A\033[2K");

    // Ask for user gender
    std::cout << "\tGender(Male/Female) : ";
    std::getline(std::cin, m_Person->m_Gender);

    printf("\033[A\033[2K");

    // Ask for user address
    std::cout << "\tHome Address : ";
    std::getline(std::cin, m_Person->m_Address);

    // Ask for user phone number
    do
    {
        printf("\033[A\033[2K");
        std::cout << "\tHandphone : ";
        std::getline(std::cin, m_Person->m_PhoneNumber);

        if (!Is_Number(m_Person->m_PhoneNumber))
        {
            std::cerr << "\tInvalid Phone Number!";
            sleep(3);
        }
    }
    while (!Is_Number(m_Person->m_PhoneNumber));
    
    // Remove 0 at front of phone number
    if (m_Person->m_PhoneNumber.front() == '0')
        m_Person->m_PhoneNumber.erase(0, 1);
    
    // FOR BANK ACCOUNT INFORMATION //
    printf("\033[2A\033[J");
    std::cout << "\t(For Bank Account)" << std::endl << std::endl;
    
    // Ask where city bank is made
    std::cout << "\tCity: ";
    std::getline(std::cin, m_Person->m_City);

    // Ask for email, keep asking til it's valid
    // Ask for user username and password, keep asking til it's valid
    do
    {
        printf("\033[A\033[2K");

        std::cout << "\tEmail : ";
        std::getline(std::cin, m_Person->m_Email);
        // Convert all email to lowercase
        transform(m_Person->m_Email.begin(), m_Person->m_Email.end(), m_Person->m_Email.begin(), ::tolower);

        if (!check_email(m_Person->m_Email))
        {
            printf("\033[A\033[2K");
            std::cerr << "\tEmail is already been used!" << std::endl;
            sleep(3);
        }
    
    } 
    while (!check_email(m_Person->m_Email));


    // Ask for user username, keep asking til it's valid
    do
    {
        printf("\033[A\033[2K");

        std::cout << "\tUsername : ";
        std::getline(std::cin, m_Person->m_Username);
        // Convert all username to lowercase
        transform(m_Person->m_Username.begin(), m_Person->m_Username.end(), m_Person->m_Username.begin(), ::tolower);

        if (!check_username(m_Person->m_Username))
        {
            printf("\033[A\033[2K");
            std::cerr << "\tUsername is not available!" << std::endl;
            sleep(3);
        }
    
    } 
    while (!check_username(m_Person->m_Username));

    printf("\033[A\033[2K");

    // Handle user password
    bool PasswordValid = false;
    do
    {
        // Ask for password
        std::cout << "\tPassword : ";
        m_Person->m_Password = Take_Password_From_User();
        // Confirm user password
        std::cout << "\tConfirm  : ";
        std::string confirm_password = Take_Password_From_User();

        // If didn't match, ask for password again
        if (confirm_password != m_Person->m_Password)
        {
            std::cout << "\n\tThose passwords didn’t match. Try again." << std::endl;
            sleep(4);
            printf("\033[4A\033[J");
        }
        else
            PasswordValid = true;

    } while (!PasswordValid);

    printf("\033[4A\033[J");

    // Give user 1000000 as default balance
    m_Person->m_Balance = 1000000;
    // Generate Account number
    m_Person->m_BankAccount = Generate_Account_Number();

    // Load information to database
    bool isLoad = Load_To_Database();
    if (!isLoad)
    {
        std::cout << "\tFailed to register. Try again later." << std::endl;
        sleep(20);
        return;
    }

    std::cout << std::endl;
    Loading();
    std::cout << "\t              Registered!" << std::endl;
    std::cout << "\t   Please login to access your account." << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "\t=======================================" << std::endl;
    sleep(5);
}

// CHECK BALANCE MENU //
void Person::Check_Balance()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t       CHECKING BALANCE" << std::endl;
    std::cout << "\t==============================" << std::endl;

    std::cout << std::endl;
    std::cout << "\t     YOUR CURRENT BALANCE" << std::endl;
    std::cout << std::endl;

    // Always get new update balance of customer in database
    // Because after every transactions, customer balance will be updated 
    float current_balance = get_balance(session_bankID);

    std::cout << std::fixed << "\t\t  " << std::setprecision(2) << current_balance << std::endl << std::endl;
    std::cout << std::endl;

    std::cout << "\t   NEED ANOTHER TRANSACTION?" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t       [2] No" << std::endl;

    std::cout << "\t==============================" << std::endl;
    int choice;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

    // Change exit to true to end the while loop in main()
    if (choice == 2)
        isExit = true;
}

void Person::Deposit()
{
    float amount = 0;
    int choice = 0;
    float current_balance = get_balance(session_bankID);

    system("clear");
    std::cout << std::endl;
    std::cout << "\t\t\t DEPOSIT MENU" << std::endl;
    std::cout << "\t==============================================" << std::endl;
    std::cout << "\t   *LEFT 0 AS INPUT TO CANCEL TRANSACTION*" << std::endl;

    std::cout << std::endl << std::endl;

    std::cout << "\t\t       ENTER THE AMOUNT:" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t\t    "; std::cin >> amount;
    if (amount == 0)
        return;

    printf("\033[6A\033[J\r");

    // SHOW THE CONFIRMATION BEFORE GET TRANSFERRED //
    std::cout << std::endl;
    std::cout << "\t\t     CONFIRM YOUR ACCOUNT" << std::endl << std::endl;
    std::cout << "\tName    : " << m_Person->m_FirstName + " " + m_Person->m_LastName <<std::endl;
    std::cout << "\tAccount : " << m_Person->m_BankAccount << std::endl;
    std::cout << std::fixed << "\tAmount  : " << std::setprecision(2) << amount << std::endl;

    std::cout << std::endl;
    std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t\t\t       [2] No" << std::endl;

    std::cout << "\t==============================================" << std::endl;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 

    // IF User choose No, we simply exit program
    if (choice == 2)
    {
        isExit = true;
        return;
    }

    // UPDATE THE BALANCE OF DEPOSITOR, PASS ACTIVE ACCOUNT NUMBER AS ARGUMENT //
    if (!Update_Balance_of_Customer(m_Person->m_BankAccount, amount, 'D'))
    {
        std::cerr << "\tFailed to update customer balance!" << std::endl;
        sleep(5);
        return;
    }

    std::string other_details = "(none)";
    if (!Add_Transfer_History_of_Customer(m_Person->m_BankAccount, amount, 'D', other_details))
    {
        std::cerr << "\tFailed to update customer history" << std::endl;
        sleep(5);
        return;
    }

    printf("\033[10A\033[J\r");
    std::cout << std::endl << std::endl;
    Loading();
    std::cout << "\t\t\tDEPOSIT SUCCESS" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "\t\t   NEED ANOTHER TRANSACTION?" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t\t\t       [2] No" << std::endl;

    std::cout << "\t==============================================" << std::endl;

    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

    // Change exit to true to end the while loop in main()
    if (choice == 2)
        isExit = true;
    // Continue transaction
    else if (choice == 1)
        isExit = false;
}

// WITHDRAWING MONEY MENU //
void Person::Withdraw()
{
    int choice = 0;
    float amount = 0;
    float maximum_withdraw = 1250000;
    float current_balance = get_balance(session_bankID);
    
    do
    {
        system("clear");
        std::cout << std::endl;
        std::cout << "\t\t\tWITHDRAWAL MENU" << std::endl;
        std::cout << "\t===============================================" << std::endl;
        std::cout << "\t      PLEASE SELECT THE WITHDRAWAL AMOUNT" << std::endl; 
        std::cout << "\t    *LEFT 0 AS INPUT TO CANCEL TRANSACTION*" << std::endl;

        std::cout << std::endl;

        std::cout << "\t[1] 100.000" << "\t\t\t  [2] 250.000" << std::endl;
        std::cout << "\t[3] 500.000" << "\t\t\t  [4] 750.000" << std::endl;
        std::cout << "\t[5] 1.000.000" << "\t\t\t  [6] 1.250.000" << std::endl;
        std::cout << "\t[7] WITHDRAW OTHER AMOUNT" << std::endl;

        std::cout << std::endl;
        std::cout << "\t===============================================" << std::endl; 
        
        std::cout << "\tEnter: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        switch (choice)
        {
        case 0:
            return;
        case 1:
            amount = 100000;   
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;
        case 2:
            amount = 250000;
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;
        case 3:
            amount = 500000;
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;
        case 4:
            amount = 750000;
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;
        case 5:
            amount = 1000000;
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;
        case 6:
            amount = 1250000;
            if (amount > current_balance)
            {
                printf("\033[10A\033[J");
                std::cout << std::endl << std::endl;
                std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                std::cout << std::endl << std::endl;
                std::cout << "\t===============================================" << std::endl;
                sleep(5);

                continue;
            }
            printf("\033[3A\033[J");
            isExit = true;
            break;    
        case 7:    
            printf("\033[10A\033[J\r");
            std::cout << "\t    *LEFT 0 AS INPUT TO CANCEL TRANSACTION*" << std::endl;
            std::cout << std::endl;
            std::cout << "\t       (MAXIMUM TO WITHDRAW IS 1.250.000)" << std::endl;
            std::cout << std::endl;
            std::cout << "\t\t       ENTER THE AMOUNT:" << std::endl;
            std::cout << std::endl;
            std::cout << "\t\t\t    "; std::cin >> amount;
            if (amount == 0)
                return;
            // Check if customer balance is not enough, show another menu option
            else if (amount > current_balance || amount > maximum_withdraw)
            {
                if (amount > maximum_withdraw)
                {
                    printf("\033[3A\033[J");
                    std::cout << std::endl;
                    std::cout << "\t\t   EXCEEDED THE MAXIMUM LIMIT" << std::endl;
                    std::cout << std::endl << std::endl;
                    std::cout << "\t===============================================" << std::endl;
                    sleep(5);
                    continue;
                }
                else if (amount > current_balance)
                {
                    printf("\033[3A\033[J");
                    std::cout << std::endl;
                    std::cout << "\t\t   YOUR BALANCE IS NOT ENOUGH" << std::endl;
                    std::cout << std::endl << std::endl;
                    std::cout << "\t===============================================" << std::endl;
                    sleep(5);
                    continue;
                }
            }
            else
                isExit = true;    
            // CASE 5: END OF SWITCH
            break;
        }
    }
    while (!isExit);

    // UPDATE THE BALANCE OF WITHDRAWAL //
    if (!Update_Balance_of_Customer(m_Person->m_BankAccount, amount, 'W'))
    {
        std::cerr << "\tFailed to update customer balance" << std::endl;
        sleep(5);
        return;
    }
    
    std::string other_details = "(none)";
    if (!Add_Transfer_History_of_Customer(m_Person->m_BankAccount, amount, 'W', other_details))
    {
        std::cerr << "\tFailed to update customer history" << std::endl;
        sleep(5);
        return;
    }

    printf("\033[7A\033[J\r");
    std::cout << std::endl << std::endl;
    Loading();
    std::cout << std::fixed << "\t     " << std::setprecision(2) << amount;
    std::cout << " WITHDRAWN FROM YOUR ACCOUNT" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "\t\t   NEED ANOTHER TRANSACTION?" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t\t\t       [2] No" << std::endl;

    std::cout << "\t===============================================" << std::endl;

    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

    // Change exit to true to end the while loop in main()
    if (choice == 2)
        isExit = true;
    // Continue transaction
    else if (choice == 1)
        isExit = false;
}

// TRANSFER MONEY MENU //
void Person::Transfer()
{
    std::string receiver_name = "";
    float amount = 0;
    int choice = 0;

    float current_balance = get_balance(session_bankID);
    unsigned long transfer_to = 0;

    system("clear");
    std::cout << std::endl;
    std::cout << "\t\t\t TRANSFER MENU" << std::endl;
    std::cout << "\t==============================================" << std::endl;

    do
    {
        std::cout << "\t    *LEFT 0 AS INPUT TO CANCEL TRANSACTION*" <<std::endl;
        std::cout << std::endl << std::endl;
        std::cout << "\t\t TRANSFER TO(ACCOUNT NUMBER) :" << std::endl;
        std::cout << std::endl;
        std::cout << "\t\t\t   "; std::cin >> transfer_to;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    
        // IF USER PUT 0, MEANS EXIT
        if (transfer_to == 0)
            return;

        // IF ACCOUNT NUMBER FALSE, SHOW ANOTHER OPTION IN MENU //
        if (!check_account_number(transfer_to))
        {
            printf("\033[5A\033[J");
            std::cout << std::endl << std::endl;
            std::cout << "\t       ACCOUNT NUMBER IS NOT REGISTERED!" << std::endl;
            sleep(2);

            std::cout << std::endl << std::endl;
            std::cout << "\t\t    CONTINUE TO TRANSFER?" << std::endl;
            std::cout << std::endl;
            std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
            std::cout << "\t\t\t\t\t       [2] No" << std::endl;

            std::cout << "\t==============================================" << std::endl;
            
            std::cout << "\tEnter: ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

            // IF User choose No, we simply go to main menu
            if (choice == 2)
                return;
            // If user choose Yes, go to beginning of menu, ask account number to be transferred
            else if (choice == 1)
            {
                printf("\033[12A\033[J\r");
                continue;
            }
        }

        printf("\033[4A\033[J\r");

        // CONTINUE TO ASK AFTER VALIDATIN ACCOUNT NUMBER //
        // ASK AMOUNT OF TRANSFER //
        std::cout << std::endl;
        std::cout << "\t\t      AMOUNT TO TRANSFER :" << std::endl;
        std::cout << std::endl;
        std::cout << "\t\t\t    "; std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 
        // IF USER put 0, means exit
        if (amount == 0)
            return;

        // Check if customer balance is not enough, show another menu option
        if (amount > current_balance)
        {
            printf("\033[3A\033[J");
            std::cout << "\t\t  YOUR BALANCE IS NOT ENOUGH!" << std::endl;
            sleep(3);

            std::cout << std::endl << std::endl;
            std::cout << "\t\t    CONTINUE TO TRANSFER?" << std::endl;
            std::cout << std::endl;
            std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
            std::cout << "\t\t\t\t\t       [2] No" << std::endl;

            std::cout << "\t==============================================" << std::endl;
    
            std::cout << "\tEnter: ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

            // IF User choose No, we simply exit program, end while loop in main()
            // IF User choose No, we simply exit function, go to very beginning menu
            if (choice == 2)
                return;
            // If user choose Yes, go to beginning of menu
            else if (choice == 1)
            {
                printf("\033[12A\033[J\r");
                continue;
            }
        }
        else
            isExit = true;
        
    }
    while (!isExit);

    printf("\033[6A\033[J\r");

    // GET RECEIVER NAME AFTER VALIDATING THEY ACCOUNT_NUMBER
    receiver_name = get_name(transfer_to);

    // SHOW THE CONFIRMATION BEFORE GET TRANSFERRED //
    std::cout << std::endl;
    std::cout << "\t\t     TRANSFER CONFIRMATION" << std::endl << std::endl;
    std::cout << "\tName    : " << receiver_name <<std::endl;
    std::cout << "\tAccount : " << transfer_to << std::endl;
    std::cout << std::fixed << "\tAmount  : " << std::setprecision(2) << amount << std::endl;

    std::cout << std::endl;
    std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t\t\t       [2] No" << std::endl;

    std::cout << "\t==============================================" << std::endl;

    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 

    // IF User choose No, we simply exit program
    if (choice == 2)
    {
        isExit = true;
        return;
    }

    // UPDATE THE BALANCE OF TRANSFERRER AND RECEIVER, PASS ACCOUNT NUMBER AS ARGUMENT //
    if (!Update_Balance_of_Customer(m_Person->m_BankAccount, amount, 'T') || !Update_Balance_of_Customer(transfer_to, amount, 'R'))
    {
        std::cerr << "\tFailed to update customer balance!" << std::endl;
        sleep(5);
        return;
    }

    std::string transferrer_name = m_Person->m_FirstName + " " + m_Person->m_LastName;

    // Wrote other details of transactions
    std::string transfer_details = "to "+ receiver_name +" "+ std::to_string(transfer_to) +"";
    std::string receive_details = "from "+ transferrer_name +" "+ std::to_string(m_Person->m_BankAccount) +"";

    // UPDATE TRANSFER HISTORY OF TRANSFERRER AND RECEIVER //
    // FOR TRANSFER, WE PASS TRANSFERRER ACCOUNT NUMBER || FOR RECEIVE, WE PASS RECEIVER ACCOUNT NUMBER //
    if (!Add_Transfer_History_of_Customer(m_Person->m_BankAccount, amount, 'T', transfer_details) || !Add_Transfer_History_of_Customer(transfer_to, amount, 'R', receive_details))
    {
        std::cerr << "\tFailed to update customer history!" << std::endl;
        sleep(5);
        return;
    }

    printf("\033[10A\033[J");
    std::cout << std::endl;
    Loading();
    std::cout << "\t\t      TRANSFER SUCCESSFUL" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "\t\t   NEED ANOTHER TRANSACTION?" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t\t\t\t       [1] Yes" << std::endl;
    std::cout << "\t\t\t\t\t       [2] No" << std::endl;

    std::cout << "\t==============================================" << std::endl;

    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');    

    // Change exit to true to end the while loop in main()
    if (choice == 2)
        isExit = true;
    // Continue transaction
    else if (choice == 1)
        isExit = false;

}

// HISTORY OF CUSTOMER TRANSACTION //
void Person::Show_History()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t\t\t\t\t\t  TRANSACTION HISTORY" << std::endl;
    std::cout << "\t==================================================================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "\tNAME    : " << m_Person->m_FirstName + " " + m_Person->m_LastName << std::endl;
    std::cout << "\tACCOUNT : " << m_Person->m_BankAccount << std::endl;
    std::cout << std::endl;

    // GET TRANSACTION HISTORY OF CUSTOMER'S ACTIVE BANK ACCOUNT IN DATABASE //
    std::string transaction_type = "";
    std::string description = "";
    std::string timestamp = "";
    float amount = 0;

    std::string query = "SELECT transaction_type, amount, other_details, timestamp FROM Transactions WHERE id IN (SELECT transaction_id FROM TransactionLog WHERE account_id = "+ std::to_string(session_bankID) +")";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return;
    }

    rc = sqlite3_step(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // Store all query into variable
        transaction_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        amount = static_cast<float>(sqlite3_column_int(stmt, 1));
        description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        // Display to user
        std::cout << "\t" << timestamp << "\t" << "  " <<  transaction_type << "  ";
        std::cout << std::fixed << "\t" << " " << std::setprecision(2) << amount;
        std::cout << "\t" << "  " << description << std::endl;
    }
    if (rc != SQLITE_DONE)
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
    

    int choice = 0;
    std::cout << std::endl << std::endl;
    std::cout << "\t\t\t\t\t\t\t\t\t\t     [1] BACK TO MAIN MENU" << std::endl;
    std::cout << "\t\t\t\t\t\t\t\t\t\t     [2] EXIT" << std::endl;
    std::cout << "\t==================================================================================================" << std::endl;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 

    // IF User choose No, we simply exit program
    if (choice == 2)
        isExit = true;

    sqlite3_finalize(stmt);
}

// ACCOUNT INFORMATION MENU //
void Person::AccountInformation()
{
    system("clear");
    std::cout << std::endl;
    std::cout << "\t\t\t\t ACCOUNT INFORMATION" << std::endl;
    std::cout << "\t=================================================================" << std::endl;

    int choice = 0;
    std::string FullName = m_Person->m_FirstName + " " + m_Person->m_LastName;

    std::cout << std::endl << std::endl;
    std::cout << "\tNAME                : " << FullName << std::endl;
    std::cout << "\tACCOUNT             : " << m_Person->m_BankAccount << std::endl;
    std::cout << "\tPLACE/DATE OF BIRTH : " << m_Person->m_PlaceOfBirth << ", " << m_Person->m_Day << "-" << m_Person->m_Month << "-" << m_Person->m_Year << std::endl;
    std::cout << "\tGENDER              : " << m_Person->m_Gender << std::endl;
    std::cout << "\tADDRESS             : " << m_Person->m_Address << std::endl;
    std::cout << "\tCITY                : " << m_Person->m_City << std::endl;
    std::cout << "\tZIP CODE            : " << ZipCode << std::endl;
    std::cout << "\tHANDPHONE           : " << m_Person->m_PhoneNumber << std::endl;
    std::cout << "\tUSERNAME            : " << m_Person->m_Username << std::endl;
    std::cout << "\tEMAIL               : " << m_Person->m_Email << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "\t\t\t\t\t\t    [1] BACK TO MAIN MENU" << std::endl;
    std::cout << "\t\t\t\t\t\t    [2] EXIT" << std::endl;
    std::cout << "\t=================================================================" << std::endl;
    std::cout << "\tEnter: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 

    // IF User choose No, we simply exit program
    if (choice == 2)
        isExit = true;
}

void Person::OpenAccount()
{
    return;
}

// LOAD FROM DATABASE TO MEMBER OF STRUCT AFTER USER LOGIN //
bool Person::Load_From_Database()
{
    // Query with ID of User active bank account as condition
    std::string query = "SELECT first_name, last_name, birth_place, birth_date, gender, address, handphone, account_number, balance, city, email FROM Customers JOIN CustomerAccount ON Customers.id = CustomerAccount.customer_id JOIN BankAccount ON CustomerAccount.account_id = BankAccount.id JOIN LoginAccount ON BankAccount.id = LoginAccount.account_id JOIN LoginProfile ON LoginAccount.account_id = LoginProfile.id JOIN Citizen ON Customers.id = Citizen.customer_id WHERE BankAccount.id = "+ std::to_string(session_bankID) +";"; 

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return false;
    }

    rc = sqlite3_step(stmt);

    m_Person->m_FirstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    m_Person->m_LastName =reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    m_Person->m_PlaceOfBirth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    
    // Handle birth date string from database to int member class
    std::string BirthDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    m_Person->m_Year = stoi(BirthDate.substr(0, BirthDate.find_first_of("-")));
    m_Person->m_Month = stoi(BirthDate.substr(BirthDate.find_first_of("-") + 1, BirthDate.find_last_of("-")));
    m_Person->m_Day = stoi(BirthDate.substr(BirthDate.find_last_of("-") + 1, BirthDate.length()));

    m_Person->m_Gender = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    m_Person->m_Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    // Store user phone number in Formatted
    m_Person->m_PhoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    m_Person->m_PhoneNumber = Format_User_Phone_Number(m_Person->m_PhoneNumber);

    m_Person->m_BankAccount = sqlite3_column_int(stmt, 7);
    m_Person->m_Balance = sqlite3_column_int(stmt, 8);
    m_Person->m_City = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
    m_Person->m_Email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));

    sqlite3_finalize(stmt);

    return true;
}

// INSERT CUSTOMER INFORMATION AFTER REGISTER TO DATABASE //
bool Person::Load_To_Database()
{
    int exit = 0;
    bool isLoad;
    std::string query;

    // FOR EVERY QUERY WE PASSED AS ARGUMENTS FOR load() in database.cpp //

    // INSERT Personal Data TO Customers Table //
    query = "INSERT INTO Customers(first_name, last_name, birth_place, birth_date, gender, address, handphone) VALUES('"+ m_Person->m_FirstName +"', '"+ m_Person->m_LastName +"', '"+ m_Person->m_PlaceOfBirth +"', '"+ std::to_string(m_Person->m_Year) +"-"+ std::to_string(m_Person->m_Month) +"-"+ std::to_string(m_Person->m_Day) +"', '"+ m_Person->m_Gender +"', '"+ m_Person->m_Address +"', '"+ m_Person->m_PhoneNumber +"')";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into Customers Table" << std::endl;
        sleep(4);
        return false;
    }

    // WE GET LAST INSERTED ID OF Customers Table //
    int customer_id = get_last_insert_id();

    // INSERT BANK INFORMATION TO BankAccount Table //
    query = "INSERT INTO BankAccount(account_number, balance) VALUES("+ std::to_string(m_Person->m_BankAccount) +", "+ std::to_string(m_Person->m_Balance) +")";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into BankAccount Table" << std::endl;
        sleep(4);
        return false;
    }

    // WE GET LAST INSERTED ID OF BankAccount Table //
    int account_id = get_last_insert_id();

    // INSERT id of Customers and BankAccount TO CustomerAccount Table //
    // We use last inserted id of Customers and BankAccount to be insert in the table, link the relationship between two table //
    query = "INSERT INTO CustomerAccount(customer_id, account_id) VALUES("+ std::to_string(customer_id) +", "+ std::to_string(account_id) +")";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into CustomerAccount Table" << std::endl;
        sleep(4);
        return false;
    }


    // INSERT UserLogin Information to LoginProfile Table //
    // Hash the password before store it in database
    m_Person->m_Password = hash_password(m_Person->m_Password);
    query = "INSERT INTO LoginProfile(email, username, password) VALUES('"+ m_Person->m_Email +"', '"+ m_Person->m_Username +"', '"+ m_Person->m_Password +"')";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into LoginProfile Table" << std::endl;
        sleep(4);
        return false;
    }

    // WE GET LAST INSERTED IF OF LoginProfile Table //
    int login_id = get_last_insert_id();

    // INSERT id of BankAccount and LoginProfile TO LoginAccount Table //
    // We use last inserted id of BankAccount and LoginProfile to be insert in the table, link the relationship between two table //
    query = "INSERT INTO LoginAccount(account_id, login_id) VALUES("+ std::to_string(account_id) +", "+ std::to_string(login_id) +")";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into LoginAccount Table" << std::endl;
        sleep(4);
        return false;
    }

    // INSERT CITIZEN INFORMATION to Citizen Table //
    // Use last inserted ID of Customers table as Foreign key //
    query = "INSERT INTO Citizen(customer_id, city, zip_code, country_code) VALUES("+ std::to_string(customer_id) +", '"+ m_Person->m_City +"', '"+ ZipCode +"', '"+ CodeNumber +"')";
    isLoad = load(query);
    if (!isLoad)
    {
        std::cout << "\tFailed to insert into Citizen Table" << std::endl;
        sleep(4);
        return false;
    }

    return true;
}

// Generate Account Number of user //
unsigned int Person::Generate_Account_Number()
{
    // 123 : Unique number is a identification number of bank
    // 0101 : Four digit number is also to identified the bank
    // Last three digit after that, is belong to customer. It will increment for every new customer
    // // Ex: 123-0101-001 first customer -- 123-0101-002 next new customer

    unsigned int result = 0;

    // GENERATE NEW ACCOUNT NUMBER BY EVALUATE DATABASE //
    std::string query = "SELECT account_number FROM BankAccount GROUP BY(id) HAVING COUNT(id) > 0";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(5);
        return -1;
    }
    
    // Iterate through all account number until last row
    int ncols = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        // Store every account number, this will stop until last row or last account number
        for (int i = 0; i < ncols; i++)
            result = sqlite3_column_int(stmt, i);
    }
    if (rc != SQLITE_DONE)
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;

    // CHECK if there's no customer in databese yet
    // Then we will give very first account number = 123-0101-001
    if (result == 0)
        result = 1230101001;
    // Increment last account number that we get, so it'll be new account number with new digit
    else
        result++;
        
    sqlite3_finalize(stmt);
    return result;    
}

bool Person::Validate_Username_and_Password(const std::string& username, const std::string& password)
{
    // Validate username and password within database
    // Get the password by putting user input of username as condition, IF it's return NULL, means get no password which there are no username
    std::string query = "SELECT password FROM LoginProfile WHERE username = '"+ username +"'";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "\tError: " << sqlite3_errmsg(DB) << std::endl;
        sleep(4);
        return false;
    }

    // Get the encrypted code to compare it within database of username
    // hash_password() is in database.cpp 
    std::string input_password = hash_password(password);

    // For returning condition
    bool isValid = true;

    // Move to column's table
    rc = sqlite3_step(stmt);

    // Get the value from query
    const unsigned char* get_password = sqlite3_column_text(stmt, 0);
    
    // If return null means there are no username yet
    if (get_password == nullptr)
    {
        std::cerr << std::endl << "\tThe username you entered doesn't belong to an account.\n\t     Please check your username and try again." << std::endl;
        sleep(5);
        isValid = false;
    }
    // If username valid, check if the password match between input and database
    else if (strcmp(&input_password[0], reinterpret_cast<const char*>(get_password)) != 0)
    {
        std::cerr << std::endl << "\tSorry, your password was incorrect.\n\tPlease double-check your password." << std::endl;
        sleep(5);
        isValid = false;
    }

    sqlite3_finalize(stmt);
 
    return isValid;
}

// FUNCTION TO HANDLE PASSWORD TYPING IN CONSOLE //
std::string Person::Take_Password_From_User()
{
    // Stores the password
    std::string passwd = "";
    char ch_ipt;

    system("stty raw");
    // Until condition is true
    while (true) 
    {
        // Use getchar() so we can keep typing the character in console before it's done
        ch_ipt = getchar();
     
        // If user press enter, return
        if (ch_ipt == PRESS::IN_RET) 
        {
            std::cout << "\b \b";
            std::cout << "\b \b";
        
            system("stty cooked");
            std::cout << std::endl;
            return passwd;
        }
        // If user press backspace and there's a character, pop it
        else if (ch_ipt == PRESS::IN_BACK && passwd.length() != 0) 
        {
            passwd.pop_back();
 
            // The first two will remove character produce by backspace
            std::cout << "\b \b";
            std::cout << "\b \b";
            // Then, remove previos printed character or *
            std::cout << "\b \b";
            continue;
        }
        // If user press backspace and there's no character yet, just continue to ask for password
        // Without using this, program will crash as \b can't be print in beginning of line
        else if (ch_ipt == PRESS::IN_BACK && passwd.length() == 0) 
        {
            // Remove character produce by backspace before continue to get input
            std::cout << "\b \b";
            std::cout << "\b \b";
            continue;
        }
 
        // Store the character of user input
        passwd.push_back(ch_ipt);
        // First we remove previously printed character in console, then Print the * pretend to hide it
        std::cout << "\b";
        putchar('*');
    }
}

// FORMAT USER PHONE NUMBER // 
std::string Person::Format_User_Phone_Number(std::string& phone)
{
    std::string code = CodeNumber;

    phone.insert(3, "-");
    phone = '(' + code + ')' + " " + phone;

    return phone;
}

// FUNCTION TO HANDLE VALID DATE INPUT //
void Person::Take_DoB_From_User()
{
    std::string temp;
    bool isValid = true;

    do
    {
        printf("\033[A\033[2K");

        std::cout << "\tDate of Birth(mm/dd/yyyy) : ";
        std::getline(std::cin, temp);

        m_Person->m_Month = stoi(temp.substr(0, 2));
        m_Person->m_Day = stoi(temp.substr(3, 5));
        m_Person->m_Year = stoi(temp.substr(6, 10));

        // Check if month valid
        if (m_Person->m_Month <= 0 || m_Person->m_Month > 12)
        {
            std::cout << std::endl << "\tYou entered wrong month!" << std::endl;
            sleep(4);
            isValid = false;
            printf("\033[2A\033[J");
        }
        // Check for day, if it's valid
        else if (m_Person->m_Day <= 0 || m_Person->m_Day > 31)
        {
            std::cout << std::endl << "\tYou entered wrong day!" << std::endl;
            sleep(4);
            isValid = false;
            printf("\033[2A\033[J");
        }
        // Check for year if it's valid, length should be 4 and not before 1900
        else if (m_Person->m_Year < 1900 || (std::to_string(m_Person->m_Year).length() < 4 || std::to_string(m_Person->m_Year).length() > 4))
        {
            std::cout << std::endl <<"\tYou entered wrong year!" << std::endl;
            sleep(4);
            isValid = false;
            printf("\033[2A\033[J");
        }
        else
            isValid = true;

    } while (!isValid);
}

bool Person::Is_Number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    
    return !s.empty() && it == s.end();
}

void Person::Loading()
{
    printf("\t\t\t    PLEASE WAIT");
    for (int i = 0; i < 5; i++)
    {
        printf(".");
        fflush(stdout);
        sleep(3);
    }
    printf("\n");
    printf("\033[A\033[2K");
}