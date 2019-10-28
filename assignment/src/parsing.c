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

struct symbol* env[2];
int curr_compart = 0;

/**
 * Evaluate arithmetic expression tree.
 */
double eval(struct ast *a)
{
    double v;

    switch (a->type)
    {

    // constants.
    case CONSLIT:
        v = ((struct numval *) a)->number;
        break;

    // name reference.
    case SYM_REF:
        v = ((struct symref *) a)->sym->value;
        break;

    // expressions:
    case PLUS:  v = eval(a->left) + eval(a->right); break;
    case MINUS: v = eval(a->left) - eval(a->right); break;
    case TIMES: v = eval(a->left) * eval(a->right); break;
    case DIV:   v = eval(a->left) / eval(a->right); break;

    default:
        printf("internal error: bad node %c\n", a->type);
    }

    return v;
}

// SBML GENERATION FUNCTIONS.

void genparam(char *name, struct ast *val)
{
    fprintf(yyout, "%s = %0.4lf;\n", name, eval(val));
}
 
void gencompart(struct compart * compartment) 
{
    curr_compart++;
    struct calllist * call = compartment->params;

    int size = 0;
    while (call != NULL)
    {
        call = call->next;
        size++;
    }

    struct symbol * prog[size];
    struct symlist * export[size];
    struct explist * params[size];

    call  = compartment->params;
    int index = 0;
    while (call != NULL)
    {
        struct calllist * aux = call;
        call = call->next;
        prog[index] = aux->sym;
        export[index] = aux->list;
        params[index] = aux->exp;
        index++;
    }
    
    mergeprograms(prog, export, params, size);
}

// PARSING STRUCTURES INITIALIZATION.

struct stmtlist *newstmtlist(struct ast * stmt, struct stmtlist * next)
{
    struct stmtlist * sl = (struct stmtlist *) malloc(sizeof(struct stmtlist));
    sl->stmt = stmt;
    sl->next = next;
    return sl;
}

struct assignlist *newassignlist(struct symassign *assign, struct assignlist *next)
{
    struct assignlist * al = (struct assignlist *) malloc(sizeof(struct assignlist));
    al->assign = assign;
    al->next = next;
    return al; 
}

struct symlist * newsymlist(struct symbol *sym, struct symlist *next)
{
    struct symlist *sl = malloc(sizeof(struct symlist));
    sl->sym = sym;
    sl->next = next;
    return sl;
}

struct explist * newexplist(struct ast* exp, struct explist* next)
{
    struct explist *a = (struct explist *) malloc(sizeof(struct explist));
    a->exp = exp;
    a->next = (struct explist*) next;
    return a;
}

struct calllist *newcalllist(
    struct symbol* sym, 
    struct symlist * symlist, 
    struct explist* explist, 
    struct calllist* next) 
{
    struct calllist * p = (struct calllist *) malloc(sizeof(struct calllist)); 
    p->sym = sym;
    p->list = symlist;
    p->exp = explist;
    p->next = next;
}

// FREE PARSING STRUCTURES.

static void symlistfree(struct symlist *sl)
{
    struct symlist *nsl;

    while(sl)
    {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

static void explistfree(struct explist * el) 
{
    while (el != NULL) {
        struct explist * aux = el;
        el = el->next;
        treefree(aux->exp);
        free(aux);
    }
}

static void assignlistfree(struct assignlist *sl)
{
    while (sl != NULL) {
        struct assignlist * aux = sl;
        sl = sl->next;
        treefree((struct ast *) aux->assign);
        free(aux);
    }
}

static void stmtlistfree(struct stmtlist * l)
{
    while (l != NULL) {
        struct stmtlist * aux = l;
        l = l->next;
        treefree(aux->stmt);
        free(aux);
    }
}

static void calllistfree(struct calllist * prog)
{
    while (prog != NULL) {
        struct calllist * aux = prog;
        prog = prog->next;
        symlistfree(aux->list);
        explistfree(aux->exp);
        free(aux);
    }
}

void treefree(struct ast *a)
{
    switch (a->type)
    {
    // two subtrees.
    case PLUS:
    case MINUS:
    case TIMES:
    case DIV:
        treefree(a->left);
        treefree(a->right);
        break;

    // one subtree.
    case SYM_ASSIGN:
        treefree(((struct symassign*)a)->val);
        break;
    case EXPLIST:
        treefree(a->left);
        treefree(a->right);
        break;

    // no subtree.
    case SYM_REF:
        break;
    case RATESTATEMENT:
        ;
        struct rate * rate = (struct rate *) a;
        treefree(rate->exp);
        assignlistfree(rate->assigns);
    case COMPART: // double free problem?
        ;
        calllistfree(((struct compart *)a)->params);
        break;
    case CONSLIT:
        break;

    default:
        printf("internal error: bad node %c\n", a->type);
    }

    free(a);
}