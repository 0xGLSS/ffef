#include "../header.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define MAX_INPUT_LEN 100

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %s %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void success(struct User u)
{

    char title[] = "✔ Success!";
    char *options[] = {"Go to main menu", "Exit"};
    int choice = menuSelect(title, options, 2);

    while (1)
    {
        switch (choice)
        {
        case 1:
            mainMenu(u);
            break;
        case 2:
            exit(0);
            break;
        }
    }
}
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
// Print message at bottom-left of the terminal
void waitForAnyKey() {
    struct termios old, new;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new);
    getchar();
    tcsetattr(0, TCSANOW, &old);
}

void printAt(const char *text) {
    printf("\033[999;1H%s\033[K\nPress any key to continue...", text);
    fflush(stdout);
    waitForAnyKey();
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    char inputs[7][MAX_INPUT_LEN + 1];
    const char *fields[] = {
        "Today's date (mm/dd/yyyy)",
        "Account number",
        "Country",
        "Phone number",
        "Amount to deposit",
        "Type of account (saving/current/fixed01/fixed02/fixed03)"};

    FILE *pf;
    int exists;

    while (1)
    {
        system("clear");
        printf("\t\t\t===== New record =====\n");

        menuInput("New Account Creation", fields, inputs, 6, 2);

        // Parse inputs
        if (sscanf(inputs[0], "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3)
        {
            printAt("Invalid date format!\n");
            continue;
        }
        if (sscanf(inputs[1], "%d", &r.accountNbr) != 1 || !validateInput(r, 2))
        {
            printAt("Invalid account number!\n");
            continue;
        }
        strncpy(r.country, inputs[2], sizeof(r.country) - 1);
        r.country[sizeof(r.country) - 1] = '\0';

        if (!validateInput(r, 3))
        {
            printAt("Invalid country name!\n");
            continue;
        }

        if (sscanf(inputs[3], "%s", r.phone) != 1 || !validateInput(r, 4))
        {
            printAt("Invalid phone number!\n");
            continue;
        }

        if (sscanf(inputs[4], "%lf", &r.amount) != 1 || !validateInput(r, 5))
        {
            printAt("Invalid amount!\n");
            continue;
        }
        strncpy(r.accountType, inputs[5], sizeof(r.accountType) - 1);
        r.accountType[sizeof(r.accountType) - 1] = '\0';

        if (!validateInput(r, 6))
        {
            printAt("Invalid account type!\n");
            continue;
        }

        // Check if account exists for this user
        pf = fopen(RECORDS, "r");
        if (!pf)
        {
            printAt("Error opening records file.\n");
            return;
        }

        // check this
        exists = 0;
        while (getAccountFromFile(pf, userName, &cr))
        {
            if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
            {
                exists = 1;
                break;
            }
        }
        fclose(pf);

        if (exists)
        {
            printAt("✖ This Account already exists for this user\n");
            printAt("Press Enter to try again...");
            getchar();
            continue; // retry loop
        }

        // Save new record
        pf = fopen(RECORDS, "a");
        if (!pf)
        {
            printAt("Error opening records file for writing.\n");
            return;
        }

        r.id = getNextRecordId(RECORDS);

        saveAccountToFile(pf, u, r);
        fclose(pf);

        success(u);
        break;
    }
}
int getNextRecordId()
{
    FILE *fp = fopen(RECORDS, "r");
    if (!fp)
    {
        perror("Error opening file");
        return 1; // Start from ID 1 if file doesn't exist
    }

    struct Record r;
    int maxID = 0;

    while (getAccountFromFile(fp, r.name, &r))
    {
        if (r.id > maxID)
            maxID = r.id;
    }

    fclose(fp);
    return maxID + 1; // Return the next available ID
}

int validateInput(struct Record r, int n)
{
    switch (n)
    {
    case 1: // date (until turned to strings)
        return 1;
    case 2: // account number
        return r.accountNbr > 0;
    case 3: // country
        if (strlen(r.country) == 0 || strlen(r.country) >= 50)
            return 0;
        for (size_t i = 0; i < strlen(r.country); ++i)
            if (r.country[i] >= '0' && r.country[i] <= '9')
                return 0;
        return 1;

    case 4: // phone
        // all-digit string
        size_t len = strlen(r.phone);
        if (len > 15) // phone number must be exactly 10 digits
            return 0;
        for (size_t i = 0; i < len; ++i)
            if (r.phone[i] < '0' || r.phone[i] > '9')
                return 0;

        return 1;

    case 5: // amount
    return r.amount > 10 && r.amount <= 10000000;
    case 6: // account type
        return strcmp(r.accountType, "saving") == 0 ||
               strcmp(r.accountType, "current") == 0 ||
               strcmp(r.accountType, "fixed01") == 0 ||
               strcmp(r.accountType, "fixed02") == 0 ||
               strcmp(r.accountType, "fixed03") == 0;

    default:
        return 0;
    }
}