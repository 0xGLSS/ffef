#include "../header.h"
#include "main_menu.h"

void mainMenu(struct User u)
{
    system("clear");

    char title[] = "ATM";
    char *options[] = {
        "Create a new account", 
        "Update account information", 
        "Check accounts",
        "Check list of owned account",
        "Make Transaction",
        "Remove existing account",
        "Transfer ownership",
        "Exit",
    };

    int choice = menuSelect(title, options, 8);

    switch (choice)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAcc(u);
        break;
    case 3:
        checkAcc(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAcc(u);
        break;
    case 7:
        transferOwner(u);
        break;
    case 8:
        exit(1);
        break;
    }
    
    sleep(2);
    mainMenu(u);
};