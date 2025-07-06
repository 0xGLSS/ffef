#include "../header.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_INPUT_LEN 100

int getAccountFromFile(FILE *ptr, char name[30], struct Record *r)
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
                  r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveRecord(struct User u, struct Record r)
{
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf)
    {
        printf("Error opening records file for writing.\n");
        return;
    }

    fseek(pf, 0, SEEK_END);

    if (ftell(pf) > 0) // Not empty
    {
        fseek(pf, -1, SEEK_END);
        int lastChar = fgetc(pf);
        fseek(pf, -2, SEEK_END);
        int lastlastChar = fgetc(pf);

        if (lastChar != '\n')
        {
            fputc('\n', pf);
        }
        fputc('\n', pf);
    }

    fprintf(pf, "%d %d %s %d %d/%d/%d %s %s %.2lf %s",
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

    fclose(pf);
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

int validateInput(struct Record r)
{
    printf("\n");

    int valid = 1;

    // Date
    if (r.deposit.month < 1 || r.deposit.month > 12 ||
        r.deposit.day < 1 || r.deposit.day > 31 ||
        r.deposit.year < 1900 || r.deposit.year > 2100)
    {
        printf("Invalid date!\n");
        valid = 0;
    }

    // Account number
    if (r.accountNbr <= 0 || r.accountNbr > 999999999999999)
    {
        printf("Invalid account number!\n");
        valid = 0;
    }

    // Country
    if (strlen(r.country) == 0 || strlen(r.country) >= 50)
    {
        printf("Invalid country name length!\n");
        valid = 0;
    }
    else
    {
        for (size_t i = 0; i < strlen(r.country); i++)
            if (r.country[i] >= '0' && r.country[i] <= '9')
            {
                printf("Country name must not contain digits!\n");
                valid = 0;
                break;
            }
    }

    // Phone
    size_t len = strlen(r.phone);
    if (len == 0 || len > 15)
    {
        printf("Phone number length invalid!\n");
        valid = 0;
    }
    else
    {
        for (size_t i = 0; i < len; i++)
            if (r.phone[i] < '0' || r.phone[i] > '9')
            {
                printf("Phone number must be digits only!\n");
                valid = 0;
                break;
            }
    }

    // Amount
    if (r.amount <= 10 || r.amount > 10000000)
    {
        printf("Amount must be > 10 and <= 10,000,000!\n");
        valid = 0;
    }

    // Account type
    if (!(strcmp(r.accountType, "savings") == 0 ||
          strcmp(r.accountType, "current") == 0 ||
          strcmp(r.accountType, "fixed01") == 0 ||
          strcmp(r.accountType, "fixed02") == 0 ||
          strcmp(r.accountType, "fixed03") == 0))
    {
        printf("Invalid account type!\n");
        valid = 0;
    }

    return valid;
}

int accountExistsForUser(const char *userName, int accountNbr)
{
    FILE *pf = fopen(RECORDS, "r");
    if (!pf)
    {
        printf("Error opening records file.\n");
        return 0;
    }

    struct Record cr;
    char name[30];

    while (getAccountFromFile(pf, name, &cr))
    {
        if (strcmp(name, userName) == 0 && cr.accountNbr == accountNbr)
        {
            fclose(pf);
            return 1;
        }
    }

    fclose(pf);
    return 0; 
}

int getNextRecordId()
{
    FILE *fp = fopen(RECORDS, "r");
    if (!fp)
    {
        perror("Error opening file");
        return 0;
    }

    struct Record r;
    int maxID = -1;

    while (getAccountFromFile(fp, r.name, &r))
    {
        if (r.id > maxID)
            maxID = r.id;
    }

    fclose(fp);
    return maxID + 1;
}

void createNewAcc(struct User u)
{
    struct Record r;
    char inputs[7][MAX_INPUT_LEN + 1];
    const char *fields[] = {
        "Today's date (mm/dd/yyyy)",
        "Account number",
        "Country",
        "Phone number",
        "Amount to deposit",
        "Type of account (savings/current/fixed01/fixed02/fixed03)"};

    while (1)
    {
        menuInput("New Account Creation", fields, inputs, 6);

        // Parse all inputs into r
        sscanf(inputs[0], "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

        sscanf(inputs[1], "%d", &r.accountNbr);

        strncpy(r.country, inputs[2], sizeof(r.country) - 1);
        r.country[sizeof(r.country) - 1] = '\0';

        sscanf(inputs[3], "%s", r.phone);

        sscanf(inputs[4], "%lf", &r.amount);

        strncpy(r.accountType, inputs[5], sizeof(r.accountType) - 1);
        r.accountType[sizeof(r.accountType) - 1] = '\0';

        if (!validateInput(r))
        {
            printf("\n");
            printf("Please fix the errors above.\n");
            getchar();
            continue;
        }

        if (accountExistsForUser(u.name, r.accountNbr))
        {
            printf("✖ This Account already exists for this user\n");
            getchar();
            continue;
        }

        // Save new record
        r.id = getNextRecordId();
        saveRecord(u, r);
        success(u);
        break;
    }
}
