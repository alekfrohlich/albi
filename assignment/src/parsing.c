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


void symbolfree(struct symbol * sym) {
    free(sym->name);
    free(sym->prog);
}

void progdef(struct symbol *name, struct symlist *syms, struct stmtlist *stmts)
{

}

struct progcall *newprogcall(
    struct symbol* sym, 
    struct symlist * symlist, 
    struct explist* explist, 
    struct progcall* next) 
{
    struct progcall * p = (struct progcall *) malloc(sizeof(struct progcall)); 
    p->sym = sym;
    p->list = symlist;
    p->exp = explist;
    p->next = next;
}

void progcallfree(struct progcall * prog) {
    while (prog != NULL) {
        struct progcall * aux = prog;
        prog = prog->next;
        symlistfree(aux->list);
        explistfree(aux->exp);
        symbolfree(aux->sym);
        free(aux);
    }
}

double genmodel(struct ast *a)
{

}

double genmodelprogram(struct ast *a) 
{

}


struct stmtlist *newstmtlist(struct ast * stmt, struct stmtlist * next) {
    struct stmtlist * sl = (struct stmtlist *) malloc(sizeof(struct stmtlist));
    sl->stmt = stmt;
    sl->next = next;
    return sl;
}

extern void stmtlistfree(struct stmtlist * l) {
    while (l != NULL) {
        struct stmtlist * aux = l;
        l = l->next;
        treefree(aux->stmt);
        free(aux);
    }
}

struct symbol* env[2];

struct assignlist *newassignlist(struct symassign *assign, struct assignlist *next) {
    struct assignlist * al = (struct assignlist *) malloc(sizeof(struct assignlist));
    al->assign = assign;
    al->next = next;
    return al; 
}

void assignlistfree(struct assignlist *sl) {
    while (sl != NULL) {
        struct assignlist * aux = sl;
        sl = sl->next;
        treefree((struct ast *) aux->assign);
        free(aux);
    }
}

struct symlist * newsymlist(struct symbol *sym, struct symlist *next)
{
    struct symlist *sl = malloc(sizeof(struct symlist));

    sl->sym = sym;
    sl->next = next;

    return sl;
}

void symlistfree(struct symlist *sl)
{
    struct symlist *nsl;

    while(sl)
    {
        nsl = sl->next;
        symbolfree(sl->sym);
        free(sl);
        sl = nsl;
    }
}

struct explist * newexplist(struct ast* exp, struct explist* next) {
    struct explist *a = (struct explist *) malloc(sizeof(struct explist));
    a->exp = exp;
    a->next = (struct explist*) next;
    return a;
}

void explistfree(struct explist * el) 
{
    while (el != NULL) {
        struct explist * aux = el;
        el = el->next;
        treefree(aux->exp);
        free(aux);
    }
}
