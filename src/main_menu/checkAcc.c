#include "../header.h"
#include "main_menu.h"

void checkAcc(struct User u)
{
    int accountNbrs[100];
    int count = getOwnedIds(u, accountNbrs);

    if (count == 0)
    {
        printf("No accounts to display.\n");
        return;
    }

    char *options[100];

    for (int i = 0; i < count; i++)
    {
        options[i] = malloc(20);
        sprintf(options[i], "%d", accountNbrs[i]);
    }

    char title[] = "Choose Account";
    int choice = menuSelect(title, options, count);

    int selectedNbr = accountNbrs[choice - 1];

    displayInfo(u, selectedNbr);
}

void displayInfo(struct User u, int accountNbr)
{
    struct Record r;

    if (getInfoFromId(u, accountNbr, &r))
    {
        printf("\nUser Information:\n");
        printf("Account Number: %d\n", r.accountNbr);
        printf("Deposit Date: %02d/%02d/%04d\n", r.deposit.month, r.deposit.day, r.deposit.year);
        printf("Country: %s\n", r.country);
        printf("Phone: %s\n", r.phone);
        printf("Amount: %.2lf\n", r.amount);
        printf("Account Type: %s\n\n", r.accountType);
        calcInterest(r);
    }
}


int getInfoFromId(struct User u, int accountNbr, struct Record *result)
{
    FILE *in = fopen(RECORDS, "r");
    if (!in)
    {
        printf("Error opening records file.\n");
        return 0; 
    }

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), in))
    {
        struct Record r;
        char userName[100];

        int parsed = sscanf(line, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                            &r.id, &r.userId, userName, &r.accountNbr,
                            &r.deposit.month, &r.deposit.day, &r.deposit.year,
                            r.country, r.phone, &r.amount, r.accountType);

        if (parsed == 11 && strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            *result = r; // Copy found record to the result pointer
            found = 1;
            break;
        }
    }

    fclose(in);
    return found;
}


void calcInterest(struct Record r)
{
    if (strcmp(r.accountType, "savings") == 0)
    {
        double rate = 7.0;
        double monthly_interest = r.amount * (rate / 100.0) / 12.0;
        printf("You will get $%.2f as interest on day %d of every month.\n", monthly_interest, r.deposit.day);
        return;
    }

    if (strcmp(r.accountType, "fixed01") == 0)
    {
        double rate = 4.0;
        double interest = r.amount * (rate / 100.0) * 1; // 1 year
        printf("You will get $%.2f of interest on %02d/%02d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year + 1);
        return;
    }

    if (strcmp(r.accountType, "fixed02") == 0)
    {
        double rate = 5.0;
        double interest = r.amount * (rate / 100.0) * 2; // 2 years
        printf("You will get $%.2f of interest on %02d/%02d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year + 2);
        return;
    }

    if (strcmp(r.accountType, "fixed03") == 0)
    {
        double rate = 8.0;
        double interest = r.amount * (rate / 100.0) * 3; // 3 years
        printf("You will get $%.2f of interest on %02d/%02d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year + 3);
        return;
    }

    if (strcmp(r.accountType, "current") == 0)
    {
        printf("You will not get interests because the account is of type current.\n");
        return;
    }
}
