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

#include "ast.h"
#include "output.h"
#include "structures.h"

/**
 * Generate AST with proper name corrections (print)
 */
void outast(struct ast *a, struct maplist *namemap)
{
    if (a == NULL)
        return;

    switch (a->type)
    {
        case PLUS:
            outast(a->left, namemap);
            fprintf(yyout, "+" );
            outast(a->right, namemap);
            break;

        case MINUS:
            outast(a->left, namemap);
            fprintf(yyout, "-" );
            outast(a->right, namemap);
            break;

        case DIV:
            outast(a->left, namemap);
            fprintf(yyout, "/" );
            outast(a->right, namemap);
            break;

        case TIMES:
            outast(a->left, namemap);
            fprintf(yyout, "*");
            outast(a->right, namemap);
            break;

        case SYM_REF:
            fprintf(yyout, "%s",
                    getmap(namemap, ((struct symref*) a)->sym->name));
            break;

        case CONSLIT:
            fprintf(yyout, "%0.4lf", ((struct numval*) a)->number);
            break;

        default:
            printf("internal error: bad node at eval, type %u\n", a->type);
    }
}

/**
 * Declare and define program variables (print)
 */
void outdecls(struct nodelist *decls, struct maplist *namemap, int compartnum)
{
    for (struct nodelist *it = decls; it != NULL; it = it->next)
    {
        // Declare it
        fprintf(yyout, "%s %s in ECOLI%d\n",
                ((struct tsymassign*)it->node)->_type == SPECIE? "Specie" : "Var",
                getmap(namemap, ((struct tsymassign*)it->node)->sym->name), compartnum);

        // Define it
        fprintf(yyout, "%s = %0.4lf\n",
                getmap(namemap, ((struct tsymassign*)it->node)->sym->name),
                ((struct tsymassign*)it->node)->sym->value);
    }
}

/**
 * List program reactions (print)
 */
void outreacs(struct reactionlist *reacs, struct maplist * namemap)
{
    for (struct reactionlist *it = reacs; it != NULL; it = it->next)
    {
        for (struct nodelist *it2 = it->reac->reactant; it2 != NULL; it2 = it2->next)
            fprintf(yyout, "%s",
                    getmap(namemap, ((struct symassign *)it2->node)->sym->name));

        fprintf(yyout, "-> ");

        for (struct nodelist *it2 = it->reac->product; it2 != NULL; it2 = it2->next)
            fprintf(yyout, "%s",
                    getmap(namemap, ((struct symassign *)it2->node)->sym->name));

        fprintf(yyout, ";");
        outast(it->reac->rate, namemap);
        fprintf(yyout, ";\n");
    }
}