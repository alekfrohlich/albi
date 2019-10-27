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

#ifndef __PARSING__
#define __PARSING__

#include "ast.h"
#include <stdio.h>

// BEGIN INTERMEDIATE NODES

struct explist {
	struct ast * exp;
	struct explist * next;
};

struct progcall {
	struct symbol * sym;
	struct progcall * next;
	struct symlist * list;
	struct explist * exp;
};

struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct assignlist {
    struct symassign *assign;
    struct assignlist *next;
};

struct stmtlist {
	struct ast * stmt;
	struct stmtlist * next;
};
// END INTERMEDIATE NODES

// forward defined.
extern void genparam(char *name, struct ast *val);
extern void gencompart(struct compart* compartment);
extern void symdef(struct symbol *sym, struct ast *val);
extern void progdef(struct symbol *name, struct symlist *syms, struct stmtlist *stmts);
extern struct assignlist *newassignlist(struct symassign *assign, struct assignlist *next);
extern struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
extern struct explist *newexplist(struct ast* exp, struct explist* next);
extern struct stmtlist *newstmtlist(struct ast * stmt, struct stmtlist * next);
extern struct progcall *newprogcall();
extern void treefree(struct ast *a);

// toolchain defined.
extern int yyerror(const char *s);
extern int yylex(void);
extern void abort(void);
extern FILE * yyout;

#endif  // __PARSING__