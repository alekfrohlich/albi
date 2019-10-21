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
#include <ast.h>

struct ast * newast(enum nodetypes type, struct ast *l, struct ast *r)
{
    struct ast *a = malloc(sizeof(struct ast));

    a->type = type;
    a->left = l;
    a->right = r;

    return a;
}

struct ast *newcompart(struct symbol *sym, struct progcall *params);

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

struct ast *newrate(struct ast* exp, struct assignlist *assigns);

// void treefree(struct ast *a)
// {
//     switch (a->type)
//     {

//     // two subtrees.
//     case PLUS:
//     case MINUS:
//     case TIMES:
//     case DIV:
//         treefree(a->left);
//         treefree(a->right);
//         break;
    
//     // one subtree.
//     case SYM_ASSIGN:
//         free(((struct symassign *) a)->sym);
//         treefree(a->left);

//     // no subtree.
//     case CONSLIT: break;
//     case SYM_REF:
//         free(((struct symref *) a)->sym);
//         break;
//     case PRIV_COMPART: // double free problem?
//     case SHARED_COMPART:
//         break;

//     default:
//         printf("internal error: bad node %c\n", a->type);
//     }

//     free(a);
// }
