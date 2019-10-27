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

int curr_env = 1;

struct symbol symtab[SYMTAB_SIZE];

struct symbol *env[2] = {
    symtab,
    NULL
};

static unsigned symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned c;

    while (c = *sym++) hash = hash*9 ^ c;

    return hash;
}

struct symbol * lookup(char *sym)
{
    struct symbol *sp = &symtab[symhash(sym) % SYMTAB_SIZE];
    int scount = SYMTAB_SIZE;

    while (--scount >= 0)
    {
        if (sp->name && !strcmp(sp->name, sym)) { return sp; }

        // new entry.
        if (!sp->name)
        {
            sp->name = strdup(sym);
            sp->value = 0;

            return sp;
        }

        if (++sp >= symtab + SYMTAB_SIZE) sp = symtab;
    }
    
    yyerror("symbol table overflow\n");
    abort(); // tried them all, table is full.
}

void freesymbol(struct symbol * sym) {
    free(sym->name);
    free(sym->prog);
    // free(sym);
}