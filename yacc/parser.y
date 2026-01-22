%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
void yyerror(const char *s);
%}

%token ID NUM ASSIGN PLUS SEMI

%%
program:
        stmt            { printf("Valid syntax\n"); }
        ;

stmt:
        ID ASSIGN expr SEMI
        ;

expr:
        ID
      | NUM
      | expr PLUS expr
      ;
%%

/* ERROR FUNCTION */
void yyerror(const char *s) {
    printf("Syntax error\n");
}

/* MAIN FUNCTION */
int main() {
    printf("Enter one line statement:\n");
    yyparse();
    return 0;
}
