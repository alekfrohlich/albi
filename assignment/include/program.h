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
#include "parsing.h"
#include "ast.h"

#ifndef __PROGRAM__
#define __PROGRAM__

struct program {
	struct symbol * symtab;
    struct symlist * parameters;
	
    struct assignlist * species;
	struct assignlist * locals;

	struct symlist * dependence;  // ignored.

	struct reactionlist * reactions;
    int reactions_size;
};

struct reactionlist {
    struct reaction * reac;
    struct reactionlist * next;
};

struct reaction {
    struct assignlist * reactant;
    struct assignlist * product;
    struct ast * rate;
};

extern void progdef(struct symbol *name, struct symlist *syms, struct stmtlist *stmts);
extern void mergeprograms(
    struct symbol** progrefs,
    struct symlist** export,
    struct explist** params,
    int size);
#endif  // __PROGRAM__