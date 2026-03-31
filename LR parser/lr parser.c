#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Grammar
char grammar[20][20][20];
char nonTerminals[20];
char terminals[20];
int prodCount[20];
int n, tCount = 0, ntCount = 0;

// States
char states[50][50][50];
int stateCount = 0;

// ACTION & GOTO
char action[50][20][20];
int goToTable[50][20];

char startSymbol;
char augmentedStart = 'Z';

// ================= UTILITY =================

int containsState(char state[][50], int size, char items[][50], int itemCount) {
    for (int i = 0; i < size; i++) {
        int match = 1;
        for (int j = 0; j < itemCount; j++) {
            if (strcmp(state[i], items[j]) != 0) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

// ================= CLOSURE =================

void closure(char items[][50], int *count) {

    int changed;

    do {
        changed = 0;

        for (int i = 0; i < *count; i++) {

            char *item = items[i];
            char *dot = strchr(item, '.');

            if (dot && *(dot + 1)) {

                char symbol = *(dot + 1);

                if (isupper(symbol)) {

                    for (int p = 0; p < n; p++) {
                        if (nonTerminals[p] == symbol) {

                            for (int k = 0; k < prodCount[p]; k++) {

                                char newItem[50];
                                sprintf(newItem, "%c->.%s", symbol, grammar[p][k]);

                                int exists = 0;
                                for (int x = 0; x < *count; x++)
                                    if (strcmp(items[x], newItem) == 0)
                                        exists = 1;

                                if (!exists) {
                                    strcpy(items[*count], newItem);
                                    (*count)++;
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

// ================= GOTO =================

void goToFunc(char state[][50], int count, char symbol,
              char result[][50], int *resCount) {

    *resCount = 0;

    for (int i = 0; i < count; i++) {

        char *dot = strchr(state[i], '.');

        if (dot && *(dot + 1) == symbol) {

            char newItem[50];
            int pos = dot - state[i];

            strncpy(newItem, state[i], pos);
            newItem[pos] = symbol;
            newItem[pos + 1] = '.';
            strcpy(newItem + pos + 2, dot + 2);

            strcpy(result[(*resCount)++], newItem);
        }
    }

    closure(result, resCount);
}

// ================= CANONICAL =================

void buildStates() {

    char start[50][50];
    int count = 1;

    sprintf(start[0], "%c->.%c", augmentedStart, startSymbol);

    closure(start, &count);

    for (int i = 0; i < count; i++)
        strcpy(states[stateCount][i], start[i]);

    stateCount++;

    for (int i = 0; i < stateCount; i++) {

        char symbols[50];
        int symCount = 0;

        for (int j = 0; j < 50; j++) {
            if (strlen(states[i][j]) == 0) break;

            char *dot = strchr(states[i][j], '.');
            if (dot && *(dot + 1)) {
                symbols[symCount++] = *(dot + 1);
            }
        }

        for (int s = 0; s < symCount; s++) {

            char newState[50][50];
            int newCount;

            goToFunc(states[i], 50, symbols[s], newState, &newCount);

            if (newCount > 0) {
                for (int k = 0; k < newCount; k++)
                    strcpy(states[stateCount][k], newState[k]);
                stateCount++;
            }
        }
    }
}

// ================= TABLE =================

void buildTable() {

    for (int i = 0; i < stateCount; i++) {

        for (int t = 0; t < tCount; t++)
            strcpy(action[i][t], "-");

        for (int nt = 0; nt < ntCount; nt++)
            goToTable[i][nt] = -1;
    }

    for (int i = 0; i < stateCount; i++) {

        for (int j = 0; j < 50; j++) {

            if (strlen(states[i][j]) == 0) break;

            char *item = states[i][j];
            char *dot = strchr(item, '.');

            if (dot && *(dot + 1)) {

                char sym = *(dot + 1);

                if (!isupper(sym)) {
                    sprintf(action[i][0], "S%d", i + 1);
                }

            } else {
                if (item[0] == augmentedStart) {
                    strcpy(action[i][tCount - 1], "ACC");
                } else {
                    for (int t = 0; t < tCount; t++)
                        sprintf(action[i][t], "R(%s)", item);
                }
            }
        }
    }
}

// ================= PRINT =================

void printTable() {

    printf("\nLR(0) PARSING TABLE:\n");

    printf("State\t");
    for (int t = 0; t < tCount; t++)
        printf("%c\t", terminals[t]);

    printf("\n");

    for (int i = 0; i < stateCount; i++) {

        printf("%d\t", i);

        for (int t = 0; t < tCount; t++)
            printf("%s\t", action[i][t]);

        printf("\n");
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
        nonTerminals[ntCount++] = lhs;

        char *rhs = strchr(input, '>') + 1;
        rhs[strcspn(rhs, "\n")] = 0;

        char *token = strtok(rhs, "|");

        while (token != NULL) {
            strcpy(grammar[i][prodCount[i]++], token);

            for (int k = 0; k < strlen(token); k++) {
                if (!isupper(token[k]) && token[k] != '#')
                    terminals[tCount++] = token[k];
            }

            token = strtok(NULL, "|");
        }
    }

    terminals[tCount++] = '$';

    startSymbol = nonTerminals[0];

    // Add augmented grammar
    strcpy(grammar[n][0], "");
    grammar[n][0][0] = startSymbol;
    prodCount[n++] = 1;
    nonTerminals[ntCount++] = augmentedStart;

    buildStates();
    buildTable();
    printTable();

    return 0;
}


output:
Enter number of productions: 3
Enter productions (use # for epsilon):
S->CC
C->cC|d

Enter input string (end with $): ccd$
