#include <stdio.h>
#include <ctype.h>
#include <string.h>

int isIdentifier(char str[]) {
    if (!(isalpha(str[0]) || str[0] == '_'))
        return 0;

    for (int i = 1; i < strlen(str); i++) {
        if (!(isalnum(str[i]) || str[i] == '_'))
            return 0;
    }
    return 1;
}

int isConstant(char str[]) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int isOperator(char str[]) {
    char operators[][3] = {
        "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">="
    };

    int n = sizeof(operators) / sizeof(operators[0]);

    for (int i = 0; i < n; i++) {
        if (strcmp(str, operators[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {
    char input[50];

    printf("Enter a string (press ENTER for epsilon): ");

    // Read full line to allow empty input
    fgets(input, sizeof(input), stdin);

    // Remove newline character
    input[strcspn(input, "\n")] = '\0';

    /* ---------- EPSILON CHECK ---------- */
    if (strlen(input) == 0) {
        printf("Epsilon (Empty String) Recognized\n");
        return 0;
    }

    if (isIdentifier(input))
        printf("It is an IDENTIFIER\n");
    else if (isConstant(input))
        printf("It is a CONSTANT\n");
    else if (isOperator(input))
        printf("It is an OPERATOR\n");
    else
        printf("Invalid token\n");

    return 0;
}
