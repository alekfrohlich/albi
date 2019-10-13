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
#include "ast.h"
#include "ast.c"

extern int yyerror(const char* s);
extern int yylex(void);
%}

%union {
    char * str;
    float f;
    struct node * node_type;
}

%token <f> NUM
%token <str> VAR
%token ASSIGN EOL PROG RATE

%type <node_type> start_of_prog program program_body program_content_list parameter specie statement rate rate_body reagent_list;
%type <str> program_def product reactant rate_def

%%

start_of_prog: 					
								{
									head = new_node(START_OF_PROG_TYPE, NULL, NULL); 
									$$ = head; 
								}
				
| start_of_prog parameter EOL 	
								{
									$$ = new_node(START_OF_PROG_TYPE, $1, NULL); 
									$$->right = $2;
							 	}

| start_of_prog program EOL 	
								{
									$$ = new_node(START_OF_PROG_TYPE, $1, NULL);
									$$->right = $2;
								}
;

program_def:
 PROG VAR ASSIGN 				
 								{
	 								$$ = $2;
								}
;


program :
 program_def program_body ';' 
 								{
									$$ = $2;
									$$->compartment = $1;
								}
;

program_content_list : specie 
								{
									$$ = $1;
								}
| rate							
								{
									$$ = $1;
								}
| program_content_list specie
								{
									$$ = $2;
									$$->right = $1;
								}
| program_content_list rate
								{
									$$ = $2;
									$$ ->right = $1;
								}
;

program_body: '{' program_content_list '}' 
								{
									$$ = $2;
								}
;

statement:
 VAR ASSIGN NUM ';' 
 								{ 
									$$ = new_node(STATEMENT_TYPE, NULL, NULL);
									$$->var = $1;
									$$->value = $3;
								}
;

rate_def :
RATE '(' VAR ')' ':' 
								{ 
									$$ = $3; 
								}
;

reagent_list: 
product 
								{
									$$ = new_node(RATE_TYPE, NULL, NULL);
									$$->n_prod = 1;
									$$->n_reac = 0;
									$$->prod = (char **) malloc(sizeof(char *)*10);
									$$->reac = (char **) malloc(sizeof(char *)*10);
									$$->prod[0] = $1;
								}
| reactant
								{
									$$ = new_node(RATE_TYPE, NULL, NULL);
									$$->n_prod = 0;
									$$->n_reac = 1;
									$$->prod = (char **) malloc(sizeof(char *)*10);
									$$->reac = (char **) malloc(sizeof(char *)*10);
									$$->reac[0] = $1;
								}
| reagent_list product
								{
									$$ = $1;
									$$->prod[$$->n_prod] = $2;
									$$->n_prod++;
								}
| reagent_list reactant
								{
									$$ = $1;
									$$->reac[$$->n_reac] = $2;
									$$->n_reac++;
								}
;

rate_body :
'{' reagent_list '}'
								{
									$$ = $2;
								}
;

rate : 
rate_def rate_body
								{
									$$ = $2;
									$$->var = $1;
								}
;

reactant :
VAR ASSIGN VAR '-' NUM ';' 
							{
								if (strcmp($1, $3) == 0) {
									$$ = $1;
								} else {
									yyerror("invalid rate body.\n");
									return 1;
								}
							}
;

product :
VAR ASSIGN VAR '+' NUM ';' 
							{
								if (strcmp($1, $3) == 0) {
									$$ = $1;
								} else {
									yyerror("invalid rate body.\n");
									return 1;
								}
							}
;

parameter :
 statement
 							{
 								$1->type = PARAMETER_TYPE;
								$$ = $1;
 							}
;

specie :
 statement
 							{
								$1->type = SPECIE_TYPE;
								$$ = $1;
 							}
;

%%