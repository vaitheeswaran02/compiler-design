%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

[a-zA-Z][a-zA-Z0-9]*   { yylval.str = strdup(yytext); return ID; }

";"                    { return ';'; }

\n                     { return 0; }

"+"                    { return '+'; }

"\""                   { return '"'; }

"("                    { return '('; }

")"                    { return ')'; }

.                      { return yytext[0]; }

%%

int yywrap() {
    return 1;
}