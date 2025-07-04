#include <termios.h>
#include "../header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *USERS = "./data/users.txt";
int MAX_INPUT_LEN = 100;

void loginMenu(struct User *u) {
    printf("=== Login ===\n");
    
    printf("Username: ");
    fgets(u->name, MAX_INPUT_LEN, stdin);
    u->name[strcspn(u->name, "\n")] = 0;  // remove newline
    
    printf("Password: ");
    fgets(u->password, MAX_INPUT_LEN, stdin);
    u->password[strcspn(u->password, "\n")] = 0;  // remove newline
}

char *getPassword(struct User u) {
    FILE *fp = fopen(USERS, "r");
    if (!fp) {
        return NULL;
    }
    
    struct User userFromFile;
    while (fscanf(fp, "%*d %s %s", userFromFile.name, userFromFile.password) != EOF) {
        printf("Checking user: %s\n", userFromFile.name);  // Debugging line
        sleep(2);
        if (strcmp(userFromFile.name, u.name) == 0) {
            fclose(fp);
            return strdup(userFromFile.password);  // caller must free this!
        }
    }
    
    fclose(fp);
    return NULL;
}

void login(struct User *u) {
    while (1) {
        loginMenu(u);

        char *stored_password = getPassword(*u);
        if (!stored_password || strcmp(u->password, stored_password) != 0) {
            printf("Wrong password or User Name! Try again.\n");
            free(stored_password);
            sleep(1);
            system("clear");  // You can remove this if you don't want to clear screen
        } else {
            printf("Login successful!\n");
            free(stored_password);
            sleep(1);
            break;  // success
        }
    }
}