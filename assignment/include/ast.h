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

#ifndef AST_H
#define AST_H

    #include "symtab.h"

    /**
     * AST node types.
     */
    enum nodetypes {
        CONSLIT = 1,
        PLUS,
        MINUS,
        TIMES,
        DIV,
        EXPLIST,
        SYM_REF,
        SYM_ASSIGN,
        COMPART,
        RATESTATEMENT,
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
    struct compart {
        enum nodetypes type;
        char *sym;
        struct calllist *params;
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

    /**
     * Rate expresion node.
     */
    struct rate {
        enum nodetypes type;
        struct ast *exp;
        struct nodelist *assigns;
    };

    // Forward definitions.
    extern struct ast *newast(enum nodetypes type, struct ast *l, struct ast *r);
    extern struct ast *newcompart(char *sym, struct calllist *params);
    extern struct ast *newnum(double d);
    extern struct ast *newref(struct symbol *sym);
    extern struct ast *newassign(struct symbol *s, struct ast *v);
    extern struct ast *newrate(struct ast* exp, struct nodelist *assigns);
	extern void treefree(struct ast *a);

#endif  // AST_H