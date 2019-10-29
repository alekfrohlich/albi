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

    struct program {
        struct symbol * symtab;
        struct nodelist * parameters;
        
        struct nodelist * species;
        struct nodelist * locals;

        struct nodelist * dependences;  // ignored.

        struct reactionlist * reactions;
        int reactions_size;
    };

    struct reaction {
        struct nodelist * reactant;
        struct nodelist * product;
        struct ast * rate;
    };

    struct reactionlist {
        struct reaction * reac;
        struct reactionlist * next;
    };

    // Forward definitions.
    extern void progdef(struct symbol *name, struct nodelist *syms, struct nodelist *stmts);
    extern void mergeprograms(
        struct symbol** progrefs,
        struct nodelist** export,
        struct nodelist** params,
        int compartnum,
        int size);

#endif  // PROGRAM_H