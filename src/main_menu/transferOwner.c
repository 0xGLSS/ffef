#include "../header.h"
#include "main_menu.h"

int userExists(const char *username)
{
    FILE *pf = fopen(USERS, "r");
    if (!pf)
    {
        printf("Error opening user file.\n");
        return 0;
    }

    struct User u;
    while (fscanf(pf, "%d %s %s", &u.id, u.name, u.password) == 3)
    {
        if (strcmp(u.name, username) == 0)
        {
            fclose(pf);
            return 1;
        }
    }
    fclose(pf);
    return 0;
}

void changeUsername(char *newOwner)
{
    printf("Which user to transfer to (username) ?: ");
    scanf("%s", newOwner);

    if (!userExists(newOwner))
    {
        printf("This account does not exist.\n");
        sleep(2);
        newOwner[0] = '\0'; // Mark as invalid
        return;
    }

    printf("Owner name changed successfully to: %s\n", newOwner);
    sleep(2);
}


void changeOwner(struct User u, int accountNbr)
{
    char line[512];
    const char *origPath = RECORDS;
    const char *tempPath = "records.tmp";

    FILE *in = fopen(origPath, "r");
    FILE *out = fopen(tempPath, "w");
    if (!in || !out)
    {
        if (in)
            fclose(in);
        if (out)
            fclose(out);
        return;
    }

    int found = 0;

    while (fgets(line, sizeof(line), in))
    {
        struct Record r;
        char userName[30];

        int parsed = sscanf(line, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                            &r.id, &r.userId, userName, &r.accountNbr,
                            &r.deposit.month, &r.deposit.day, &r.deposit.year,
                            r.country, r.phone, &r.amount, r.accountType);

        if (parsed == 11 && strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr && !found)
        {
            char newOwner[30];
            changeUsername(newOwner);
            if (strlen(newOwner) == 0)
            {
                // Invalid transfer, write the original line and skip
                fputs(line, out);
                found = 1;
                continue;
            }

            snprintf(line, sizeof(line),
                     "%d %d %s %d %d/%d/%d %s %s %.2lf %s\n",
                     r.id, r.userId, newOwner, r.accountNbr,
                     r.deposit.month, r.deposit.day, r.deposit.year,
                     r.country, r.phone, r.amount, r.accountType);

            found = 1;
        }

        fputs(line, out);
    }

    fclose(in);
    fclose(out);

    if (found)
    {
        if (remove(origPath) != 0 || rename(tempPath, origPath) != 0)
        {
            perror("Error replacing records file");
        }
        else
        {
            printf("Ownership transferred successfully.\n");
            success(u);
        }
    }
    else
    {
        printf("Account not found.\n");
        remove(tempPath); // Optional cleanup
    }
}


void transferOwner(struct User u)
{
    int accountNbrs[100];
    int count = getOwnedIds(u, accountNbrs);

    if (count == 0)
    {
        printf("No accounts to transfer.\n");
        return;
    }

    char *options[100];

    for (int i = 0; i < count; i++)
    {
        options[i] = malloc(20);
        sprintf(options[i], "%d", accountNbrs[i]);
    }

    int choice = menuSelect("Choose Account to Transfer", options, count);
    int selectedNbr = accountNbrs[choice - 1];

    struct Record r;

    if (!getInfoFromId(u, selectedNbr, &r))
    {
        printf("Account not found.\n");
        return;
    }

    printf("Account Number: %d\n", r.accountNbr);
    printf("Deposit Date: %02d/%02d/%04d\n", r.deposit.month, r.deposit.day, r.deposit.year);
    printf("Country: %s\n", r.country);
    printf("Phone: %s\n", r.phone);
    printf("Amount: %.2lf\n", r.amount);
    printf("Account Type: %s\n\n", r.accountType);

    changeOwner(u, selectedNbr);
}
