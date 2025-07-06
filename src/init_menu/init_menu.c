#include "../header.h"
#include "init_menu.h"

void initMenu(struct User *u)
{
    char title[] = "Init Menu";
    char *options[] = {"Login", "Register", "Exit"};
    int choice = menuSelect(title, options, 3);
    system("clear");

    switch (choice)
    {
    case 1:
        Login(u);
        break;
    case 2:
        Register(u);
        break;
    case 3:
        exit(0);
        break;
    }
};
