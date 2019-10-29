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
#include "ast.h"
#include "parsing.h"

struct ast * newast(enum nodetypes type, struct ast *l, struct ast *r)
{
    struct ast *a = malloc(sizeof(struct ast));

    a->type = type;
    a->left = l;
    a->right = r;

    return a;
}

struct ast *newcompart(char *sym, struct calllist *params) {
    struct compart *a = malloc(sizeof(struct compart*));
    a->type = COMPART;
    a->sym = sym;
    a->params = params;
    return (struct ast*) a;
}

struct ast * newnum(double d)
{
    struct numval *a = malloc(sizeof(struct numval));

    a->type = CONSLIT;
    a->number = d;
    
    return (struct ast *) a;
}

struct ast * newref(struct symbol *sym)
{
    struct symref *a = malloc(sizeof(struct symref));

    a->type = SYM_REF;
    a->sym = sym;

    return (struct ast *) a;
}

struct ast * newassign(struct symbol *sym, struct ast *val)
{
    struct symassign *a = malloc(sizeof(struct symassign));

    a->type = SYM_ASSIGN;
    a->sym = sym;
    a->val = val;

    return (struct ast *) a;
}

struct ast *newrate(struct ast* exp, struct nodelist *assigns) {
    struct rate * r = malloc(sizeof(struct rate));
    r->assigns = assigns;
    r->type = RATESTATEMENT;

    return (struct ast *) r;
}