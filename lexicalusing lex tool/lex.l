%{
#include <stdio.h>
%}

%%
"int"|"float"|"if"|"else"|"while"|"return"   { printf("%s : Keyword\n", yytext); }

[0-9]+                                      { printf("%s : Constant\n", yytext); }

[a-zA-Z][a-zA-Z0-9]*                        { printf("%s : Identifier\n", yytext); }

[+\-*/=<>]                                  { printf("%s : Operator\n", yytext); }

[;,(){}]                                    { printf("%s : Special Symbol\n", yytext); }

[ \t\n]+                                    ;   // Ignore whitespace

.                                           { printf("Unknown : %s\n", yytext); }
%%

int main() {
    printf("Enter input:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

run:
flex filename.lex
gcc lex.yy.c -o filename
./filename
