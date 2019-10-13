%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_OF_PROG_TYPE 1
#define RATE_TYPE 2
#define PARAMETER_TYPE 3
#define SPECIE_TYPE 4
#define STATEMENT_TYPE 5

int yyerror(const char* s);
int yylex(void);

struct node {
	int type;
	struct node * left;
	struct node * right;
	
	char * compartment;
	char * var;
	char ** prod;
	char ** reac;
	int n_prod, n_reac;
	float value;
};

void search_program(struct node * node, char * compartment);
void search_tree(struct node * node);
struct node * new_node(int type, struct node * left_of, struct node * right_of);

struct node * head;

/**
 * Construct new AST node.
 */
struct node * new_node(int type, struct node * left_of, struct node * right_of) {
	
	struct node * node = (struct node *) malloc(sizeof(struct node));

	node->type = type;
	
	if (left_of != NULL)
		left_of->left = node;

	if (right_of != NULL)
		right_of->right = node;

	return node;
}

void search_tree(struct node * node) {
	switch(node->type) {
		case START_OF_PROG_TYPE:
			if (node->right != NULL) {
				search_tree(node->right);
			}
			if (node->left != NULL) {
				search_tree(node->left);
			}
			break;
		case PARAMETER_TYPE:
			printf("%s = %0.4lf;\n", node->var, node->value);
			break;
		case RATE_TYPE:
		case SPECIE_TYPE:
			printf("compartment %s;\n",  node->compartment);
			printf("%s = 1.0;\n", node->compartment);
			search_program(node, node->compartment);
			break;
	}
}

void search_program(struct node * node, char * compartment) {
	
	if (node->type == SPECIE_TYPE) {
		printf("specie %s;\n", node->var);
		printf("%s in %s;\n", node->var, compartment);
		printf("%s = %0.4lf;\n", node->var, node->value);
	} else if(node->type == RATE_TYPE) {
		for (int i = 0; i < node->n_reac; i++) {
			printf("%s ", node->reac[i]);
		}
		printf("->");
		for (int i = 0; i < node->n_prod - 1; i++) {
			if (i == 0) {
				printf("s ");
			}
			printf("%s ", node->prod[i]);
		}

		if (node->n_prod > 0 && node->n_prod != 1)
			printf("%s; %s;\n", node->prod[node->n_prod - 1], node->var);
		else if (node->n_prod == 1)
			printf(" %s; %s;\n", node->prod[node->n_prod - 1], node->var);
	}

	if (node->right != NULL) {
		search_program(node->right, compartment);
	}
}

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
//
//				start_prog
//				   /
//			 start_prog
//				/       \
//		  start_prog    parameter
//	   	      /     \
//		start_prog  specie
//			/			\
//	  start_prog		 rate
//		  /		  \		    \
//  start_program  parameter specie
// [...]	   
//			    
//
//				parameter
//				   /
//			 parameter
//				/    
//		      rate    
//	   	      /     \
//		parameter  specie
//			/			\
//	    specie		    rate
//		  /	  \		       \
//  parameter  rate       specie


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
									$$ = $1;
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

int main(int argc, char **argv)
{
    if (yyparse() != 1)
		search_tree(head);
}

int yyerror(const char *s)
{
    fprintf(stderr, "erro: %s\n", s);
}