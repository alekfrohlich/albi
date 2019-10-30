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

#ifndef PROGRAM_H
#define PROGRAM_H

    #include "symtab.h"
    #include "parsing.h"
    #include "structures.h"

    /**
     * Parsed program
     */
    struct program {
        struct symbol *symtab;
        struct nodelist *body;
        struct nodelist *declarations;
        struct nodelist *dependences;  // TODO
        struct symlist *parameters;
        struct reactionlist *reactions;
    };

    /**
     * Parsed reaction
     */
    struct reaction {
        struct nodelist *reactant;
        struct nodelist *product;
        struct ast *rate;
    };

    /**
     * List of parsed reactions
     */
    struct reactionlist {
        struct reaction *reac;
        struct reactionlist *next;
    };

    // Forward definitions
    extern void progdef(struct symbol *name, struct symlist *syms, struct nodelist *stmts);
    extern struct maplist **mergeprograms(
        struct symbol **progrefs,
        struct symlist **export,
        struct nodelist **params,
        int compartnum,
        int size);

#endif  // PROGRAM_H