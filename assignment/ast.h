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
    SYM_REF,
    SYM_ASSIGN,
    PRIV_COMPART,
    SHARED_COMPART,
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
struct newcompart {
 	enum nodetypes type;
    struct symbol *sym;
    struct symlist *progs;
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

// END AST NODES

/**
 * Build an AST.
 */
struct ast *newast(enum nodetypes type, struct ast *l, struct ast *r);
struct ast *newcompart(struct symbol *sym, struct ast *args);
struct ast *newnum(double d);
struct ast *newref(struct symbol *sym);
struct ast *newassign(struct symbol *s, struct ast *v);

/** 
 * Define a program.
 */
void progdef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/**
 * Generate SBML model from AST.
 */
double genmodel(struct ast *);

/**
 * Free AST.
 */
void treefree(struct ast *);

#endif // __AST_H__