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

#define NUMNODETYPES 15

/**
     * AST node types
     */
enum nodetypes {
    CONSLIT = 0,    // Constant literal
    PLUS,           // Arithmetic expression: A + B
    MINUS,          // Arithmetic expression: A - B
    TIMES,          // Arithmetic expression: A * B
    DIV,            // Arithmetic expression: A / B
    MOD,            // Arithmetic expression: A % B
    POW,            // Arithmetic expression: A ^ B
    UMINUS,         // Arithmetic expression: -A
    NODELIST,       // Grouping of nodes
    SYM_REF,        // Symbol reference
    SYM_ASSIGN,     // Symbol assignment
    T_SYM_ASSIGN,   // Typed symbol assignment
    COMPART,        // Compartment
    RATESTATEMENT,  // Reaction rate
    BUILTIN,        // Built-in function
};

/**
     * SBML types
     */
enum sbmltypes {
    SPECIE = 0,  // Specie
    LOCAL,       // Var
};

/**
     * Built-in compiler functions
     */
enum funtypes {
    __builtin_sin = 0,
    __builtin_cos,
    __builtin_tan,
    __builtin_ln,
    __builtin_log,
    __builtin_ceil,
    __builtin_floor,
    __builtin_sqrt,
};

/**
     * Generic expression node
     */
struct ast {
    enum nodetypes type;
    struct ast * left;
    struct ast * right;
};

/**
     * Compartment instantiation node
     */
struct compart {
    enum nodetypes type;
    char * name;
    struct progcall * call;  // E. coli parameters
};

/**
     * Numeric value node
     */
struct numval {
    enum nodetypes type;
    double number;
};

/**
     * Symbol reference node
     */
struct symref {
    enum nodetypes type;
    struct symbol * sym;
};

/**
     * Symbol assignment node
     */
struct symassign {
    enum nodetypes type;
    struct symbol * sym;
    struct ast * val;
};

/**
     * Typed symbol assignment node
     */
struct tsymassign {
    enum nodetypes type;
    enum sbmltypes _type;
    struct symbol * sym;
    struct ast * val;
};

/**
     * Rate expresion node
     */
struct rate {
    enum nodetypes type;
    struct ast * exp;
    struct nodelist * assigns;
};

/**
     * Built-in function call
     */
struct funcall {
    enum nodetypes type;
    enum funtypes _type;
    struct ast * exp;
};

// Forward definitions
struct ast * newast(enum nodetypes type, struct ast * left, struct ast * right);
struct ast * newcompart(char * sym, struct progcall * progcall);
struct ast * newnum(double d);
struct ast * newref(struct symbol * sym);
struct ast * newassign(struct symbol * sym, struct ast * val);
struct ast * newtassign(
    enum sbmltypes type, struct symbol * sym, struct ast * val);
struct ast * newrate(struct ast * exp, struct nodelist * assigns);
struct ast * newfuncall(enum funtypes type, struct ast * exp);
void treefree(struct ast * a);

#endif  // AST_H