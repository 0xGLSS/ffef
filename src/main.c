#include "header.h"
#include "init_menu/init_menu.h"

const char *RECORDS = "./data/records.txt";
struct User u;
#define MAX_INPUT_LEN 100

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

void menuInput(const char *title,
               const char *fields[],
               char inputs[][MAX_INPUT_LEN+1],
               int n)
{
    printf("\n=== %s ===\n", title);
    for (int i = 0; i < n; i++) {
        printf("%s: ", fields[i]);
        fgets(inputs[i], MAX_INPUT_LEN + 1, stdin);
        // Remove trailing newline
        size_t len = strlen(inputs[i]);
        if (len > 0 && inputs[i][len - 1] == '\n') {
            inputs[i][len - 1] = '\0';
        }
    }
}

/*
void menuInput(const char *title,
               const char *fields[],
               char inputs[][MAX_INPUT_LEN+1],
               int n,
               int authenticated)
{
    printf("\n=== %s ===\n", title);
    for (int i = 0; i < n; i++) {
        printf("%s: ", fields[i]);
        fgets(inputs[i], MAX_INPUT_LEN + 1, stdin);
        // Remove trailing newline
        size_t len = strlen(inputs[i]);
        if (len > 0 && inputs[i][len - 1] == '\n') {
            inputs[i][len - 1] = '\0';
        }
    }
    printf("[1] Submit\n");
    if (authenticated == 2) {
        printf("[2] Menu\n");
    }
    int choice = 1;
    if (authenticated == 2) {
        printf("Choose option: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // clear stdin
        if (choice == 2) {
            mainMenu(u);
            return;
        }
    }
}

*/

int main()
{
    initMenu(&u);
    mainMenu(u);
    return 0;
}
