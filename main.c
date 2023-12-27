// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"

int main()
{
    User *userList = NULL;

    // Load existing users from file
    loadUsersFromFile(&userList);

    int choice;
    int loggedIn = 0; // Flag to check if the user is logged in
    int pos, id, price, quantity;
    char name[100], category[100];

    do
    {
        printf("\n1. Register\n2. Login\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (registerUser(&userList) == 0)
            {
                printf("Registration successful! Welcome!\n");
            }
            break;
        case 2:
        {
            struct User loginAttempt;
            if (loginUser(userList, &loginAttempt) == 0)
            {
                printf("Login successful! Welcome, %s!\n", loginAttempt.username);
                loggedIn = 1; // Set the flag to indicate that the user is logged in

                // Find the current user in the linked list
                User *currentUser = findUser(userList, loginAttempt.username);
                // Load the user's inventory from file
                loadInventoryFromFile(&(currentUser->inventory), currentUser->username);
                // Ensure the user's inventory is initialized
                initUserInventory(currentUser);

                // After login, allow the user to work with their personal inventory
                int dashboardChoice;
                do
                {
                    printf("\n1. Add Product\n2. Remove Product\n3. Display Inventory\n4. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &dashboardChoice);

                    switch (dashboardChoice)
                    {
                    case 1: // Add Product
                        printf("Enter the id: ");
                        scanf("%d", &id);
                        printf("Enter the name: ");
                        scanf("%s", name);
                        printf("Enter the category: ");
                        scanf("%s", category);
                        printf("Enter the quantity: ");
                        scanf("%d", &quantity);
                        printf("Enter the price: ");
                        scanf("%d", &price);
                        if (strcmp(category, "food") == 0 || strcmp(category, "toiletries") == 0)
                        {
                            insertProduct(&(currentUser->inventory), id, name, category, quantity, price);
                            printf("Product added to the inventory.\n");
                        }
                        else
                        {
                            printf("Category not valid.\n");
                        }

                        break;
                    case 2: // Remove Product
                        printf("Enter the id of the product to remove: ");
                        scanf("%d", &id);
                        int qty;
                        printf("Enter the quantity to be reduced: ");
                        scanf("%d", &qty);
                        if (idExistsInCategory(&(currentUser->inventory), id) == 1)
                        {
                            sellProduct(&(currentUser->inventory), id, qty);
                            printf("Product with ID %d reduced by %d from the inventory.\n", id, qty);
                        }
                        else
                        {
                            printf("Product with ID %d not found in the inventory.\n", id);
                        }
                        restock(&(currentUser->inventory));
                        break;
                    case 3: // Display Inventory
                        display(&(currentUser->inventory));
                        break;
                    case 4: // Logout
                        printf("Logging out...\n");
                        loggedIn = 0; // Set the flag to indicate that the user is logged out
                        saveInventoryToFile(&(currentUser->inventory), currentUser->username);
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    }
                } while (dashboardChoice != 4);
            }
            else
            {
                printf("Login failed. Check your username and password.\n");
            }
        }
        break;
        case 3:
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    // Before exiting the program, save the user list to the file
    saveUsersToFile(userList);

    // Free the allocated memory for the linked list
    freeUserList(userList);

    return 0;
}
