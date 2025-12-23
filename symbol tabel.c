#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for Symbol Table Entry */
struct symbol {
    char name[20];
    char type[20];
    int scope;
    int line;
    struct symbol *next;
};

struct symbol *head = NULL;

/* Insert a symbol into the table */
void insert() {
    struct symbol *newnode;
    newnode = (struct symbol *)malloc(sizeof(struct symbol));

    printf("Enter Symbol Name: ");
    scanf("%s", newnode->name);

    printf("Enter Data Type: ");
    scanf("%s", newnode->type);

    printf("Enter Scope (0 = global, 1 = local): ");
    scanf("%d", &newnode->scope);

    printf("Enter Line Number: ");
    scanf("%d", &newnode->line);

    newnode->next = head;
    head = newnode;

    printf("Symbol Inserted Successfully!\n");
}

/* Display the symbol table */
void display() {
    struct symbol *temp = head;

    if (temp == NULL) {
        printf("Symbol Table is Empty!\n");
        return;
    }

    printf("\n-------------------------------------------------\n");
    printf("Name\tType\tScope\tLine\n");
    printf("-------------------------------------------------\n");

    while (temp != NULL) {
        printf("%s\t%s\t%d\t%d\n",
               temp->name,
               temp->type,
               temp->scope,
               temp->line);
        temp = temp->next;
    }
}

/* Search a symbol */
void search() {
    char key[20];
    struct symbol *temp = head;

    printf("Enter Symbol Name to Search: ");
    scanf("%s", key);

    while (temp != NULL) {
        if (strcmp(temp->name, key) == 0) {
            printf("Symbol Found!\n");
            printf("Name: %s\nType: %s\nScope: %d\nLine: %d\n",
                   temp->name,
                   temp->type,
                   temp->scope,
                   temp->line);
            return;
        }
        temp = temp->next;
    }

    printf("Symbol Not Found!\n");
}

/* Main Function */
int main() {
    int choice;

    while (1) {
        printf("\n===== SYMBOL TABLE MENU =====\n");
        printf("1. Insert Symbol\n");
        printf("2. Display Symbol Table\n");
        printf("3. Search Symbol\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: insert(); break;
            case 2: display(); break;
            case 3: search(); break;
            case 4: exit(0);
            default: printf("Invalid Choice!\n");
        }
    }
    return 0;
}
