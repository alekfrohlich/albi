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
#include "parsing.h"

void progdef(struct symbol *name, struct symlist *syms, struct ast *stmts)
{

}
double genmodel(struct ast *a)
{

}

struct symlist * newsymlist(struct symbol *sym, struct symlist *next)
{
    struct symlist *sl = malloc(sizeof(struct symlist));

    sl->sym = sym;
    sl->next = next;

    return sl;
}

struct symbol* env[2];

struct assignlist *newassignlist(struct symassign *assign, struct assignlist *next);
void assignlistfree(struct assignlist *sl);

void symlistfree(struct symlist *sl)
{
    struct symlist *nsl;

    while(sl)
    {
        nsl - sl->next;
        free(sl);
        sl = nsl;
    }
}

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

struct explist * newexplist(struct ast* exp, struct explist* next) {
    struct explist *a = (struct explist *) malloc(sizeof(struct explist));
    a->exp = exp;
    a->next = (struct explist*) next;
    return a;
}

void explistfree(struct explist * el);
