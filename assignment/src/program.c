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
#include "structures.h"

// Begin Output
static void printspecies(struct assignlist * species, struct maplist * map, char * compart)
{
    //DECLARE VALUES
    struct assignlist * specie = species;
    while (specie != NULL) {
        fprintf( yyout, "Specie %s in %s\n", getmap(map, specie->assign->sym->name), compart);
        specie = specie->next;
    }

    //PRINT VALUES
    specie = species;
    while (specie != NULL) {
        fprintf( yyout, "%s = %0.4lf\n", getmap(map, specie->assign->sym->name), specie->assign->sym->value);
        specie = specie->next;
    }
}

static void printlocals(struct assignlist* locals, struct maplist * map, char * compart) {
    //DECLARE VALUES
    struct assignlist * local = locals;
    while (local != NULL) {
        fprintf(yyout, "var %s in %s\n", getmap(map, local->assign->sym->name), compart);
        local = local->next;        
    }

    //PRINT VALUES
    local = locals;
    while (local != NULL) {
        fprintf( yyout, "%s = %0.4lf\n", getmap(map, local->assign->sym->name), local->assign->sym->value);
        local = local->next;
    }
}

static void printreaction(struct reaction * reac, struct maplist * map) {
    struct assignlist * reactant = reac->reactant;
    struct assignlist * product = reac->product;
    int first = 1;
    while (reactant != NULL) {
        if (!first)
            fprintf(yyout, " ");
        else
            first = 0;
        fprintf(yyout, "%s", getmap(map, reactant->assign->sym->name));
        reactant = reactant->next;
    }
    fprintf(yyout, "-> ");
    while (product != NULL) {
        fprintf(yyout, "%s", getmap(map, product->assign->sym->name));

        if(product->next != NULL)
            fprintf(yyout, " ");
        product = product->next;
    }
    fprintf(yyout, "; %s;\n", "RATE");
}

static void printreactionlist(struct reactionlist * reactions, struct maplist* map) {
    struct reactionlist * element = reactions;
    while (element != NULL) {
        printreaction(element->reac, map);
        element = element->next;
    }
}

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


static void push_parameter(struct program * program, struct assignlist * local)
{
    local->next = program->locals;
    program->locals = local;
}

static void push_specie(struct program * program, struct assignlist * specie)
{
    specie->next = program->species;
    program->species = specie;
}

/**
 * Merged succesfully?
 */
static int empty_dependencies(struct program * program)
{
	return program->dependence == NULL;
}

static void declare_parameter(struct program * program, struct symassign * assign)
{
    struct assignlist * locals = program->locals;

    locals = newassignlist(assign, locals);
    program->locals = locals;
}

/**
 * Add new reaction to program's reaction list.
 */
static void newreaction(struct program * program, struct rate * rate)
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

static char *namemergedvar(char *progname, char * varname)
{
    char *name = (char *) malloc(100);
    char *count = (char *) malloc(20);

    snprintf(count, 20, "%d", curr_compart);
    strcat(name, "ECOLI");
    strcat(name, count);
    strcat(name, "_");
    strcat(name, progname);
    strcat(name, "_");
    strcat(name, varname);

    return name;
}

/**
 * 
 */
void mergeprograms(
    struct symbol** progrefs,
    struct symlist** export,
    struct explist** params,
    int size)
 {
    struct maplist ** map = (struct maplist **) malloc(sizeof(struct maplist*)*size);
    for (int i = 0; i < size; i++)
    {
        struct program *prog = progrefs[i]->prog;
        for (struct assignlist * specie = prog->species; specie != NULL;
             specie = specie->next)
        {
            char *name = namemergedvar(progrefs[i]->name, specie->assign->sym->name);
            map[i] = newmaplist(specie->assign->sym->name, name, map[i]);
        }
        for (struct assignlist * local = prog->locals; local != NULL;
               local = local->next)
        {
            char * name = namemergedvar(progrefs[i]->name, local->assign->sym->name);
            map[i] = newmaplist(local->assign->sym->name, name, map[i]);
        }
        printspecies(prog->species, map[i], "ECOLIE");
        printlocals(prog->locals, map[i], "ECOLIE");
    }
    for (int i = 0; i < size; i++) {
        struct program * prog = progrefs[i]->prog;
        printreactionlist(prog->reactions, map[i]);
    }
 }

 /**
 * Define new program.
 */
void progdef(struct symbol *name, struct symlist *syms, struct stmtlist * stmts)
{
    #ifdef DEBUG
        for(int i = 0; i < 9997; i++)
        {
            if ((env[0]+i)->name != 0)
                printf("%d, table 0, Symbol of name %s, value %lf and program %p\n", i, (env[0]+i)->name, (env[0]+i)->value, (env[0]+i)->prog);
            if (env[1] && (env[1]+i)->name != 0)
                printf("%d, table 1, Symbol of name %s, value %lf and program %p\n", i, (env[1]+i)->name, (env[1]+i)->value, (env[1]+i)->prog);
        }
    #endif  // DEBUG

    struct program * program = (struct program *) malloc(sizeof(struct program));
    name->prog = program;

    /**
     * Store parsing context.
     */
    program->symtab = env[curr_env];

    /**
     * Save formal parameters.
     */
    program->parameters = syms;
    struct stmtlist * iter = stmts;
    while (iter != NULL)
    {
        if (iter->stmt->type == SYM_ASSIGN)
        {
            declare_parameter(program, (struct symassign *) iter->stmt);
        }
        
        else if (iter->stmt->type == RATESTATEMENT)
        {
            newreaction(program, (struct rate *) iter->stmt);
        }
        
        else
        {
            yyerror("Invalid expression inside rate block");
        }
    
        iter = iter->next;
    }
}