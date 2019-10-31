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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parsing.h"
#include "program.h"
#include "structures.h"
#include "output.h"
#include "error.h"

struct symbol* env[2];         // [ Global | Local ] parsing tables
int currcompart = 0;           // Current compartment count

/**
 * Evaluate arithmetic expression tree
 */
double eval(struct ast *a)
{
    double v;

    switch (a->type)
    {

    // Constants
    case CONSLIT:
        v = ((struct numval *) a)->number;
        break;

    // Symbol references
    case SYM_REF:
        v = ((struct symref *) a)->sym->value;
        break;

    // Expressions:
    case PLUS:  v = eval(a->left) + eval(a->right); break;
    case MINUS: v = eval(a->left) - eval(a->right); break;
    case TIMES: v = eval(a->left) * eval(a->right); break;
    case DIV:   v = eval(a->left) / eval(a->right); break;

    default:
        yyerror("internal error: bad node at eval %d", a->type);
    }

    return v;
}

///////////////////////////////////////////////////////////////////////////////
//                       INTERMEDIATE CODE GENERATION                        //
///////////////////////////////////////////////////////////////////////////////

/**
 * Generate intermediate
 * representation of parameter (print)
 */
void genparam(char *name, struct ast *val)
{
    outparam(name, val);
}

 /**
  * Generate intermediate
  * representation of compartment (print)
  */
void gencompart(struct compart *compartment)
{
    struct maplist **maps;
    struct symbol **progrefs = (struct symbol**)malloc(sizeof(struct symbol*)*20);
    struct nodelist **explists = (struct nodelist**) malloc(sizeof(struct nodelist*)*20);

    int progcount = 0;
    for (struct progcall *it = compartment->call; it != NULL;
         it = it->next, progcount++)
    {
        progrefs[progcount] = it->progref;
        explists[progcount] = it->explist;
    }

    // Eval and apply explist. TODO: handle shares
    maps = mergeprograms(progrefs, NULL, explists, currcompart, progcount);

    fprintf(yyout, "Compartment ECOLI%d\n", currcompart);

    // Generate intermediate code (print)
    for (int i = 0; i < progcount; i++)
    {
        outdecls(progrefs[i]->prog->declarations, maps[i], currcompart);
        outreacs(progrefs[i]->prog->reactions, maps[i]);
    }
    currcompart++;
}

///////////////////////////////////////////////////////////////////////////////
//                          STRUCTURE MANINPULATION                          //
///////////////////////////////////////////////////////////////////////////////

/**
 * Create list of AST nodes
 */
struct nodelist *newnodelist(struct ast *node, struct nodelist *next)
{
    struct nodelist * list = (struct nodelist *) malloc(sizeof(struct nodelist));
    list->node = node;
    list->next = next;
    return list;
}

/**
 * Free list of AST nodes
 */
void nodelistfree(struct nodelist *list)
{
    // TODO
}

/**
 * Create list of program call parameters
 */
struct progcall *newprogcall(
    struct symbol *progref,
    struct symlist *shares,
    struct nodelist *explist,
    struct progcall *next)
{
    struct progcall * p = (struct progcall *) malloc(sizeof(struct progcall));
    p->progref = progref;
    p->shares = shares;
    p->explist = explist;
    p->next = next;
}

/**
 * Free list of program call parameters
 */
void progcallfree(struct progcall *progcall)
{
    while (progcall != NULL)
    {
        struct progcall * aux = progcall;
        progcall = progcall->next;
        free(aux);
    }
}