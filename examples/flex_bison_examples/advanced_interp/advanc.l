%option noyywrap nodefault yylineno

%{
    #include "advanc.h"
    #include "advanc.tab.h"
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%

"+" |
"-" |
"*" |
"/" |
"=" |
"|" |
"," |
";" |
"(" |
")"                     { return yytext[0]; }

">"                     { yylval.fn = 1; return CMP; }
"<"                     { yylval.fn = 2; return CMP; }
"<>"                    { yylval.fn = 3; return CMP; }
"=="                    { yylval.fn = 4; return CMP; }
">="                    { yylval.fn = 5; return CMP; }
"<="                    { yylval.fn = 6; return CMP; }

"if"                    { return IF;}
"then"                  { return THEN;}
"else"                  { return ELSE;}
"while"                 { return WHILE;}
"do"                    { return DO;}
"let"                   { return LET;}

 "__builtin_sqrt"       { yylval.fn = __builtin_sqrt; return FUNC; }
 "__builtin_exp"        { yylval.fn = __builtin_exp; return FUNC; } 
 "__builtin_log"        { yylval.fn = __builtin_log; return FUNC; } 
 "__builtin_print"      { yylval.fn = __builtin_print; return FUNC; } 

[a-zA-Z][a-zA-Z0-9]*    { yylval.s = lookup(yytext); return NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?        { yylval.d = atof(yytext); return NUMBER; } 

"//".*
[ \t]                   { }

\\\n                    { printf("c> "); }

\n                      { return EOL; }


.                       { yyerror("Mystery character %c\n", *yytext); }
%%