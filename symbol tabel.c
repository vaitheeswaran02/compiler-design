#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for Symbol Table Entry */
struct symbol {
    char name[20];
    char type[20];
    char scope[10];     // global or local
    int memory;         // memory location
    char value[20];     // value
    struct symbol *next;
};

struct symbol *head = NULL;

/* Insert a symbol */
void insert() {
    struct symbol *newnode;
    newnode = (struct symbol *)malloc(sizeof(struct symbol));

    printf("Enter Symbol Name: ");
    scanf("%s", newnode->name);

    printf("Enter Data Type: ");
    scanf("%s", newnode->type);

    printf("Enter Scope (global/local): ");
    scanf("%s", newnode->scope);

    printf("Enter Memory Location: ");
    scanf("%d", &newnode->memory);

    printf("Enter Value: ");
    scanf("%s", newnode->value);

    newnode->next = head;
    head = newnode;

    printf("Symbol Inserted Successfully!\n");
}

/* Display the Symbol Table */
void display() {
    struct symbol *temp = head;

    if (temp == NULL) {
        printf("Symbol Table is Empty!\n");
        return;
    }

    printf("\n-------------------------------------------------------------\n");
    printf("Name\tType\tScope\tMemory\tValue\n");
    printf("-------------------------------------------------------------\n");

    while (temp != NULL) {
        printf("%s\t%s\t%s\t%d\t%s\n",
               temp->name,
               temp->type,
               temp->scope,
               temp->memory,
               temp->value);
        temp = temp->next;
    }
}

/* Delete a Symbol */
void deleteSymbol() {
    char key[20];
    struct symbol *temp = head, *prev = NULL;

    if (head == NULL) {
        printf("Symbol Table is Empty!\n");
        return;
    }

    printf("Enter Symbol Name to Delete: ");
    scanf("%s", key);

    /* If first node */
    if (strcmp(head->name, key) == 0) {
        temp = head;
        head = head->next;
        free(temp);
        printf("Symbol Deleted Successfully!\n");
        return;
    }

    while (temp != NULL && strcmp(temp->name, key) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Symbol Not Found!\n");
    } else {
        prev->next = temp->next;
        free(temp);
        printf("Symbol Deleted Successfully!\n");
    }
}

/* Main Function */
int main() {
    int choice;

    while (1) {
        printf("\n===== SYMBOL TABLE MENU =====\n");
        printf("1. Insert Symbol\n");
        printf("2. Display Symbol Table\n");
        printf("3. Delete Symbol\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: insert(); break;
            case 2: display(); break;
            case 3: deleteSymbol(); break;
            case 4: exit(0);
            default: printf("Invalid Choice!\n");
        }
    }
    return 0;
}
