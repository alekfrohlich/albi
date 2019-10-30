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
#include "output.h"

/**
 * Move a specie from local's list to
 * the specie's list and returns it.
 */
static struct nodelist *declare_specie(
    struct program *program,
    struct symassign *assign,
    struct nodelist *species,
    struct nodelist *locals)
{
    char * name = assign->sym->name;
    struct nodelist * iter = locals; /* Who is to be removed, if any. */
    
    /**
     * Removing head?
     */
    if (iter != NULL && 
        strcmp(((struct symassign *) iter->node)->sym->name, name) == 0)
    {
        locals = locals->next;
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
            strcmp(((struct symassign *)iter->next->node)->sym->name, name) != 0)
        {
            iter = iter->next;
        }

        /**
         * Symbol is not in local's.
         * Using undeclared variable!
         */
        if (iter->next == NULL)
            yyerror("Undeclared variable");

        struct nodelist * aux = iter->next;
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
    iter->next = species;
    species = iter;
    return species;
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

/**
 * Add new reaction to program's reaction list.
 */
static struct nodelist *newreaction(
    struct program *program,
    struct rate *rate,
    struct nodelist *species,
    struct nodelist *locals)
{
    struct reaction * reac = (struct reaction *) malloc(sizeof(struct reaction));
    program->reactions = newreactionlist(reac, program->reactions);
    reac->rate = rate->exp;
    
    /**
     * Classifying the rate assignments into
     * reactans and products. Also move them from
     * the local's list to the specie's list.
     */
    for (struct nodelist * iter = rate->assigns; iter != NULL; iter = iter->next)
    {
        if (((struct symassign *)iter->node)->val->type == MINUS)
        {
            reac->reactant = newnodelist(((struct ast *) iter->node), reac->reactant);
            species = declare_specie(program, ((struct symassign *) iter->node), species, locals);
        } 
        
        else if (((struct symassign *)iter->node)->val->type == PLUS)
        {
            reac->product = newnodelist(((struct ast *) iter->node), reac->product);
            species = declare_specie(program, ((struct symassign *) iter->node), species, locals);
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
    return species;
}

/**
 * Rename variables to avoid
 * namespace collisions.
 */
static char *namemergedvar(char *progname, char *varname, int compartnum)
{
    char *name = (char *) malloc(100);
    snprintf(name, 100, "ECOLI%d_%s_%s", compartnum, progname, varname);
    return name;
}

/**
 * Merge program list's of declarations.
 */
MAP* mergeprograms(
    struct symbol** progrefs,
    struct nodelist** export,
    struct nodelist** explist,
    int compartnum,
    int size)
 {
     /**
      * Program to compartment variable name dictionary.
      */
    struct maplist ** map = (struct maplist **) malloc(sizeof(struct maplist*)*size);

    /**
     * Iterate over all programs in call list.
     */
    for (int i = 0; i < size; i++)
    {
        // Working program.
        struct program *prog = progrefs[i]->prog;

        /**
         * Evaluate call parameters.
         */
        struct nodelist *param = prog->parameters, *exp = explist[i];
        while (param != NULL)
        {
            ((struct symbol *) param->node)->value = eval(exp); // recover symlist.
            param = param->next;
        }

        if (param || exp)
            yyerror("Wrong number of arguments to program!");

        /**
         * Apply call parameters.
         */
        for (struct nodelist *decl = prog->declarations; decl != NULL; decl = decl->next)
        {
            ((struct tsymassign*) decl->node)->sym->value = eval(((struct tsymassign*) decl->node)->val);
        }

        /**
         * Map working program variables
         * to compartment namespace.
         */
        for (struct nodelist *decl = prog->declarations; decl != NULL; decl = decl->next)
        {
            char *name = namemergedvar(progrefs[i]->name,
                    ((struct tsymassign *)decl->node)->sym->name, compartnum);
            map[i] = newmaplist(((struct tsymassign *)decl->node)->sym->name, name, map[i]);
        }
    }

    return map;
 }

/**
 * Make list of declarations.
 */
static void makedecls(struct program *prog)
{
    struct nodelist *species = NULL;
    struct nodelist *locals = NULL;
    struct nodelist *decls = (struct nodelist*) malloc(sizeof(struct nodelist));

    /**
     * Build locals, species and reactions.
     */
    for (struct nodelist *it = prog->body; it != NULL; it = it->next)
    {
        if (it->node->type == SYM_ASSIGN)
        {
            locals = newnodelist((struct ast *) it->node, locals);
        }
        
        else if (it->node->type == RATESTATEMENT)
        {
            species = newreaction(prog, (struct rate *) it->node, species, locals);
        }
        
        else
        {
            yyerror("Invalid expression inside rate block");
        }
    }

    /**
     * Build decls.
     */
    for (struct nodelist *it = prog->body, *itdec = decls; it != NULL; it = it->next)
    {
        /**
         * Skip non-assignment nodes.
         */
        if (it->node->type != SYM_ASSIGN)
            goto mov;

        /**
         * Add species to declarations.
         */
        for (struct nodelist *it3 = species; it3 != NULL; it3 = it3->next)
        {
            if (strcmp(((struct symassign *) it3->node)->sym->name,
                ((struct symassign *) it->node)->sym->name) == 0)
            {
                struct ast *a = newtassign(SPECIE, ((struct symassign *) it3->node)->sym,
                        ((struct symassign *) it3->node)->val);
                // free(species->node); ?
                itdec->next = newnodelist(a, NULL);
                itdec = itdec->next;
                goto mov;
            }
        }

        /**
         * Add locals to declarations.
         */
        for (struct nodelist *it2 = locals; it2 != NULL; it2 = it2->next)
        {
            if (strcmp(((struct symassign *) it2->node)->sym->name,
                ((struct symassign *) it->node)->sym->name) == 0)
            {
                struct ast *a = newtassign(LOCAL, ((struct symassign *) it2->node)->sym,
                        ((struct symassign *) it2->node)->val);
                // free(locals->node); ?
                itdec->next = newnodelist(a, NULL);
                itdec = itdec->next;
                goto mov;
            }
        }

    mov:
        continue;
    } 
    prog->declarations = decls->next;
}

 /**
  * Define new program.
  */
void progdef(struct symbol *name, struct nodelist *syms, struct nodelist * stmts)
{
    struct program * program = (struct program *) malloc(sizeof(struct program));
    name->prog = program;
    program->symtab = env[curr_env];
    program->parameters = syms;
    program->body = stmts;
    makedecls(program);
}