#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header.h"
#include "main_menu.h"

void updateAcc(struct User u)
{
    int accountNbrs[100]; 
    int count = getOwnedIds(u, accountNbrs);

    if (count == 0)
    {
        printf("No accounts to update.\n");
        return;
    }

    char *options[100];

    for (int i = 0; i < count; i++)
    {
        options[i] = malloc(20);
        sprintf(options[i], "%d", accountNbrs[i]);
    }

    char title[] = "Update Account Information";

    int selectedNbr = menuSelectById("Update Account Information", accountNbrs, count);

    // Call the reusable function with the custom modifier
    applyModification(u, selectedNbr, updateCountryAndPhone);
}

int getOwnedIds(struct User u, int accountNbrs[])
{
    char userName[30];
    struct Record r;
    int maxNbrs = 100; 

    FILE *pf = fopen(RECORDS, "r");
    if (!pf)
    {
        printf("Error opening records file.\n");
        return 0;
    }

    int count = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            if (count < maxNbrs)
            {
                accountNbrs[count++] = r.accountNbr;
            }
        }
    }
    fclose(pf);
    return count; 
}

void updateCountryAndPhone(struct Record *r)
{
    printf("Current Country: %s\n", r->country);
    printf("Current Phone:   %s\n", r->phone);

    while (1)
    {
        printf("New Country: ");
        scanf("%s", r->country);

        if (validateInput(*r)) {
            break;
        } else {
            printf("Invalid country! Please try again.\n");
        }
    }

    while (1)
    {
        printf("New Phone: ");
        scanf("%s", r->phone);

        if (validateInput(*r)) {
            break;
        } else {
            printf("Invalid phone number! Please try again.\n");
        }
    }
}

void applyModification(struct User u, int accountNbr, void (*modifier)(struct Record *r))
{
    char line[512];
    const char *origPath = RECORDS;
    const char *tempPath = "records.tmp";

    FILE *in = fopen(origPath, "r");
    FILE *out = fopen(tempPath, "w");
    if (!in || !out)
    {
        perror("Error opening file");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    while (fgets(line, sizeof(line), in))
    {
        char originalLine[512];
        strcpy(originalLine, line);

        struct Record r;
        char userName[30];

        int parsed = sscanf(line, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                            &r.id, &r.userId, userName, &r.accountNbr,
                            &r.deposit.month, &r.deposit.day, &r.deposit.year,
                            r.country, r.phone, &r.amount, r.accountType);

        if (parsed == 11 && strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            modifier(&r);

            snprintf(line, sizeof(line),
                     "%d %d %s %d %d/%d/%d %s %s %.2lf %s\n",
                     r.id, r.userId, userName, r.accountNbr,
                     r.deposit.month, r.deposit.day, r.deposit.year,
                     r.country, r.phone, r.amount, r.accountType);

        }

        fputs(line, out);
    }

    fclose(in);
    fclose(out);
    if (remove(origPath) != 0 || rename(tempPath, origPath) != 0)
    {
        perror("Error replacing records file");
    }
    success(u);
}
