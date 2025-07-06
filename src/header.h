#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int MAX_INPUT_LEN;
extern const char *USERS;
extern const char *RECORDS;
extern struct User u;

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

void mainMenu(struct User u);
void initMenu(struct User *u);
int menuSelect(const char *title, char *options[], int n_options);
void menuInput(const char *title, const char *fields[], char (*inputs)[MAX_INPUT_LEN + 1], int n);
void clear();
