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
program_keyword		"program"
rate_keyword		"rate"
%%

{white}             { }
"include gro"       { }
{program_keyword}	{ return PROG; }
{rate_keyword}		{ return RATE; }

{float}             { yylval.f = atof(yytext); return NUM; }
{variable}          { yylval.str = strdup(yytext); return VAR; }
{assignment}        { return ASSIGN;  }
";"                 { return SEMICOL; }
"+"					{ return ADD; }
"-"					{ return SUB; }
"[" 				{ return OPEN_PAR; }
"]"					{ return CLOSE_PAR; }
":"					{ return DOUBLEDOT; }
"{"					{ return OPEN_BODY; }
"}"					{ return CLOSE_BODY; }
"\n"                { return EOL; }
.                   { }

%%