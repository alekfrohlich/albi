%option noyywrap

%{
#include <stdlib.h>
#include <string.h>
#include "albi.tab.h"
%}

white               [ \t]+
digit               [0-9]
integer             {digit}+
variable            [a-zA-Z_]+
float               [+-]?{integer}(\.{integer})?

assignment          ":="

%%

{white}             { }
"include gro"       { }

{float}             { yylval.f = atof(yytext); return NUM; }
{variable}          { yylval.str = strdup(yytext); return VAR; }
{assignment}        { return ASSIGN;  }

";"                 { return SEMICOL; }
"\n"                { return EOL; }
.                   { }

%%