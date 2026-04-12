Write a c program to implement the symbol table. Give input expression as a+b+-c$
============================

  #include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Structure for Symbol Table Entry
struct symbol {
    char name;
    int address;
    int value;
};

struct symbol table[MAX];
int count = 0;
int nextAddress = 1000;  // starting memory address

// Check if symbol already exists
int exists(char ch) {
    for (int i = 0; i < count; i++) {
        if (table[i].name == ch)
            return i;
    }
    return -1;
}

// Add symbol to table
void addSymbol(char ch) {
    if (exists(ch) == -1) {
        table[count].name = ch;
        table[count].address = nextAddress++;
        table[count].value = 0; // default value
        count++;
    }
}

// Modify value of variable
void modifyValue(char ch, int val) {
    int index = exists(ch);
    if (index != -1) {
        table[index].value = val;
    }
}

int main() {
    char expr[] = "a+b+-c$";

    printf("Input Expression: %s\n", expr);

    // Extract variables and store in symbol table
    for (int i = 0; i < strlen(expr); i++) {
        if (isalpha(expr[i])) {
            addSymbol(expr[i]);
        }
    }

    // Assign values to variables
    printf("\nEnter values for variables:\n");
    for (int i = 0; i < count; i++) {
        int val;
        printf("Enter value for %c: ", table[i].name);
        scanf("%d", &val);
        modifyValue(table[i].name, val);
    }

    // Display Symbol Table
    printf("\nSymbol Table:\n");
    printf("Index\tVariable\tAddress\t\tValue\n");

    for (int i = 0; i < count; i++) {
        printf("%d\t%c\t\t%d\t\t%d\n",
               i,
               table[i].name,
               table[i].address,
               table[i].value);
    }

    return 0;
}
