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
program_keyword		"program "

%%

{white}             { }
"include gro"       { }

{float}             { yylval.f = atof(yytext); return NUM; }
{variable}          { yylval.str = strdup(yytext); return VAR; }
{assignment}        { return ASSIGN;  }
{program_keyword}	{ return PROG; }
";"                 { return SEMICOL; }

"{"					{ return OPEN_BODY; }
"}"					{ return CLOSE_BODY; }
"\n"                { return EOL; }
.                   { }

%%