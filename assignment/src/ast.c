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

#include "ast.h"
#include "parsing.h"
#include "error.h"

/**
 * Create generic expression node
 */
struct ast *newast(enum nodetypes type, struct ast *left, struct ast *right)
{
    struct ast *a = malloc(sizeof(struct ast));
    a->type = type;
    a->left = left;
    a->right = right;
    return a;
}

/**
 * Create compartment instantiation node
 */
struct ast *newcompart(char *name, struct progcall *call)
{
    struct compart *a = malloc(sizeof(struct compart));
    a->type = COMPART;
    a->name = name;
    a->call = call;
    return (struct ast*) a;
}

/**
 * Create numeric value node
 */
struct ast *newnum(double d)
{
    struct numval *a = malloc(sizeof(struct numval));
    a->type = CONSLIT;
    a->number = d;
    return (struct ast*) a;
}

/**
 * Create symbol reference
 */
struct ast *newref(struct symbol *sym)
{
    struct symref *a = malloc(sizeof(struct symref));
    a->type = SYM_REF;
    a->sym = sym;
    return (struct ast*) a;
}

/**
 * Create symbol assingment node
 */
struct ast *newassign(struct symbol *sym, struct ast *val)
{
    struct symassign *a = malloc(sizeof(struct symassign));
    a->type = SYM_ASSIGN;
    a->sym = sym;
    a->val = val;
    return (struct ast*) a;
}

/**
 * Create typed symbol assignment node
 */
struct ast *newtassign(enum sbmltypes type, struct symbol *sym, struct ast *val)
{
    struct tsymassign *a = malloc(sizeof(struct tsymassign));
    a->type = T_SYM_ASSIGN;
    a->_type = type;
    a->sym = sym;
    a->val = val;
    return (struct ast*) a;
}

/**
 * Create rate expression node
 */
struct ast *newrate(struct ast *exp, struct nodelist *assigns)
{
    struct rate *r = malloc(sizeof(struct rate));
    r->type = RATESTATEMENT;
    r->assigns = assigns;
    r->exp = exp;
    return (struct ast*) r;
}

struct ast *newfuncall(enum funtypes type, struct ast *exp)
{
    struct funcall *fc = malloc(sizeof(struct funcall));
    fc->type = BUILTIN;
    fc->_type = type;
    fc->exp = exp;
    return (struct ast*) fc;
}

/**
 * Free AST
 */
void treefree(struct ast *a)
{
    switch (a->type)
    {

    // Two subtrees.
    case PLUS:
    case MINUS:
    case TIMES:
    case DIV:
        treefree(a->left);
        treefree(a->right);
        break;

    // One subtree.
    case SYM_ASSIGN:
        treefree(((struct symassign*)a)->val);
        break;
    case NODELIST:
        treefree(a->left);
        treefree(a->right);
        break;

    // No subtree.
    case SYM_REF:
        break;
    case RATESTATEMENT:
        ;
        struct rate *rate = (struct rate*)a;
        treefree(rate->exp);
        // nodelistfree?
    case COMPART: // double free problem?
        ;
        progcallfree(((struct compart*)a)->call);
        break;
    case CONSLIT:
        break;

    default:
        yyerror("internal error: bad node at treefree %d", a->type);
    }

    free(a);
}