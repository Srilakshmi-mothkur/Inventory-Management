// inventory.h
#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

typedef struct Product
{
    int id;
    char name[100];
    char category[100];
    int quantity;
    int price;
    struct Product *next;
} Product;

typedef struct List
{
    Product *head;
} List;

typedef struct User
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    List inventory; // Each user has their own inventory
    struct User *next;
} User;

void initList(List *pl);
Product *createProduct(int id, char *name, char *category, int quantity, int price);
void insertProduct(List *pl, int id, char *name, char *category, int quantity, int price);
int countProducts(List *pl);
void display(List *pl);
void restock(List *pl);
int idExistsInCategory(const List *pl, int id);
void deleteById(List *pl, int id);
void sellProduct(List *pl, int id, int qty);

int registerUser(User **userList);
int loginUser(const User *userList, User *loginAttempt);
void saveUsersToFile(const User *userList);
void loadUsersFromFile(User **userList);
User *findUser(const User *userList, const char *username);
void initUserInventory(User *user);
void freeUserList(User *userList);
void saveInventoryToFile(List *pl, const char *username);
void loadInventoryFromFile(List *pl, const char *username);
#endif
