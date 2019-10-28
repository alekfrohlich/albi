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
#include <string.h>
#include "program.h"


// BEGIN PRIVATE INTERFACE.


/**
 * Move a specie from local's list to
 * the specie's list.
 */
static void declare_specie(struct program * program, struct symassign * assign)
{
    char * name = assign->sym->name;
    struct assignlist * iter = program->locals; /* Who is to be removed, if any. */
    
    /**
     * Removing head?
     */
    if (iter != NULL && 
        strcmp(iter->assign->sym->name, name) == 0)
    {
        program->locals = program->locals->next;
    } 
    
    /**
     * Removing middle element?
     */
    else if (iter != NULL)
    {
        
        /**
         * Find symbol in specie's list.
         */
        while (iter->next != NULL && 
            strcmp(iter->next->assign->sym->name, name) != 0)
        {
            iter = iter->next;
        }

        /**
         * Symbol is not in local's.
         * Using undeclared variable!
         */
        if (iter->next == NULL)
            yyerror("Undeclared variable");

        struct assignlist * aux = iter->next;
        iter->next = iter->next->next;
        iter = aux;
    }
    
    /**
     * Empty list.
     * Using undeclared variable!
     */
    else
    {
        yyerror("Undeclared variable");
    }
    
    /**
     * Add new specie.
     */
    iter->next = program->species;
    program->species = iter;
}

/**
 * Create new reaction list.
 */
static struct reactionlist * newreactionlist(struct reaction * reaction, struct reactionlist * next)
{
    struct reactionlist * rl = malloc(sizeof(struct reactionlist));
    rl->reac = reaction;
    rl->next = next;
    return rl;
}


// BEGIN PUBLIC INTERFACE.

/**
 * Merged succesfully?
 */
int empty_dependencies(struct program * program)
{
	return program->dependence == NULL;
}

void declare_parameter(struct program * program, struct symassign * assign)
{
    struct assignlist * locals = program->locals;

    locals = newassignlist(assign, locals);
    program->locals = locals;
}

/**
 * Add new reaction to program's reaction list.
 */
void newreaction(struct program * program, struct rate * rate)
{
    struct reaction * reac = (struct reaction *) malloc(sizeof(struct reaction));
 
    program->reactions = newreactionlist(reac, program->reactions);
    reac->rate = rate->exp;
    
    /**
     * Classifying the rate assignments into
     * reactans and products. Also move them from
     * the local's list to the specie's list.
     */
    for (struct assignlist * iter = rate->assigns; iter != NULL; iter = iter->next)
    {
        if (iter->assign->val->type == MINUS)
        {
            reac->reactant = newassignlist(iter->assign, reac->reactant);
            declare_specie(program, iter->assign);
        } 
        
        else if (iter->assign->val->type == PLUS)
        {
            reac->product = newassignlist(iter->assign, reac->product);
            declare_specie(program, iter->assign);
        }
        
        /**
         * Product/reactant not expressed
         * as A := A + 1 or B := B -1.
         */
        else
        {
            yyerror("Invalid expression");
        }
    }
}