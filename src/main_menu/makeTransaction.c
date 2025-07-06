#include "../header.h"
#include "main_menu.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
        printf("\nPress enter to return\n");
        clear();
        getchar();
        return;
    }

    char *options[100];
    for (int i = 0; i < count; i++)
    {
        options[i] = malloc(20);
        sprintf(options[i], "%d", accountNbrs[i]);
    }

    int selectedNbr = menuSelectById("Choose Account", accountNbrs, count);

    for (int i = 0; i < count; i++)
        free(options[i]);

    struct Record r;
    getInfoFromId(u, selectedNbr, &r);

    if (hasPrefix(r.accountType, "fixed"))
    {
        printf("Transactions are not allowed on fixed accounts.\n");
        printf("\nPress enter to return\n");
        clear();
        getchar();
        return;
    }

    int choice = menuSelect("Transaction Type", (char *[]){"Deposit", "Withdrawal"}, 2);

    switch (choice)
    {
    case 1:
        applyModification(u, selectedNbr, Deposit);
        break;
    case 2:
        applyModification(u, selectedNbr, Withdraw);
        break;
    default:
        printf("Invalid selection.\n");
        break;
    }

    printf("\nPress enter to return\n");
    clear();
    getchar();
}

void Withdraw(struct Record *r)
{
    printf("Current balance: %.2lf\n", r->amount);
    double amount;

    while (1)
    {
        printf("Withdraw: ");
        int res = scanf("%lf", &amount);
        getchar(); // Consume newline

        if (res == 1 && amount > 0 && amount <= r->amount)
        {
            r->amount -= amount;
            printf("Withdrawal successful. New balance: %.2lf\n", r->amount);
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
        getchar(); // Consume newline

        if (res == 1 && amount > 0 && amount < 999999999)
        {
            r->amount += amount;
            printf("Deposit successful. New balance: %.2lf\n", r->amount);
            break;
        }

        printf("Invalid amount! Please try again.\n");
    }
}
