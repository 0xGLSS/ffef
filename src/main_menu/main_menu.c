#include "../header.h"

void mainMenu(struct User u)
{
    int option;
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
        updateAcc();
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};