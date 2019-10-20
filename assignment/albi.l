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

%option noyywrap yylineno

%{
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
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
sharing_keyword     "sharing"
ecolli_keyword      "ecolli"
ignore              "include gro"
%%

"\n"                { } 
{white}             { }
{ignore}            { }

{program_keyword}	{ 
                        env[1] = (struct symbol *) malloc(sizeof(struct symbol) * SYMTAB_SIZE);
                        curr_env = 1;
                        return PROG;
                    }
{rate_keyword}		{ return RATE; }
{sharing_keyword}   { return SHARE;  }
{ecolli_keyword}    { return ECOLLI; }

{float}             { yylval.double_t = atof(yytext); return NUM; }
{variable}          { yylval.sym_tok = lookup(yytext); return VAR; }
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
    fprintf(stderr, "error: %s on line %d\n", s, yylineno);
}