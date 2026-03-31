#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

char code[MAX][100];
int leaders[MAX];
int block_start[MAX], block_end[MAX];
int n = 0, leader_count = 0, block_count = 0;

// Function to check if a line contains "goto"
int isGoto(char *stmt) {
    return strstr(stmt, "goto") != NULL;
}

// Function to extract target line number
int getTarget(char *stmt) {
    char *ptr = strstr(stmt, "goto");
    if (ptr != NULL) {
        return atoi(ptr + 5) - 1;
    }
    return -1;
}

// Check if already leader
int isLeader(int x) {
    int i;
    for (i = 0; i < leader_count; i++) {
        if (leaders[i] == x)
            return 1;
    }
    return 0;
}

// Add leader
void addLeader(int x) {
    if (!isLeader(x)) {
        leaders[leader_count++] = x;
    }
}

// Sort leaders
void sortLeaders() {
    int i, j, temp;
    for (i = 0; i < leader_count - 1; i++) {
        for (j = i + 1; j < leader_count; j++) {
            if (leaders[i] > leaders[j]) {
                temp = leaders[i];
                leaders[i] = leaders[j];
                leaders[j] = temp;
            }
        }
    }
}

// Find block number for a line
int findBlock(int line) {
    int i;
    for (i = 0; i < block_count; i++) {
        if (line >= block_start[i] && line < block_end[i])
            return i;
    }
    return -1;
}

int main() {
    int i, j;

    printf("Enter number of statements: ");
    scanf("%d", &n);
    getchar();

    printf("Enter Three Address Code:\n");
    for (i = 0; i < n; i++) {
        fgets(code[i], 100, stdin);
        code[i][strcspn(code[i], "\n")] = 0;
    }

    // Step 1: Identify leaders
    addLeader(0);

    for (i = 0; i < n; i++) {
        if (isGoto(code[i])) {
            int target = getTarget(code[i]);

            if (target >= 0)
                addLeader(target);

            if (i + 1 < n)
                addLeader(i + 1);
        }
    }

    sortLeaders();

    // Step 2: Form basic blocks
    for (i = 0; i < leader_count; i++) {
        block_start[i] = leaders[i];

        if (i + 1 < leader_count)
            block_end[i] = leaders[i + 1];
        else
            block_end[i] = n;

        block_count++;
    }

    // Print Basic Blocks
    printf("\nBasic Blocks:\n");
    for (i = 0; i < block_count; i++) {
        printf("\nB%d:\n", i + 1);
        for (j = block_start[i]; j < block_end[i]; j++) {
            printf(" %d. %s\n", j + 1, code[j]);
        }
    }

    // Step 3: Flow Graph
    printf("\nFlow Graph (Adjacency List):\n");

    for (i = 0; i < block_count; i++) {
        printf("B%d -> ", i + 1);

        char *last = code[block_end[i] - 1];

        if (isGoto(last)) {
            int target = getTarget(last);
            int targetBlock = findBlock(target);

            if (targetBlock != -1)
                printf("B%d ", targetBlock + 1);

            if (strstr(last, "if") != NULL && i + 1 < block_count) {
                printf("B%d", i + 2);
            }
        } else {
            if (i + 1 < block_count)
                printf("B%d", i + 2);
        }

        printf("\n");
    }

    return 0;
}

Input 
7
i = 1
sum = 0
if i > 10 goto 7
sum = sum + i
i = i + 1
goto 3
print sum

Execution
gcc flowgraph.c -o flowgraph
./flowgraph
