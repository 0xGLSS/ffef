#include "init_menu.h"
#include "../header.h"

#define MAX_INPUT_REG 30

void Register(struct User *u) {

    FILE *file = fopen(USERS, "a+");
    if (!file) {
        printf("Error opening user file.\n");
        sleep(1);
        return;
    }
    printf("=== Register ===\n");

    printf("Choose a Username: ");
    fgets(u->name, MAX_INPUT_REG, stdin);
    u->name[strcspn(u->name, "\n")] = 0;  // remove newline
    
    if (validateName(file, u->name) != 0){
        printf("Username already exists\n");
        sleep(1);
        system("clear");
        fclose(file);
        Register(u);
        return;
    }
    if (strlen(u->name) == 0) {
        printf("Username can't be empty\n");
        sleep(1);
        system("clear");
        fclose(file);
        Register(u);
        return;
    }

    printf("Choose a Password: ");
    fgets(u->password, MAX_INPUT_REG, stdin);
    u->password[strcspn(u->password, "\n")] = 0;  // remove newline
    
    if (strlen(u->password) == 0) {
        printf("Username can't be empty\n");
        sleep(1);
        system("clear");
        fclose(file);
        Register(u);
        return;
    }

    int id = getId(file);  // Should return int

    SaveUser(id, u->name, u->password);

    fclose(file);
}

int getId(FILE *file) {
    int id = -1;
    rewind(file);

    // Read the first integer on each line and store it directly in 'id'
    while (fscanf(file, "%d %*s %*s", &id) != EOF) {
        // no extra variable needed
    }

    return (id == -1) ? 0 : id + 1;
}
int validateName(FILE *file, const char *name) {
    rewind(file);  // Go to start
    char username[100];

    while (fscanf(file, "%*d %19s %*s", username) != EOF) {  // Read ID, username, skip password
        if (strcmp(username, name) == 0) {
            return -1;  // Username exists
        }
    }

    return 0;
}

void SaveUser(int id, const char *name, const char *password) {
    FILE *file = fopen(USERS, "a+");  // Open for reading and appending
    if (!file) return;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    if (size > 0) {
        fseek(file, -1, SEEK_END);
        if (fgetc(file) != '\n') {
            fputc('\n', file);  // Add newline only if missing
        }
    }

    fprintf(file, "%d %s %s\n", id, name, password);
    fclose(file);

    printf("Saved: %d %s %s\n", id, name, password);
    sleep(1);
}
