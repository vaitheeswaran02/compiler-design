#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Grammar storage
char grammar[20][20][20];
char nonTerminals[20];
int prodCount[20];
int n;

// FIRST & FOLLOW
char first[20][20];
char follow[20][20];

// Parsing table
char table[20][20][20];

// Terminals
char terminals[20];
int tCount = 0;

char startSymbol;

// Utility
int contains(char arr[][20], int index, char *val) {
    for (int i = 0; arr[index][i] != '\0'; i++) {
        if (arr[index][i] == val[0]) return 1;
    }
    return 0;
}

void addToSet(char arr[][20], int index, char *val) {
    if (!contains(arr, index, val)) {
        int len = strlen(arr[index]);
        arr[index][len] = val[0];
        arr[index][len + 1] = '\0';
    }
}

// ================= TERMINALS =================

void findTerminals() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < prodCount[i]; j++) {
            for (int k = 0; k < strlen(grammar[i][j]); k++) {
                char c = grammar[i][j][k];
                if (!isupper(c) && c != '#') {
                    int found = 0;
                    for (int x = 0; x < tCount; x++)
                        if (terminals[x] == c) found = 1;
                    if (!found)
                        terminals[tCount++] = c;
                }
            }
        }
    }
    terminals[tCount++] = '$';
}

// ================= FIRST =================

void computeFirst() {
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < prodCount[i]; j++) {
                char *prod = grammar[i][j];

                for (int k = 0; k < strlen(prod); k++) {
                    char c = prod[k];

                    if (!isupper(c)) {
                        char temp[2] = {c, '\0'};
                        if (!contains(first, i, temp)) {
                            addToSet(first, i, temp);
                            changed = 1;
                        }
                        break;
                    }

                    int index = c - 'A';
                    for (int x = 0; first[index][x] != '\0'; x++) {
                        if (first[index][x] != '#') {
                            char temp[2] = {first[index][x], '\0'};
                            if (!contains(first, i, temp)) {
                                addToSet(first, i, temp);
                                changed = 1;
                            }
                        }
                    }

                    if (!strchr(first[index], '#'))
                        break;

                    if (k == strlen(prod) - 1) {
                        if (!contains(first, i, "#")) {
                            addToSet(first, i, "#");
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while (changed);
}

// ================= FOLLOW =================

void computeFollow() {
    follow[startSymbol - 'A'][0] = '$';
    follow[startSymbol - 'A'][1] = '\0';

    int changed;
    do {
        changed = 0;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < prodCount[i]; j++) {
                char *prod = grammar[i][j];

                for (int k = 0; k < strlen(prod); k++) {
                    char c = prod[k];

                    if (isupper(c)) {
                        int index = c - 'A';

                        if (k + 1 < strlen(prod)) {
                            char next = prod[k + 1];

                            if (isupper(next)) {
                                int nextIndex = next - 'A';

                                for (int x = 0; first[nextIndex][x] != '\0'; x++) {
                                    if (first[nextIndex][x] != '#') {
                                        char temp[2] = {first[nextIndex][x], '\0'};
                                        if (!contains(follow, index, temp)) {
                                            addToSet(follow, index, temp);
                                            changed = 1;
                                        }
                                    }
                                }

                                if (strchr(first[nextIndex], '#')) {
                                    for (int x = 0; follow[i][x] != '\0'; x++) {
                                        char temp[2] = {follow[i][x], '\0'};
                                        if (!contains(follow, index, temp)) {
                                            addToSet(follow, index, temp);
                                            changed = 1;
                                        }
                                    }
                                }

                            } else {
                                char temp[2] = {next, '\0'};
                                if (!contains(follow, index, temp)) {
                                    addToSet(follow, index, temp);
                                    changed = 1;
                                }
                            }
                        } else {
                            for (int x = 0; follow[i][x] != '\0'; x++) {
                                char temp[2] = {follow[i][x], '\0'};
                                if (!contains(follow, index, temp)) {
                                    addToSet(follow, index, temp);
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

    } while (changed);
}

// ================= OUTPUT =================

void printFirst() {
    printf("\nFIRST SETS:\n");
    for (int i = 0; i < n; i++) {
        printf("FIRST(%c) = { ", nonTerminals[i]);
        for (int j = 0; first[i][j] != '\0'; j++)
            printf("%c ", first[i][j]);
        printf("}\n");
    }
}

void printFollow() {
    printf("\nFOLLOW SETS:\n");
    for (int i = 0; i < n; i++) {
        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for (int j = 0; follow[i][j] != '\0'; j++)
            printf("%c ", follow[i][j]);
        printf("}\n");
    }
}

// ================= MAIN =================

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);
    getchar();

    printf("Enter productions (use # for epsilon):\n");

    for (int i = 0; i < n; i++) {
        char input[50];
        fgets(input, sizeof(input), stdin);

        char lhs = input[0];
        nonTerminals[i] = lhs;
        prodCount[i] = 0;

        char *rhs = strchr(input, '>') + 1;
        rhs[strcspn(rhs, "\n")] = 0;

        char *token = strtok(rhs, "|");

        while (token != NULL) {
            strcpy(grammar[i][prodCount[i]++], token);
            token = strtok(NULL, "|");
        }
    }

    startSymbol = nonTerminals[0];

    findTerminals();
    computeFirst();
    computeFollow();

    printFirst();
    printFollow();

    return 0;
}
