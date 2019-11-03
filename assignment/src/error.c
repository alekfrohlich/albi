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

#include <stdio.h>
#include <stdarg.h>

#include "albi.tab.h"
#include "ast.h"
#include "output.h"

// Colorful output
#define ERROR_RED(s)    fprintf(stderr, "\033[1;31m%s\033[0m", s)
#define BEGIN_BOLD      fprintf(stderr, "\033[1;80m")
#define COLOR_RESET     fprintf(stderr, "\033[0m")

int surpressout;        // Don't generate SBML if error was reported
int nowrites;           // Symbol table writable?
int yycolumn;           // Bison location
char *currfilename;     // Current yyin file name

/**
 * Display expression to stderr (err)
 */
void errexp(struct ast *a)
{
    if (a == NULL)
        return;

    switch (a->type)
    {
        case UMINUS:
            fprintf(stderr, "-");
            errexp(a->left);
            break;

        case PLUS:
        case MINUS:
        case TIMES:
        case DIV:
        case MOD:
        case POW:
            errexp(a->left);
            fprintf(stderr, "%c", ast2str[a->type]);
            errexp(a->right);
            break;

        case SYM_REF:
            fprintf(stderr, "%s", ((struct symref*) a)->sym->name);
            break;

        case CONSLIT:
            fprintf(stderr, "%0.4lf", ((struct numval*) a)->number);
            break;

        default:
            printf("internal error: bad node at errexp, type %c\n", ast2str[a->type]);
    }
}


///////////////////////////////////////////////////////////////////////////////
//                      VISIBLE ERROR DISPLAY FUNCTIONS                      //
///////////////////////////////////////////////////////////////////////////////


/**
 * Display error message (err)
 */
int yyerror(const char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    if (yylloc.first_line)
    {
        BEGIN_BOLD;
        fprintf(stderr, "%s:%d:%d: ", currfilename, yylloc.last_line,
                yylloc.last_column);
        COLOR_RESET;
    }

    ERROR_RED("error: ");
    vfprintf(stderr, s, ap);
    fprintf(stderr, ".\n");
    surpressout = 1;
}

/**
 * Display invalid expression (err)
 */
int yyerrorexp(const char *s, struct ast *a, ...)
{
    yyerror(s);
    ERROR_RED("-> ");
    errexp(a);
    ERROR_RED(" <-\n");
}