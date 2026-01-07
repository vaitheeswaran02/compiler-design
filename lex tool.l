%{
#include <stdio.h>
%}

%%
"int"|"float"|"char"|"double"|"if"|"else"
        { printf("%s : keyword\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*
        { printf("%s : identifier\n", yytext); }

[0-9]+
        { printf("%s : number\n", yytext); }

"="|"+"|"-"|"*"|"/"
        { printf("%s : operator\n", yytext); }

";"
        { printf("%s : symbol\n", yytext); }

[ \t]+     ;   /* ignore spaces */

\n          { return 0; }
%%
int yywrap(){ return 1; }

int main() {
    yylex();
    return 0;
}