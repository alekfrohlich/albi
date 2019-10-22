/*	  
 *    Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com> 
 *    & Gustavo Biage <gustavo.c.biage@gmail.com>.
 *
 * 	  This file is a part of Albi.
 * 
 *    Albi is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Albi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "../include/ast.h"
#include "../include/symtab.h"
#include "../include/parsing.h"
%}

%union {
    struct ast *node;
    double double_t;
    struct symbol *sym_tok;   // which symbol?  
    struct symlist *symlist_tok;
	struct assignlist *assigns;
	struct explist * expressions;
	struct progcall *call_params;
}

// declare tokens.
%token <double_t> NUM
%token <sym_tok> VAR
%token ASSIGN PROG RATE SHARE ECOLI

%right '='
%left '+' '-'
%left '*' '/'

%type <node> exp statement list assignment ecoli
%type <symlist_tok> symlist
%type <assigns> assignment_list
%type <call_params> proglist
%type <expressions> explist

%%

start_of_prog: %empty
| start_of_prog assignment
									{
										//genmodel();
										treefree($2);
									}
| start_of_prog program
									{
										curr_env = 0;
									}
| start_of_prog ecoli				
									{
										//genmodel();
										treefree($2);
									}
;

assignment: VAR ASSIGN exp ';'		
									{
										$$ = newassign($1, $3);
									}
;

exp: exp '+' exp					
									{
										$$ = newast(PLUS, $1, $3);
									}
| exp '-' exp						
									{
										$$ = newast(MINUS, $1, $3);
									}
| exp '*' exp						
									{
										$$ = newast(TIMES, $1, $3);
									}
| exp '/' exp						
									{
										$$ = newast(DIV, $1, $3);
									}
| NUM								
									{
										$$ = newnum($1);
									}
| VAR								
									{
										$$ = newref($1);
									}
| %empty							
									{
										$$ = NULL;
									}
;

program: PROG VAR '(' symlist ')' ASSIGN '{' list '}' ';'
									{
										progdef($2, $4, $8);
									}
| PROG VAR '(' ')' ASSIGN '{' list '}' ';'
									{
										progdef($2, NULL, $7);
									}
;

symlist: VAR
									{
										$$ = newsymlist($1, NULL);
									}
| VAR ',' symlist              
									{
										$$ = newsymlist($1, $3);
									}
;

list: %empty
									{
										$$ = NULL;
									}
| statement list
									{
										if ($2 == NULL)
											$$ = $1;
										else
											$$ = newast(EXPLIST, $1, $2);
									}
;

statement: assignment
| RATE '(' exp ')' ':' '{' assignment_list '}'
									{
										// $$ = newrate($3, $7);
									}
;

assignment_list: %empty
									{
										$$ = NULL;
									}
| assignment assignment_list
									{
										// if ($2 == NULL)
										// 	$$ = newassignlist((struct symassign *) $1, NULL);
										// else
										// 	$$ = newassignlist((struct symassign *) $1, $2);
									}
;

ecoli: ECOLI '(' '[' ']' ',' PROG proglist ')' ';'
									{
										$$ = newcompart("ECOLI", $7);
									}
;

proglist: VAR '(' explist ')' 
									{ 
										$$ = (struct progcall *) malloc(sizeof(struct progcall)); 
										$$->list = NULL;
										$$->sym = $1;
										$$->next = NULL;
										$$->exp = (struct explist*) $3;
									}
| VAR '(' explist ')' SHARE symlist 
									{
										$$ = (struct progcall *) malloc(sizeof(struct progcall)); 
										$$->list = $6;
										$$->sym = $1;
										$$->next = NULL;
										$$->exp = (struct explist*) $3;
									}
| VAR '(' explist ')' '+' proglist 
									{ 
										$$ = (struct progcall *) malloc(sizeof(struct progcall)); 
										$$->list = NULL;
										$$->sym = $1;
										$$->next = $6;
										$$->exp = (struct explist*) $3;
									}
| VAR '(' explist ')' SHARE symlist '+' proglist 
									{ 
										$$ = (struct progcall *) malloc(sizeof(struct progcall)); 
										$$->list = $6;
										$$->sym = $1;
										$$->next = $8;
										printf("is null = %d\n", $3 == NULL);
										$$->exp = $3;
									}
;

explist: exp 						{
										if ($1 != NULL) {
											$$ = newexplist($1, NULL);
										} else {
											$$ = NULL;
										}
									}
| exp ',' explist
									{
										$$ = newexplist($1, $3);
									}
;
%%