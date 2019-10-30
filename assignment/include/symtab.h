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

#ifndef SYMTAB_H
#define SYMTAB_H

    /**
     * Size of symbol table.
     */
    #define SYMTAB_SIZE 9997

    /**
     * Symbol table entry.
     */
    struct symbol {
        char *name;
        double value;           
        struct program *prog;
    };

    /**
     * List of symbols.
     */
    struct symlist {
        struct symbol *sym;
        struct symlist *next;
    };

    // forward defined.
    struct ast;
    extern int curr_env;
    extern struct symbol* env[2];
    extern struct symbol *lookup(char *sym);
    extern struct symlist *newslist(struct symbol *sym, struct symlist *next);
    extern void symdef(struct symbol *sym, struct ast *val);
    extern void freesymbol(struct symbol * sym);

#endif  // SYMTAB_H