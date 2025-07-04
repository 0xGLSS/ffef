#include "header.h"
#include "init_menu/init_menu.h"

const char *RECORDS = "./data/records.txt";

int menuSelect(const char *title, char *options[], int n_options)
{
    printf("\n=== %s ===\n", title);
    for (int i = 0; i < n_options; i++) {
        printf("[%d] %s\n", i + 1, options[i]);
    }
    int choice = 0;
    do {
        printf("Select option (1-%d): ", n_options);
        scanf("%d", &choice);
        while (getchar() != '\n'); // clear stdin
    } while (choice < 1 || choice > n_options);
    return choice;
}

int main()
{
    struct User u;
    initMenu(&u);
    mainMenu(u);
    return 0;
}
