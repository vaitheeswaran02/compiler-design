#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 50

int memory[MEMORY_SIZE];

int SP = MEMORY_SIZE;   // Stack Pointer starts from top
int HP = 0;             // Heap Pointer starts from bottom

void displayMemory()
{
    printf("\n================ MEMORY LAYOUT ================\n");
    printf("+--------+--------+------------+\n");
    printf("| Index  | Value  | Segment    |\n");
    printf("+--------+--------+------------+\n");

    for(int i = 0; i < MEMORY_SIZE; i++)
    {
        char segment[10];

        if(i < HP)
            strcpy(segment, "Heap");
        else if(i >= SP)
            strcpy(segment, "Stack");
        else
            strcpy(segment, "Free");

        printf("| %-6d | %-6d | %-10s |\n", i, memory[i], segment);
    }

    printf("+--------+--------+------------+\n");
    printf("Heap Pointer (HP): %d\n", HP);
    printf("Stack Pointer (SP): %d\n", SP);
}

void pushStack(int value)
{
    if(SP - 1 == HP)
    {
        printf("Stack-Heap Collision! Cannot push.\n");
        return;
    }

    SP--;
    memory[SP] = value;

    printf("\nPushed %d into stack\n", value);
    displayMemory();
}

void popStack()
{
    if(SP == MEMORY_SIZE)
    {
        printf("Stack Underflow\n");
        return;
    }

    printf("\nPopped %d from stack\n", memory[SP]);
    memory[SP] = 0;   // clear value (better view)
    SP++;

    displayMemory();
}

void allocateHeap(int value)
{
    if(HP + 1 == SP)
    {
        printf("Stack-Heap Collision! Cannot allocate heap.\n");
        return;
    }

    memory[HP] = value;
    printf("\nAllocated %d in heap\n", value);
    HP++;

    displayMemory();
}

void freeHeap()
{
    if(HP == 0)
    {
        printf("Heap Empty\n");
        return;
    }

    HP--;
    printf("\nFreed heap value %d\n", memory[HP]);
    memory[HP] = 0;

    displayMemory();
}

void recursiveCall(int n)
{
    if(n == 0)
        return;

    pushStack(n);
    recursiveCall(n-1);
    popStack();
}

int main()
{
    int choice, value;

    printf("Runtime Stack and Heap Management Simulation\n");

    while(1)
    {
        printf("\n1. Push Stack\n");
        printf("2. Pop Stack\n");
        printf("3. Allocate Heap\n");
        printf("4. Free Heap\n");
        printf("5. Simulate Recursive Call\n");
        printf("6. Display Memory\n");
        printf("7. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);
                pushStack(value);
                break;

            case 2:
                popStack();
                break;

            case 3:
                printf("Enter value: ");
                scanf("%d", &value);
                allocateHeap(value);
                break;

            case 4:
                freeHeap();
                break;

            case 5:
                printf("Enter recursion depth: ");
                scanf("%d", &value);
                recursiveCall(value);
                break;

            case 6:
                displayMemory();
                break;

            case 7:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
