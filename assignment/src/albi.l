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

#include "albi.tab.h"  // tokens, yylval
#include "symtab.h"    // lookup, currenv
#include "error.h"     // yyerror, YY_USER_ACTION
#include "ast.h"       // funtypes
%}

white               [ \t]+
linecomment         "//".*
digit               [0-9]
integer             {digit}+
variable            [a-zA-Z_]([a-zA-Z_0-9]+)?
invalid_id          [0-9][a-zA-Z0-9_]+
float               ([+-])?([0-9]+)|([0-9]*"."[0-9]+(E[+-][0-9]{1,2}){0,1})
assignment          ":="

program_keyword     "program"
rate_keyword        "rate"
sharing_keyword     "sharing"
ecoli_keyword       "ecoli"

builtin_sin         "sin"
builtin_cos         "cos"
builtin_tan         "tan"
builtin_ln          "ln"
builtin_log         "log"
builtin_ceil        "ceil"
builtin_floor       "floor"
builtin_sqrt        "sqrt"

ignore              ("include gro"|"lamba"|"true"|"false"|"let"|"in"|"foreach"|"do"|"end"|"maptocells"|"if"|"then"|"else"|"fun"|"head"|"tail"|"external"|"internal"|"compose"|"exec")
%%

{white}             { }
{ignore}            { }
{linecomment}       { }

{program_keyword}   { return PROG;  }
{rate_keyword}      { return RATE;  }
{sharing_keyword}   { return SHARE; }
{ecoli_keyword}     { return ECOLI; }

{builtin_sin}       { yylval.funtype = __builtin_sin;   return FUNC; }
{builtin_cos}       { yylval.funtype = __builtin_cos;   return FUNC; }
{builtin_tan}       { yylval.funtype = __builtin_tan;   return FUNC; }
{builtin_ln}        { yylval.funtype = __builtin_ln;    return FUNC; }
{builtin_log}       { yylval.funtype = __builtin_log;   return FUNC; }
{builtin_ceil}      { yylval.funtype = __builtin_ceil;  return FUNC; }
{builtin_floor}     { yylval.funtype = __builtin_floor; return FUNC; }
{builtin_sqrt}      { yylval.funtype = __builtin_sqrt;  return FUNC; }

{float}             { yylval.double_t = atof(yytext); return NUM; }
{variable}          { yylval.sym_t = lookup(yytext); return VAR;  }
{invalid_id}        { yyerror("invalid identifier '%s'", yytext); REJECT; }
{assignment}        { return ASSIGN; }

"\n"                { yycolumn = 1;     }
.                   { return yytext[0]; }

%%

/**
 * Entry point
 */
int main(int argc, char **argv)
{
    yyin = fopen(argv[1],"r");
    yyout = fopen("parsed.intermediate", "w");

    nowrites = 0;
    yycolumn = 1;

    // only displays relative via second cl argument
    if (argc == 3)
        currfilename = argv[2];
    else
        currfilename = argv[1];

    yyparse();
}