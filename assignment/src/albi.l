 //  
 // Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com> 
 // & Gustavo Biage <gustavo.c.biage@gmail.com>.
 // 
 // This file is a part of Albi.
 // 
 // Albi is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) any later version.
 // 
 // Albi is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 // GNU General Public License for more details.
 // 
 // You should have received a copy of the GNU General Public License along
 // with this program; if not, write to the Free Software Foundation, Inc.,
 // 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
 //

%option noyywrap yylineno

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtab.h"    // lookup, curr_env.
#include "albi.tab.h"  // tokens, yylval.

// Bison location.
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;
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
ecoli_keyword       "ecoli"
ignore              "include gro"
%%

{white}             { }
{ignore}            { }

{program_keyword}	{ 
                        env[1] = (struct symbol *) malloc(sizeof(struct symbol) * SYMTAB_SIZE);
                        curr_env = 1;
                        return PROG;
                    }
{rate_keyword}		{ return RATE; }
{sharing_keyword}   { return SHARE;  }
{ecoli_keyword}     { return ECOLI; }

{float}             { yylval.double_t = atof(yytext); return NUM; }
{variable}          { yylval.sym_tok = lookup(yytext); return VAR; }
{assignment}        { return ASSIGN;  }

","                 |
";"                 |
"+"					|
"-"					|
"*"                 |
"/"                 |
"[" 				|
"]"					|
"("                 |
")"                 |
":"					|
"{"					|
"}"					{ return yytext[0]; }

"\n"                { yycolumn = 1; }
.                   { }

%%

// Current yyin file name.
char *filename;

/**
 * Entry point.
 */
int main(int argc, char **argv)
{
    yyin = fopen(argv[1],"r");
    yyout = fopen("parsed.intermediate", "w");
    filename = argv[2];

	yyparse();
}

/**
 * Display error message and exit.
 */
int yyerror(const char *s)
{
    #define ERROR_RED(s)    fprintf(stderr, "\033[1;31m%s\033[0m", s)
    #define BEGIN_BOLD      fprintf(stderr, "\033[1;80m")
    #define COLOR_RESET     fprintf(stderr, "\033[0m")

    if (yylloc.first_line)
    {
        BEGIN_BOLD;
        fprintf(stderr, "%s:%d:%d: ", filename, yylloc.last_line,
                yylloc.last_column);
        COLOR_RESET;
    }

    ERROR_RED("error: ");
    fprintf(stderr, "%s.\n", s);
    exit(-1);
}