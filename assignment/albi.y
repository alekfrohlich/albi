%{
#include <stdio.h>
int yyerror(const char* s);
int yylex(void);
%}

%union {
    char * str;
    float f;
}

%token <f> NUM
%token <str> VAR
%token ASSIGN SEMICOL EOL PROG OPEN_BODY CLOSE_BODY SUB ADD OPEN_PAR CLOSE_PAR DOUBLEDOT RATE

%type <str> program_def program_construct statement

%%

start_of_prog:
| start_of_prog parameter EOL
| start_of_prog program EOL
;

program_def:
 PROG VAR ASSIGN OPEN_BODY	{$$ = $2; printf("compartment %s;\n",  $2); printf("%s = 1.0;\n", $2);}
;

program_construct: program_def
| program_construct statement {printf("specie %s;\n", $2); printf("%s in %s;\n", $2, $1); }
| program_construct rate
;

program :
 program_construct CLOSE_BODY SEMICOL
;

statement:
 VAR ASSIGN NUM SEMICOL { printf("%s = %0.4f;\n", $1, $3); }
;

rate_def :
RATE OPEN_PAR VAR CLOSE_PAR DOUBLEDOT OPEN_BODY { printf("rate definition\n"); }
;

rate_construct: rate_def
| rate_construct product
| rate_construct reactant
;

rate : 
rate_construct CLOSE_BODY
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