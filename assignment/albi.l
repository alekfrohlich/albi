	//   
    //  Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com> 
    //  & Gustavo Biage <gustavo.c.biage@gmail.com>.
    //  
 	//  This file is a part of Albi.
    //  
    //  Albi is free software; you can redistribute it and/or modify
    //  it under the terms of the GNU General Public License as published by
    //  the Free Software Foundation; either version 2 of the License, or
    //  (at your option) any later version.
    //  
    //  Albi is distributed in the hope that it will be useful,
    //  but WITHOUT ANY WARRANTY; without even the implied warranty of
    //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    //  GNU General Public License for more details.
    //  
    //  You should have received a copy of the GNU General Public License along
    //  with this program; if not, write to the Free Software Foundation, Inc.,
    //  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
    //

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
set_keyword         "set"
ignore              "include gro"
%%

"\n"                { } 
{white}             { }
{ignore}            { }

{program_keyword}	{ return PROG; }
{rate_keyword}		{ return RATE; }
{set_keyword}       { return SET;  }

{float}             { yylval.f = atof(yytext); return NUM; }
{variable}          { yylval.str = strdup(yytext); return VAR; }
{assignment}        { return ASSIGN;  }

","                 |
";"                 |
"+"					|
"-"					|
"[" 				|
"]"					|
"("                 |
")"                 |
":"					|
"{"					|
"}"					{ return yytext[0]; }

.                   { }

%%

/**
 * Entry point.
 */
int main(int argc, char **argv)
{
    yyin = fopen(argv[1],"r");
    yyout = fopen("parsed.intermediate", "w");
    
	yyparse();
}

int yyerror(const char *s)
{
    fprintf(stderr, "erro: %s\n", s);
}