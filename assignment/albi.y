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
%token ASSIGN SEMICOL EOL

%%

start_of_prog:
| start_of_prog parameter EOL 
;

parameter:
 VAR ASSIGN NUM SEMICOL     { printf("%s = %0.4f;\n", $1, $3); }
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