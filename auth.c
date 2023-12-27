// auth.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

#define USER_FILE "users.txt"

#ifndef AUTH_C
#define AUTH_C

int registerUser(User **userList);
int loginUser(const User *userList, User *loginAttempt);
void saveUsersToFile(const User *userList);
void loadUsersFromFile(User **userList);
User *findUser(const User *userList, const char *username);
void initUserInventory(User *user);
void freeUserList(User *userList);

int registerUser(struct User **userList)
{
    struct User *newUser = (struct User *)malloc(sizeof(struct User));

    if (newUser == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter your username: ");
    scanf("%49s", newUser->username);

    // Check if the username already exists
    struct User *current = *userList;
    while (current != NULL)
    {
        if (strcmp(newUser->username, current->username) == 0)
        {
            printf("Username already exists. Please choose another username.\n");
            free(newUser);
            return -1;
        }
        current = current->next;
    }

    printf("Enter your password: ");
    scanf("%49s", newUser->password);

    // Add the new user to the linked list
    newUser->next = *userList;
    *userList = newUser;

    saveUsersToFile(*userList);

    return 0;
}

int loginUser(const struct User *userList, struct User *loginAttempt)
{
    printf("Enter your username: ");
    scanf("%49s", loginAttempt->username);

    printf("Enter your password: ");
    scanf("%49s", loginAttempt->password);

    const struct User *current = userList;
    while (current != NULL)
    {
        if (strcmp(loginAttempt->username, current->username) == 0 &&
            strcmp(loginAttempt->password, current->password) == 0)
        {
            return 0;
        }
        current = current->next;
    }

    return -1;
}

void saveUsersToFile(const User *userList)
{
    FILE *file = fopen(USER_FILE, "w");

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    const User *current = userList;
    while (current != NULL)
    {
        fprintf(file, "%s %s\n", current->username, current->password);
        current = current->next;
    }

    fclose(file);
}

void loadUsersFromFile(User **userList)
{
    FILE *file = fopen(USER_FILE, "r");

    if (file == NULL)
    {
        // If the file does not exist, assume an empty user list
        *userList = NULL;
        return;
    }

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    while (fscanf(file, "%49s %49s", username, password) == 2)
    {
        // Allocate memory for a new user
        User *newUser = (User *)malloc(sizeof(User));
        if (newUser == NULL)
        {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        // Copy data to the new user
        strcpy(newUser->username, username);
        strcpy(newUser->password, password);

        // Add the new user to the linked list
        newUser->next = *userList;
        *userList = newUser;
    }

    fclose(file);
}

User *findUser(const User *userList, const char *username)
{
    const User *current = userList;
    while (current != NULL)
    {
        if (strcmp(username, current->username) == 0)
        {
            return (User *)current;
        }
        current = current->next;
    }

    return NULL; // User not found
}

void initUserInventory(User *user)
{
    if (user != NULL && user->inventory.head == NULL)
    {
        initList(&(user->inventory));
    }
}

void freeUserList(User *userList)
{
    User *current = userList;
    User *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void saveInventoryToFile(List *pl, const char *username)
{
    char filename[50];
    sprintf(filename, "%s_inventory.txt", username);

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error opening file '%s' for writing.\n", filename);
        return;
    }

    Product *p = pl->head;
    while (p != NULL)
    {
        fprintf(file, "%d %s %d %d %s\n", p->id, p->name, p->quantity, p->price, p->category);
        p = p->next;
    }

    fclose(file);
    printf("Inventory saved successfully for user: %s\n", username);
}

void loadInventoryFromFile(List *pl, const char *username)
{
    char filename[50];
    sprintf(filename, "%s_inventory.txt", username);

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file '%s' for reading. Assuming an empty inventory.\n", filename);
        initList(pl);
        return;
    }

    initList(pl);
    int id, quantity, price;
    char name[100], category[100];

    while (fscanf(file, "%d %99s %d %d %99s", &id, name, &quantity, &price, category) == 5)
    {
        insertProduct(pl, id, name, category, quantity, price);
    }

    fclose(file);
    printf("Inventory loaded successfully for user: %s\n", username);
}

#endif
