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

#ifndef __AST_H__
#define __AST_H__

#include "symtab.h"

// BEGIN AST NODES

enum nodetypes {
    CONSLIT = 1,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    EXPLIST,
    SYM_REF,
    SYM_ASSIGN,
    COMPART,
};

/**
 * Generic expression node.
 */
struct ast {
	enum nodetypes type;
	struct ast * left;
	struct ast * right;
};

/**
 * Compartment instantiation node.
 */
struct compart {
 	enum nodetypes type;
    struct symbol *sym;
    struct progcall *params;  
};

/**
 * Numeric value node.
 */
struct numval {
	enum nodetypes type;
    double number;
};

/**
 * Reference to symbol node.
 */
struct symref {
 	enum nodetypes type;
    struct symbol *sym;
};

/**
 * Symbol assignment node.
 */
struct symassign {
	enum nodetypes type;
    struct symbol *sym;
    struct ast *val;
};

/**
 * Symbol assignment node.
 */
struct rate {
	enum nodetypes type;
    struct ast *exp;
    struct assignlist *assigns;
};

// END AST NODES

struct progcall {
	struct symbol * sym;
	struct progcall * next;
	struct symlist * list;
};

// forward defined.
extern struct ast *newast(enum nodetypes type, struct ast *l, struct ast *r);
extern struct ast *newcompart(struct symbol *sym, struct progcall *params);
extern struct ast *newnum(double d);
extern struct ast *newref(struct symbol *sym);
extern struct ast *newassign(struct symbol *s, struct ast *v);
extern struct ast *newrate(struct ast* exp, struct assignlist *assigns);
extern struct assignlist *newassignlist(struct symassign *assign, struct assignlist *next);
extern void assignlistfree(struct assignlist *sl);
extern void progdef(struct symbol *name, struct symlist *syms, struct ast *stmts);
extern double genmodel(struct ast *);
extern void treefree(struct ast *);

#endif // __AST_H__