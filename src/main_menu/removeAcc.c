#include "../header.h"
#include "main_menu.h"

void deleteAccount(struct User u, int accountNbr)
{
    FILE *in = fopen(RECORDS, "r");
    FILE *out = fopen("records.tmp", "w");
    if (!in || !out)
    {
        perror("Error opening file");
        fclose(in);
        fclose(out);
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), in))
    {
        struct Record r;
        char userName[30];
        int n = sscanf(line,
                       "%d %d %30s %d %d/%d/%d %49s %19s %lf %9s",
                       &r.id, &r.userId, userName, &r.accountNbr,
                       &r.deposit.month, &r.deposit.day, &r.deposit.year,
                       r.country, r.phone, &r.amount, r.accountType);

        if (n == 11 && strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            char confirm[4];
            printf("Delete account %d? (yes/no): ", accountNbr);
            if (fgets(confirm, sizeof(confirm), stdin) && strncmp(confirm, "yes", 3) == 0)
            {
                fgets(line, sizeof(line), in);
                continue;
            }
        }

        fputs(line, out);
    }

    fclose(in);
    fclose(out);

    if (remove(RECORDS) || rename("records.tmp", RECORDS))
        perror("Error updating records");
    else
        success(u);
}

void removeAcc(struct User u)
{
    int accountNbrs[100];
    int count = getOwnedIds(u, accountNbrs);

    if (count == 0)
    {
        printf("No accounts to remove.\n");
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

    deleteAccount(u, selectedNbr);
}
