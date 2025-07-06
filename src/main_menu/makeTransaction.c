#include "../header.h"
#include "main_menu.h"
#include <stdbool.h>
#include <string.h>

bool hasPrefix(const char *str, const char *prefix)
{
    size_t lenPrefix = strlen(prefix);
    return strncmp(str, prefix, lenPrefix) == 0;
}

void makeTransaction(struct User u)
{
    int accountNbrs[100];
    int count = getOwnedIds(u, accountNbrs);

    if (count == 0)
    {
        printf("No accounts to make transactions.\n");
        return;
    }

    char *options[100];

    for (int i = 0; i < count; i++)
    {
        options[i] = malloc(20);
        sprintf(options[i], "%d", accountNbrs[i]);
    }

    int choice = menuSelect("Choose Account", options, count);
    int selectedNbr = accountNbrs[choice - 1];

    struct Record r;

    getInfoFromId(u, selectedNbr, &r);

    if (hasPrefix(r.accountType, "fixed"))
    {
        printf("Transactions are not allowed on fixed accounts.\n");
        return;
    }

    choice = menuSelect("Transaction Type", (char *[]){"Deposit", "Withdrawal"}, 2);

    switch (choice)
    {
    case 1:
        applyModification(u, selectedNbr, Deposit);
        break;
    case 2:
        applyModification(u, selectedNbr, Withdraw);
        break;
    }
}

void Withdraw(struct Record *r)
{
    printf("Current balance: %.2lf\n", r->amount);
    double amount;

    while (1)
    {
        printf("Withdraw: ");
        int res = scanf("%lf", &amount);
        getchar(); // consume leftover newline

        // Check if input is valid and balance is sufficient
        if (res == 1 && amount > 0 && amount <= r->amount)
        {
            r->amount -= amount; // Apply the withdrawal
            printf("Withdrawal successful. New balance: %.2lf\n", r->amount);
            sleep(1);
            break;
        }

        printf("Invalid amount! Please try again.\n");
    }
}

void Deposit(struct Record *r)
{
    printf("Current balance: %.2lf\n", r->amount);
    double amount;

    while (1)
    {
        printf("Deposit: ");
        int res = scanf("%lf", &amount);
        getchar(); // consume leftover newline

        // Check if input is valid and balance is sufficient
        if (res == 1 && amount > 0 && amount < 999999999)
        {
            r->amount += amount; // Apply the deposit
            printf("Deposit successful. New balance: %.2lf\n", r->amount);
            sleep(1);
            break;
        }

        printf("Invalid amount! Please try again.\n");
    }
}