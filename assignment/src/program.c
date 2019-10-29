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
 * the specie's list.
 */
static void declare_specie(
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
static void newreaction(
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
            reac->reactant = newnodelist((struct ast *) iter->node, reac->reactant);
            declare_specie(program, (struct symassign *)iter->node, species, locals);
        } 
        
        else if (((struct symassign *)iter->node)->val->type == PLUS)
        {
            reac->product = newnodelist((struct ast *) iter->node, reac->product);
            declare_specie(program, (struct symassign *)iter->node, species, locals);
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
// void mergeprograms(
//     struct symbol** progrefs,
//     struct nodelist** export,
//     struct nodelist** explist,
//     int compartnum,
//     int size)
//  {
//     struct maplist ** map = (struct maplist **) malloc(sizeof(struct maplist*)*size);
//     for (int i = 0; i < size; i++)
//     {
//         struct program *prog = progrefs[i]->prog;
//         for (struct nodelist *specie = prog->species; specie != NULL;
//              specie = specie->next)
//         {
//             char *name = namemergedvar(progrefs[i]->name, ((struct symassign *)specie->node)->sym->name, compartnum);
//             map[i] = newmaplist(((struct symassign *)specie->node)->sym->name, name, map[i]);
//         }
//         for (struct nodelist * local = prog->locals; local != NULL;
//                local = local->next)
//         {
//             char * name = namemergedvar(progrefs[i]->name, ((struct symassign *)local->node)->sym->name, compartnum);
//             map[i] = newmaplist(((struct symassign *)local->node)->sym->name, name, map[i]);
//         }
//         printspecies(prog->species, map[i], "ECOLIE");
//         printlocals(prog->locals, map[i], "ECOLIE");
//     }
//     for (int i = 0; i < size; i++) {
//         struct program * prog = progrefs[i]->prog;
//         printreactionlist(prog->reactions, map[i]);
//     }
//  }

/**
 * Make list of declarations.
 */
static void makedecls(struct program *prog)
{
    struct nodelist *locals = NULL;
    struct nodelist *species = NULL;
    struct nodelist *decls = NULL;
    printf("BEGINNING\n");

    /**
     * Build locals, species and reactions.
     */
    for (struct nodelist *iter = prog->body; iter != NULL; iter = iter->next)
    {
        if (iter->node->type == SYM_ASSIGN)
        {
            locals = newnodelist((struct ast *) iter->node, locals);
        }
        
        else if (iter->node->type == RATESTATEMENT)
        {
            newreaction(prog, (struct rate *) iter->node, species, locals);
        }
        
        else
        {
            yyerror("Invalid expression inside rate block");
        }
    }

    printf("MIDDLE\n");

    struct nodelist *it1 = prog->body;
    struct nodelist *it2 = locals;
    struct nodelist *it3 = species;
    struct nodelist *it4 = decls;

    /**
     * Build decls.
     */
    while (it1 != NULL)
    {
        /**
         * Skip non-assignment nodes.
         */
        if (it1->node->type != SYM_ASSIGN)
        {
            it1 = it1->next;
            continue;
        }

        /**
         * Add locals to declarations.
         */
        if (it2 != NULL && strcmp(((struct symassign *) it2->node)->sym->name,
            ((struct symassign *) it1->node)->sym->name) == 0)
        {
            struct ast *a = newtassign(LOCAL, ((struct symassign *) it2->node)->sym,
                    ((struct symassign *) it2->node)->val);
            // free(it2->node); ?
            it4 = newnodelist(a, it4);
            it2 = it2->next;
            it1 = it1->next;
            continue;
        }

        /**
         * Add species to declarations.
         */
        if (it3 != NULL && strcmp(((struct symassign *) it3->node)->sym->name,
            ((struct symassign *) it1->node)->sym->name) == 0)
        {
            struct ast *a = newtassign(LOCAL, ((struct symassign *) it3->node)->sym,
                    ((struct symassign *) it3->node)->val);
            // free(it3->node); ?
            it4 = newnodelist(a, it4);
            it3 = it3->next;
            it1 = it1->next;
            continue;
        }
    }

    printf("END\n");

    prog->declarations = decls;
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