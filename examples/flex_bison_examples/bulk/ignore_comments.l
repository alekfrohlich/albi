/* Only skip comments. */
%{  
#include <stdio.h>
%}

COMMENT     "//".*

%%

{COMMENT}

%%

int yyparse(const char *file)
{
    yyin = fopen (file, "r");
    if (!yyin)
    {
        perror ("fopen");
        exit (EXIT_FAILURE);
    }
    /* One token only. */
    yylex ();
    if (fclose (yyin) != 0)
    {
        perror ("fclose");
        exit (EXIT_FAILURE);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Unknown parameters, usage: ig <file>\n");
        return -1;
    }

    YY_FLUSH_BUFFER;
    yyparse(argv[1]);
}