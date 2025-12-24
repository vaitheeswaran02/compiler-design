#include <stdio.h>
#include <ctype.h>
#include <string.h>

int isKeyword(char buffer[]) {
    char keywords[10][10] = {
        "int", "float", "if", "else", "while",
        "for", "return", "char", "double", "void"
    };

    int i;
    for (i = 0; i < 10; i++) {
        if (strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int main() {
    char ch, buffer[20];
    int i = 0;

    FILE *fp;
    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF) {

        // Check for operators
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=') {
            printf("Operator: %c\n", ch);
        }

        // Check for separators
        else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' ||
                 ch == '{' || ch == '}') {
            printf("Separator: %c\n", ch);
        }

        // Check for identifiers or keywords
        else if (isalnum(ch)) {
            buffer[i++] = ch;
        }

        // End of token
        else if ((ch == ' ' || ch == '\n') && i != 0) {
            buffer[i] = '\0';
            i = 0;

            if (isKeyword(buffer))
                printf("Keyword: %s\n", buffer);
            else
                printf("Identifier: %s\n", buffer);

                        }
    }


    fclose(fp);
    return 0;
}
