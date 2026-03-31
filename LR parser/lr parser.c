#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

// Grammar
char grammar[20][20][20];
char nonTerminals[20];
char terminals[20];
int prodCount[20];
int n, tCount = 0, ntCount = 0;

// States
char states[50][50][50];
int stateItemCount[50];
int stateCount = 0;

// ACTION & GOTO
char action[50][20][20];
int goToTable[50][20];

// Stack
int stack[100], top = -1;

// Start symbols
char startSymbol;
char augmentedStart = 'Z';

// ================= UTILITY =================

int containsItem(char items[][50], int count, char *str) {
    for (int i = 0; i < count; i++)
        if (strcmp(items[i], str) == 0)
            return 1;
    return 0;
}

// ================= CLOSURE =================

void closure(char items[][50], int *count) {

    int changed;

    do {
        changed = 0;

        for (int i = 0; i < *count; i++) {

            char *dot = strchr(items[i], '.');

            if (dot && *(dot + 1)) {

                char symbol = *(dot + 1);

                if (isupper(symbol)) {

                    for (int p = 0; p < ntCount; p++) {

                        if (nonTerminals[p] == symbol) {

                            for (int k = 0; k < prodCount[p]; k++) {

                                char newItem[50];
                                sprintf(newItem, "%c->.%s", symbol, grammar[p][k]);

                                if (!containsItem(items, *count, newItem)) {
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

// ================= CANONICAL COLLECTION =================

int stateExists(char newState[][50], int newCount) {
    for (int i = 0; i < stateCount; i++) {
        if (stateItemCount[i] != newCount) continue;

        int match = 1;
        for (int j = 0; j < newCount; j++) {
            if (!containsItem(states[i], stateItemCount[i], newState[j])) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

void buildCanonicalCollection() {

    char startState[50][50];
    int count = 1;

    sprintf(startState[0], "%c->.%c", augmentedStart, startSymbol);
    closure(startState, &count);

    for (int i = 0; i < count; i++)
        strcpy(states[0][i], startState[i]);

    stateItemCount[0] = count;
    stateCount = 1;

    for (int i = 0; i < stateCount; i++) {

        char symbols[50];
        int symCount = 0;

        for (int j = 0; j < stateItemCount[i]; j++) {

            char *dot = strchr(states[i][j], '.');
            if (dot && *(dot + 1)) {

                char sym = *(dot + 1);
                int found = 0;

                for (int k = 0; k < symCount; k++)
                    if (symbols[k] == sym) found = 1;

                if (!found)
                    symbols[symCount++] = sym;
            }
        }

        for (int s = 0; s < symCount; s++) {

            char newState[50][50];
            int newCount;

            goToFunc(states[i], stateItemCount[i], symbols[s], newState, &newCount);

            int exists = stateExists(newState, newCount);

            if (newCount > 0 && exists == -1) {

                for (int k = 0; k < newCount; k++)
                    strcpy(states[stateCount][k], newState[k]);

                stateItemCount[stateCount] = newCount;
                stateCount++;
            }
        }
    }
}

// ================= TABLE =================

void buildParsingTable() {

    for (int i = 0; i < stateCount; i++) {

        for (int t = 0; t < tCount; t++)
            strcpy(action[i][t], "-");

        for (int nt = 0; nt < ntCount; nt++)
            goToTable[i][nt] = -1;
    }

    for (int i = 0; i < stateCount; i++) {

        for (int j = 0; j < stateItemCount[i]; j++) {

            char *item = states[i][j];
            char *dot = strchr(item, '.');

            // SHIFT
            if (dot && *(dot + 1)) {

                char sym = *(dot + 1);

                char nextState[50][50];
                int newCount;

                goToFunc(states[i], stateItemCount[i], sym, nextState, &newCount);

                int index = stateExists(nextState, newCount);

                if (!isupper(sym)) {
                    int tIndex = -1;
                    for (int t = 0; t < tCount; t++)
                        if (terminals[t] == sym) tIndex = t;

                    if (tIndex != -1)
                        sprintf(action[i][tIndex], "S%d", index);
                } else {
                    for (int nt = 0; nt < ntCount; nt++)
                        if (nonTerminals[nt] == sym)
                            goToTable[i][nt] = index;
                }
            }

            // REDUCE
            else {

                if (item[0] == augmentedStart) {
                    for (int t = 0; t < tCount; t++)
                        if (terminals[t] == '$')
                            strcpy(action[i][t], "ACC");
                } 
                else {

                    // 🔥 FIX: remove dot
                    char prod[50];
                    strcpy(prod, item);

                    for (int x = 0; prod[x]; x++) {
                        if (prod[x] == '.') {
                            for (int y = x; prod[y]; y++)
                                prod[y] = prod[y + 1];
                            break;
                        }
                    }

                    for (int t = 0; t < tCount; t++) {
                        if (strcmp(action[i][t], "-") == 0) {
                            sprintf(action[i][t], "R(%s)", prod);
                        }
                    }
                }
            }
        }
    }
}

// ================= PRINT =================

void printTable() {

    printf("\nLR(0) PARSING TABLE:\nState\t");

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

// ================= PARSER =================

void parse(char input[]) {

    stack[++top] = 0;
    int i = 0;

    printf("\nSTACK\tINPUT\tACTION\n");

    while (1) {

        int state = stack[top];
        char curr = input[i];

        int tIndex = -1;
        for (int t = 0; t < tCount; t++)
            if (terminals[t] == curr) tIndex = t;

        char *act = (tIndex != -1) ? action[state][tIndex] : NULL;

        printf("[");
        for (int k = 0; k <= top; k++)
            printf("%d ", stack[k]);
        printf("]\t%s\t%s\n", input + i, act);

        if (act == NULL || strcmp(act, "-") == 0) {
            printf("REJECT\n");
            return;
        }

        if (strcmp(act, "ACC") == 0) {
            printf("ACCEPT\n");
            return;
        }

        if (act[0] == 'S') {
            int next = atoi(act + 1);
            stack[++top] = next;
            i++;
        }

        else if (act[0] == 'R') {

            char lhs = act[2];
            char rhs[20];
            strcpy(rhs, act + 5);
            rhs[strlen(rhs) - 1] = '\0';

            if (strcmp(rhs, "#") != 0)
                for (int j = 0; j < strlen(rhs); j++)
                    top--;

            int currState = stack[top];

            int ntIndex = -1;
            for (int nt = 0; nt < ntCount; nt++)
                if (nonTerminals[nt] == lhs) ntIndex = nt;

            int nextState = goToTable[currState][ntIndex];

            if (nextState == -1) {
                printf("REJECT\n");
                return;
            }

            stack[++top] = nextState;
        }
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

    // Augmented grammar
    grammar[n][0][0] = startSymbol;
    grammar[n][0][1] = '\0';
    nonTerminals[ntCount++] = augmentedStart;
    prodCount[n++] = 1;

    buildCanonicalCollection();
    buildParsingTable();
    printTable();

    char input[50];
    printf("\nEnter input string (end with $): ");
    scanf("%s", input);

    parse(input);

    return 0;
}
