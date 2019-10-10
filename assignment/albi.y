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
%token ASSIGN SEMICOL EOL PROG OPEN_BODY CLOSE_BODY

%%

start_of_prog:
| start_of_prog parameter EOL
| start_of_prog program EOL
;

program_def:
 PROG VAR ASSIGN	{printf("component %s\n",  $2); }
;

program_body:
OPEN_BODY
| program_body statement
| program_body CLOSE_BODY SEMICOL
;

program :
 program_def program_body
;

statement:
 VAR ASSIGN NUM SEMICOL
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