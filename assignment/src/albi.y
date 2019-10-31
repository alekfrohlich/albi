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

%locations

%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "symtab.h"
#include "parsing.h"
#include "program.h"

extern int yylex();
%}

%union {
    // Exported by Flex
	double double_t;				// Double
    struct symbol *sym_t;   		// Symbol

	// Intermediate structures
    struct ast *ast;				// AST node
	struct progcall *progcall;		// Compartment creation node
	struct nodelist *nodelist;		// List of AST nodes
	struct symlist *symlist;		// List of symbols
}

// Token types
%token <double_t> NUM
%token <sym_t> VAR
%token PROG RATE SHARE ECOLI
%token ASSIGN ":="

// Precendences and associativity
%right '='
%left '+' '-'
%left '*' '/'

// Rule types
%type <sym_t> progdef
%type <ast> exp statement assignment ecoli
%type <nodelist> assignlist explist stmtlist
%type <symlist> symlist
%type <progcall> progcall

%%

start_of_prog: %empty
| start_of_prog assignment
									{
										struct symassign *assign = (struct symassign *) $2;
										symdef(assign->sym, assign->val);
										genparam(assign->sym->name, assign->val);
									}
| start_of_prog program
									{
										currenv = 0;
									}
| start_of_prog ecoli
									{
										gencompart((struct compart *) $2);
									}
;

assignment: VAR ":=" exp ';'
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
;

program: progdef '(' symlist ')' ":=" '{' stmtlist '}' ';'
									{
										progdef($1, $3, $7);
									}
| progdef '(' ')' ":=" '{' stmtlist '}' ';'
									{
										progdef($1, NULL, $6);
									}
;

progdef: PROG VAR
									{
										$$ = $2;
										env[1] = (struct symbol *) malloc(sizeof(struct symbol) * SYMTAB_SIZE);
										currenv = 1;
									}
;

symlist: VAR
									{
										$$ = newslist($1, NULL);
									}
| VAR ',' symlist
									{
										$$ = newslist($1, $3);
									}
;

stmtlist: %empty
									{
										$$ = NULL;
									}
| statement stmtlist
									{
										if ($2 == NULL)
											$$ = newnodelist($1, NULL);
										else
											$$ = newnodelist($1, $2);
									}
;

statement: assignment
| RATE '(' exp ')' ':' '{' assignlist '}'
									{
										$$ = newrate($3, $7);
									}
;

assignlist: %empty
									{
										$$ = NULL;
									}
| assignment assignlist
									{
										if ($2 == NULL)
											$$ = newnodelist($1, NULL);
										else
										 	$$ = newnodelist($1, $2);
									}
;

ecoli: ECOLI '(' '[' ']' ',' PROG progcall ')' ';'
									{
										$$ = newcompart("ECOLI", $7);
									}
;

progcall: VAR '(' explist ')'
									{
										$$ = newprogcall(
										    $1,
										    NULL,
										    (struct nodelist*) $3,
										    NULL);
									}
| VAR '(' explist ')' SHARE symlist
									{
										$$ = newprogcall(
										    $1,
										    $6,
										    (struct nodelist*) $3,
										    NULL);
									}
| VAR '(' explist ')' '+' progcall
									{
										$$ = newprogcall(
										    $1,
										    NULL,
										    (struct nodelist*) $3,
										    $6);
									}
| VAR '(' explist ')' SHARE symlist '+' progcall
									{
										$$ = newprogcall(
										    $1,
										    $6,
										    $3,
										    $8);
									}
| VAR '('')'
									{
										$$ = newprogcall(
										    $1,
										    NULL,
										    NULL,
										    NULL);
									}
| VAR '('')' SHARE symlist
									{
										$$ = newprogcall(
										    $1,
										    $5,
										    NULL,
										    NULL);
									}
| VAR '(' ')' '+' progcall
									{
										$$ = newprogcall(
										    $1,
										    NULL,
										    NULL,
										    $5);
									}
| VAR '(' ')' SHARE symlist '+' progcall
									{
										$$ = newprogcall(
										    $1,
										    $5,
										    NULL,
										    $7);
									}
;

explist: exp 						{
										if ($1 != NULL) {
											$$ = newnodelist($1, NULL);
										} else {
											$$ = NULL;
										}
									}
| exp ',' explist
									{
										$$ = newnodelist($1, $3);
									}
;
%%