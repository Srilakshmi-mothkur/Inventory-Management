// dashboard.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

void initList(List *pl)
{
    pl->head = NULL;
}

Product *createProduct(int id, char *name, char *category, int quantity, int price)
{
    struct Product *temp = malloc(sizeof(struct Product));
    if (temp == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    temp->id = id;
    strcpy(temp->name, name);
    strcpy(temp->category, category);
    temp->quantity = quantity;
    temp->price = price;
    temp->next = NULL;
    return temp;
}

void insertProduct(List *pl, int id, char *name, char *category, int quantity, int price)
{
    struct Product *p = pl->head;

    // Check if the product with the given ID already exists
    while (p != NULL)
    {
        if (p->id == id)
        {
            // Product with the same ID found, update the quantity
            p->quantity += quantity;
            return;
        }
        p = p->next;
    }

    // If the product with the given ID doesn't exist, add a new product
    struct Product *temp = createProduct(id, name, category, quantity, price);
    temp->next = pl->head;
    pl->head = temp;
}

int countProducts(List *pl)
{
    struct Product *p = pl->head;
    int count = 0;
    while (p != NULL)
    {
        count++;
        p = p->next;
    }
    return count;
}

int idExistsInCategory(const List *pl, int id)
{
    const Product *p = pl->head;

    while (p != NULL)
    {
        if (p->id == id)
        {
            return 1; // ID found
        }
        p = p->next;
    }

    return 0; // ID not found
}

void sellProduct(List *pl, int id, int qty)
{
    Product *p = pl->head;

    while (p != NULL)
    {
        if (p->id == id)
        {
            p->quantity = p->quantity - qty;
            return; // Exit the function after deletion
        }
        p = p->next;
    }

    // If the loop completes, the ID was not found
    printf("Product with ID %d not found.\n", id);
}

void display(List *pl)
{
    struct Product *p = pl->head;
    if (p == NULL)
    {
        printf("Empty list.\n");
        return;
    }
    else
    {
        printf("Inventory:\n");
        printf("%-5s%-20s%-10s%-10s%-20s\n", "ID", "Name", "Quantity", "Price", "Category");
        printf("------------------------------------------------------------\n");
    }
    while (p != NULL)
    {
        printf("%-5d%-20s%-10d%-10d%-20s\n", p->id, p->name, p->quantity, p->price, p->category);
        printf("------------------------------------------------------------\n");
        p = p->next;
    }
    printf("\n");
}

void restock(List *pl)
{
    Product *p = pl->head;
    while (p != NULL)
    {
        if (p->quantity <= 10)
        {
            printf("Restock for %s request sent to supplier.\n", p->name);
        }
        p = p->next;
    }
    printf("\n");
}
