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

void printinorder(struct ast * ast, struct maplist * map) {
    if (ast == NULL)
        return;

    switch (ast->type) {
        case PLUS:
        printinorder(ast->left, map);
        fprintf(yyout, "+" );
        printinorder(ast->right, map);
        break;
        case MINUS:
        printinorder(ast->left, map);
        fprintf(yyout, "-" );
        printinorder(ast->right, map);
        break;
        case DIV:
        printinorder(ast->left, map);
        fprintf(yyout, "/" );
        printinorder(ast->right, map);
        break;
        case TIMES:
        printinorder(ast->left, map);
        fprintf(yyout, "*");
        printinorder(ast->right, map);
        break;
        case SYM_REF:
        fprintf(yyout, "%s", getmap(map, ((struct symref*)ast)->sym->name));
        break;
        case CONSLIT:
        fprintf(yyout, "%0.4lf", ((struct numval*)ast)->number);
        break;
        default:
        ;
        //GENERATE ERROR OR WARN
        // yyerror()
    }
}

/**
 * Declare and define program variables.
 */
void outdecls(struct nodelist *decls, MAP namemap, int compartnum)
{
    for (struct nodelist *it = decls; it != NULL; it = it->next)
    {
        // Declare it.
        fprintf(yyout, "%s %s in ECOLI%d\n",
                ((struct tsymassign*)it->node)->_type == SPECIE? "Specie" : "Var", 
                getmap(namemap, ((struct tsymassign*)it->node)->sym->name), compartnum);

        // Define it.
        fprintf(yyout, "%s = %0.4lf\n",
                getmap(namemap, ((struct tsymassign*)it->node)->sym->name),
                ((struct tsymassign*)it->node)->sym->value);
    }
}

static void printreaction(struct reaction * reac, struct maplist * map)
{
    struct nodelist *reactant = reac->reactant;
    struct nodelist *product = reac->product;
    int first = 1;
    while (reactant != NULL) {
        if (!first)
            fprintf(yyout, " ");
        else
            first = 0;
        fprintf(yyout, "%s",
                getmap(map, ((struct symassign *)reactant->node)->sym->name));
        reactant = reactant->next;
    }
    fprintf(yyout, "-> ");
    while (product != NULL) {
        fprintf(yyout, "%s",
                getmap(map, ((struct symassign *)product->node)->sym->name));

        if(product->next != NULL)
            fprintf(yyout, " ");
        product = product->next;
    }
    fprintf(yyout, ";");
    printinorder(reac->rate, map);
    fprintf(yyout, ";\n");
}

/**
 * List program reactions.
 */
void outreacs(struct reactionlist *reacs, MAP namemap)
{
    struct reactionlist * element = reacs;
    while (element != NULL) {
        printreaction(element->reac, namemap);
        element = element->next;
    }
}