#include "header.h"
#include "init_menu/init_menu.h"

struct User u;
#define MAX_INPUT_LEN 100
const char *RECORDS = "./data/records.txt";
const char *USERS = "./data/users.txt";

int menuSelect(const char *title, char *options[], int n_options)
{
    system("clear");
    printf("\n=== %s ===\n", title);
    for (int i = 0; i < n_options; i++)
        printf("[%d] %s\n", i + 1, options[i]);

    int choice = 0;
    do
    {
        printf("Select option (1-%d): ", n_options);
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n'); // clear input buffer
            choice = 0;
        }
    } while (choice < 1 || choice > n_options);

    return choice;
}

int menuSelectById(const char *title, int options[], int n_options)
{
    int choice;
    int found;

    system("clear");
    printf("\n=== %s ===\n", title);

    printf("You have access to these IDs: [");
    for (int i = 0; i < n_options; i++)
        printf("%d%s", options[i], (i == n_options - 1) ? "" : ", ");
    printf("]\n");

    while (1)
    {
        printf("Select an ID: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        found = 0;
        for (int i = 0; i < n_options; i++)
        {
            if (options[i] == choice)
            {
                found = 1;
                break;
            }
        }

        if (found)
            return choice;

        printf("ID not found! This account does not exist.\n");
    }
}

void menuInput(const char *title, const char *fields[], char inputs[][MAX_INPUT_LEN + 1], int n)
{
    system("clear");
    printf("\n=== %s ===\n", title);

    for (int i = 0; i < n; i++)
    {
        printf("%s: ", fields[i]);
        fgets(inputs[i], MAX_INPUT_LEN + 1, stdin);

        inputs[i][strcspn(inputs[i], "\n")] = '\0';
    }
}

int main()
{
    initMenu(&u);
    mainMenu(u);
    return 0;
}
