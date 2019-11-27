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

#ifndef PARSING_H
#define PARSING_H

#include "ast.h"

/**
 * List of AST nodes
 */
struct nodelist {
  struct ast *node;
  struct nodelist *next;
};

/**
 * Program call list
 */
struct progcall {
  struct symbol *progref;   // Called program
  struct symlist *shares;   // Shared variables
  struct nodelist *explist; // Parameters
  struct progcall *next;    // Next called program
};

// Forward definitions
extern double eval(struct ast *a);
extern double callbuiltin(struct funcall *fun);
extern void genparam(char *name, struct ast *val);
extern void gencompart(struct compart *compartment);
extern struct nodelist *newnodelist(struct ast *node, struct nodelist *next);
extern struct progcall *newprogcall(struct symbol *sym, struct symlist *symlist,
                                    struct nodelist *parameters,
                                    struct progcall *call);
extern void progcallfree(struct progcall *prog);

#endif // PARSING_H