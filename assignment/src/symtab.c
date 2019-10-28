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

#include <string.h>
#include <stdlib.h>
#include "symtab.h"
#include "parsing.h"

/**
 * Start saving to global context.
 */
int curr_env = 0;
struct symbol globals[SYMTAB_SIZE];

struct symbol *env[2] = {
    globals,
    NULL
};

/**
 * Hashes a symbol by multiplying by 9
 * and xor'ing with the last result.
 */
static unsigned symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned c;

    while (c = *sym++)
        hash = hash*9 ^ c;

    return hash;
}

/**
 * Look-up key in hash table.
 */
struct symbol *lookup(char *sym)
{   

    for (int i = curr_env; i >= 0; i--)
    {
        struct symbol *entry = &env[i][symhash(sym) % SYMTAB_SIZE];
        int iter = SYMTAB_SIZE;

        /**
         * Try all entries on global symbol table.
         */
        while (--iter >= 0)
        {
            /**
             * Key already exists, so return it!
             */
            if (entry->name && !strcmp(entry->name, sym))
                return entry;

            /**
             * Loop back to start of table.
             */
            if (++entry >= env[i] + SYMTAB_SIZE)
                entry = env[i];
        }

        /**
         * Tried all global entries whilst
         * not inside a program.
         */
    }

    struct symbol *entry = &env[curr_env][symhash(sym) % SYMTAB_SIZE];
    int iter = SYMTAB_SIZE;

    /**
     * Key doesn't exist,
     * create new entry for it!
     */
    while (--iter >= 0)
    {
        if (!entry->name)
        {
            entry->name = strdup(sym);
            entry->value = 0;

            return entry;
        }

        /**
         * Loop back to start of table.
         */
        if (++entry >= env[curr_env] + SYMTAB_SIZE)
            entry = env[curr_env];
    }

    /**
     * Tried all local entries, symbol table full.
     */
    yyerror("symbol table overflow\n");
}

/**
 * Define new symbol.
 */
void symdef(struct symbol *sym, struct ast *val)
{
    sym->value = eval(val);
}

/**
 * Clean's a symbol dynamic memory
 * consumption.
 */
void symbolfree(struct symbol * sym)
{
    free(sym->name);
    free(sym->prog);
}