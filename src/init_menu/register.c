#include "init_menu.h"
#include "../header.h"

#define MAX_INPUT_REG 30

void clear()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}

int getNextId(FILE *file)
{
    int id = -1;
    rewind(file);

    while (fscanf(file, "%d %*s %*s", &id) != EOF);

    return id + 1;
}

int validateName(const char *name)
{
    FILE *file = fopen(USERS, "r");
    if (!file)
        return -1;

    char username[30], password[30];
    int id, res;

    while ((res = fscanf(file, "%d %29s %29s", &id, username, password)) != EOF)
    {
        if (res != 3)
        {
            printf("Corrupted user file.\n");
            fclose(file);
            exit(1);
        }

        if (strcmp(username, name) == 0)
        {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

void SaveUser(int id, const char *name, const char *password)
{
    FILE *file = fopen(USERS, "a+");
    if (!file)
        return;

    fseek(file, 0, SEEK_END);

    if (ftell(file) > 0)
    {
        fseek(file, -1, SEEK_END);
        if (fgetc(file) != '\n')
        {
            fputc('\n', file);
        }
    }

    fprintf(file, "%d %s %s\n", id, name, password);
    fclose(file);

    printf("Saved: %d %s %s\n", id, name, password);
    sleep(1);
}

void Register(struct User *u)
{
    FILE *file = fopen(USERS, "a+");
    if (!file)
    {
        printf("Error opening user file.\n");
        sleep(1);
        return;
    }

    printf("=== Register ===\n");

    while (1)
    {

        printf("Choose a Username: ");
        scanf("%29s", u->name);

        clear();

        if (strlen(u->name) == 29)
        {
            printf("username too long, maximum 28.\n");
            sleep(1);
            system("clear");
            continue;
        }

        if (validateName(u->name) != 0)
        {
            printf("Username already exists.\n");
            sleep(1);
            system("clear");
            continue;
        }

        break;
    }

    while (1)
    {
        printf("Choose a Password: ");
        scanf("%29s", u->password);

        clear();

        if (strlen(u->password) == 29)
        {
            printf("password too long, maximum 28\n");
            sleep(1);
            system("clear");
            continue;
        }

        break;
    }

    int id = getNextId(file);
    SaveUser(id, u->name, u->password);
    fclose(file);
}

