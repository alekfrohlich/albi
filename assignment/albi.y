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

%type <node_type> start_of_prog program program_body program_content_list parameter specie statement
%type <str> program_def

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
									//printf("compartment %s;\n",  $2);
									//printf("%s = 1.0;\n", $2);
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
									$$ = new_node(RATE_TYPE, NULL, NULL);
								}
| program_content_list specie
								{
									$$ = new_node(SPECIE_TYPE, NULL, $1); 
									$$->right = $1;
								}
| program_content_list rate
								{
									$$ = new_node(RATE_TYPE, NULL, NULL); 
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
									printf("rate definition\n"); 
								}
;

reagent_list: 
reagent
| reagent_list reagent
;

reagent: 
product
| reactant
;

rate_body :
'{' reagent_list '}'
;

rate : 
rate_def rate_body
;

reactant :
VAR ASSIGN VAR '-' NUM ';' 
							{
								printf("%s--\n", $1);
							}
;

product :
VAR ASSIGN VAR '+' NUM ';' 
							{
								printf("%s++\n", $1);
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