%{
#include <stdio.h>
#include <stdlib.h>
int yyerror(const char* s);
int yylex(void);

# define START_OF_PROG_TYPE 1
# define PROGRAM_TYPE 2
# define PARAMETER_TYPE 3

struct node {
	int type;
	struct node * left;
	struct node * right;
};

struct node * head;

struct node * new_node(int type, struct node * left_of, struct node * right_of) {
	struct node * node = (struct node *) malloc(sizeof(struct node));
	node->type = type;
	if (left_of != NULL) {
		left_of->left = node;
	}

	if (right_of != NULL) {
		right_of->right = node;
	}

	return node;
}

%}

%union {
    char * str;
    float f;
    struct node * node_type;
}

%token <f> NUM
%token <str> VAR
%token ASSIGN SEMICOL EOL PROG OPEN_BODY CLOSE_BODY SUB ADD OPEN_PAR CLOSE_PAR DOUBLEDOT RATE

%type <node_type> start_of_prog

%%

start_of_prog: {head = new_node(START_OF_PROG_TYPE, NULL, NULL); $$ = head; }
| start_of_prog parameter EOL {$$ = new_node(START_OF_PROG_TYPE, $1, NULL); new_node(PARAMETER_TYPE, NULL, $$); }
| start_of_prog program EOL {$$ = new_node(START_OF_PROG_TYPE, $1, NULL); new_node(PROGRAM_TYPE, NULL, $$); }
;

program_def:
 PROG VAR ASSIGN {printf("compartment %s;\n",  $2); printf("%s = 1.0;\n", $2);}
;

program :
 program_def program_body SEMICOL {printf("ss\n");}
;

program_content_list :  statement {printf("we have a body\n");}
| rate
| program_content_list statement
| program_content_list rate
;

program_body: OPEN_BODY program_content_list CLOSE_BODY {printf("found body\n"); }
;

statement:
 VAR ASSIGN NUM SEMICOL { printf("%s = %0.4f;\n", $1, $3); }
;

rate_def :
RATE OPEN_PAR VAR CLOSE_PAR DOUBLEDOT { printf("rate definition\n"); }
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
OPEN_BODY reagent_list CLOSE_BODY
;

rate : 
rate_def rate_body
;

reactant :
VAR ASSIGN VAR SUB NUM SEMICOL {printf("%s--\n", $1);}
;

product :
VAR ASSIGN VAR ADD NUM SEMICOL {printf("%s++\n", $1);}
;

parameter :
 statement
;

%%

int main(int argc, char **argv)
{
    yyparse();
}

int yyerror(const char *s)
{
    fprintf(stderr, "erro: %s\n", s);
}