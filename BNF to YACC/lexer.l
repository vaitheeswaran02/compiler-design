%{
#include "parser.tab.h"
%}

%%

[a-zA-Z]    { return ID; }
[ \t]       ;        // ignore spaces
\n          { return 0; }
.           { return yytext[0]; }

%%

int yywrap()
{
    return 1;
}
