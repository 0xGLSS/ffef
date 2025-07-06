#include <termios.h>
#include "../header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void loginMenu(struct User *u)
{
    printf("=== Login ===\n");

    printf("Username: ");
    scanf("%29s", u->name);

    printf("Password: ");
    scanf("%29s", u->password);
}

int getPassword(struct User *u)
{
    FILE *fp = fopen(USERS, "r");
    if (!fp)
    {
        return -1;
    }

    struct User userFromFile;
    while (fscanf(fp, "%d %s %s", &userFromFile.id, userFromFile.name, userFromFile.password) != EOF)
    {
        if (strcmp(userFromFile.name, u->name) == 0 && strcmp(userFromFile.password, u->password) == 0)
        {
            u->id = userFromFile.id;
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return -1;
}

void Login(struct User *u)
{
    while (1)
    {
        loginMenu(u);
        if (getPassword(u) != 0)
        {
            printf("Wrong password or User Name! Try again.\n");
            sleep(1);
            system("clear");
        } else {
            printf("Login successful!\n");
            sleep(1);
            break;
        }
    }
}