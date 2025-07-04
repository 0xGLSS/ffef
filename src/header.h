#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[30];
    char country[51];
    char phone[15];
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[30];
    char password[30];
};

extern const char *RECORDS;

void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
void initMenu(struct User *u);
int menuSelect(const char *title, char *options[], int n_options);