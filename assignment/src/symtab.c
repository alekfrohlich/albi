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
#include "error.h"

int currenv = 0;                            // Current parsing environment
struct symbol globals[SYMTAB_SIZE];         // Global symbol table

/**
 * Parsing environments
 */
struct symbol *env[2] = {
    globals,
    NULL
};

/**
 * Hash function for symbol tables
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
 * Look-up key in hash table
 */
struct symbol *lookup(char *sym)
{
    // Search for symbol in both environments
    for (int i = currenv; i >= 0; i--)
    {
        struct symbol *entry = &env[i][symhash(sym) % SYMTAB_SIZE];
        int iter = SYMTAB_SIZE;

        while (--iter >= 0)
        {
            // Key already exists
            if (entry->name && !strcmp(entry->name, sym))
                return entry;

            // Loop back
            if (++entry >= env[i] + SYMTAB_SIZE)
                entry = env[i];
        }
    }

    if (nowrites)
        yyerror("undeclared variable %s", sym);

    struct symbol *entry = &env[currenv][symhash(sym) % SYMTAB_SIZE];
    int iter = SYMTAB_SIZE;

    // New symbol, insert it into current environment
    while (--iter >= 0)
    {
        if (!entry->name)
        {
            entry->name = strdup(sym);
            entry->value = 0;
            return entry;
        }

        // Loop back
        if (++entry >= env[currenv] + SYMTAB_SIZE)
            entry = env[currenv];
    }

    yyerror("internal error: symbol table overflow");
}

/**
 * Define symbol
 */
void symdef(struct symbol *sym, struct ast *val)
{
    sym->value = eval(val);
}

/**
 * Free symbol
 */
void symbolfree(struct symbol *sym)
{
    free(sym->name);
    free(sym->prog);
}

/**
 * Create symbol list
 */
struct symlist* newslist(struct symbol *sym, struct symlist *next)
{
    struct symlist *list = (struct symlist*) malloc(sizeof(struct symlist));
    list->sym = sym;
    list->next = next;
    return list;
}