%{
#include <stdio.h>
%}

NUMPART     [0-9]+

%%

^{NUMPART}$               { printf("Integer = %d\n", atoi(yytext)); }
^{NUMPART}\.{NUMPART}$    { printf("Float = %.2f\n", atof(yytext)); }
.                         { }

%%
int main(int argc, char **argv)
{
    yylex();
}