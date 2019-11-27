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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "program.h"
#include "structures.h"

/**
 * Undeclare variable as local
 * and redeclare it as specie
 */
static struct nodelist *declspecie(struct program *program,
                                   struct symassign *assign,
                                   struct nodelist *species,
                                   struct nodelist **locals) {
  char *declaree = assign->sym->name;
  struct nodelist *it = *locals;

  // Removing head?
  if (it != NULL &&
      strcmp(((struct symassign *)it->node)->sym->name, declaree) == 0) {
    *locals = (*locals)->next;
  }

  // Removing middle element?
  else if (it != NULL) {
    // Iterate until prev
    while (it->next != NULL &&
           strcmp(((struct symassign *)it->next->node)->sym->name, declaree) !=
               0) {
      it = it->next;
    }

    if (it->next == NULL)
      return species;

    struct nodelist *aux = it->next;
    it->next = it->next->next;
    it = aux;
  }

  //  Locals are NULL
  else {
    return species;
  }
  // Add located specie
  it->next = species;
  species = it;
  return species;
}

/**
 * Create reaction list
 */
static struct reactionlist *newreactionlist(struct reaction *reaction,
                                            struct reactionlist *next) {
  struct reactionlist *rl = malloc(sizeof(struct reactionlist));
  rl->reac = reaction;
  rl->next = next;
  return rl;
}

/**
 * Add reaction to program's reaction list
 */
static struct nodelist *newreaction(struct program *program, struct rate *rate,
                                    struct nodelist *species,
                                    struct nodelist **locals) {
  struct reaction *reac = (struct reaction *)malloc(sizeof(struct reaction));
  program->reactions = newreactionlist(reac, program->reactions);
  reac->rate = rate->exp;
  // Build-up reac's reactants and products,
  // also redeclare found species
  for (struct nodelist *it = rate->assigns; it != NULL; it = it->next) {
    struct symassign *assign = ((struct symassign *)it->node);

    // Invalid node
    if ((assign->val->type != PLUS && assign->val->type != MINUS) ||
        (assign->val->left->type != CONSLIT &&
         assign->val->left->type != SYM_REF) ||
        (assign->val->right->type != CONSLIT &&
         assign->val->right->type != SYM_REF)) {
      yyerrorexp("invalid expression inside rate body", assign->val);
    }

    // Reactant
    if (assign->val->type == MINUS) {
      reac->reactant = newnodelist(((struct ast *)it->node), reac->reactant);
      species = declspecie(program, assign, species, locals);
    }

    // Product
    else if (assign->val->type == PLUS) {
      reac->product = newnodelist(((struct ast *)it->node), reac->product);
      species = declspecie(program, assign, species, locals);
    }
  }
  return species;
}

/**
 * Rename variable to avoid namespace collision
 */
static char *namemergedvar(char *progname, char *varname, int compartnum) {
  char *name = (char *)malloc(100);
  snprintf(name, 100, "ECOLI%d_%s_%s", compartnum, progname, varname);
  return name;
}

/**
 * Merge program list's of declarations and return resulting namemaps
 */
struct maplist **mergeprograms(struct symbol **progrefs,
                               struct symlist **exports,
                               struct nodelist **explists, int compartnum,
                               int progcount) {
  struct maplist **maps =
      (struct maplist **)malloc(sizeof(struct maplist *) * progcount);
  for (int i = 0; i < progcount; i++) {
    maps[i] = globalmap;
  }

  for (int i = 0; i < progcount; i++) {
    struct program *prog = progrefs[i]->prog;

    // Evaluate call parameters
    struct symlist *param = prog->parameters;
    struct nodelist *exp = explists[i];
    while (param != NULL && exp != NULL) {
      param->sym->value = eval(exp->node);
      param = param->next;
      exp = exp->next;
    }

    if (param || exp)
      yyerror("wrong number of arguments to program");

    // Apply call parameters to variable, also map it's name to compartment
    // namespace
    for (struct nodelist *it = prog->declarations; it != NULL; it = it->next) {
      ((struct tsymassign *)it->node)->sym->value =
          eval(((struct tsymassign *)it->node)->val);
      char *name =
          namemergedvar(progrefs[i]->name,
                        ((struct tsymassign *)it->node)->sym->name, compartnum);
      maps[i] =
          newmaplist(((struct tsymassign *)it->node)->sym->name, name, maps[i]);
    }
  }
  return maps;
}

/**
 * Make list of declarations
 */
static void makedecls(struct program *prog) {
  struct nodelist *species = NULL;
  struct nodelist *locals = NULL;
  struct nodelist *decls = (struct nodelist *)malloc(sizeof(struct nodelist));

  // Build locals, species and reactions
  for (struct nodelist *it = prog->body; it != NULL; it = it->next) {
    if (it->node->type == SYM_ASSIGN) {
      locals = newnodelist((struct ast *)it->node, locals);
    }

    else if (it->node->type == RATESTATEMENT) {
      species = newreaction(prog, (struct rate *)it->node, species, &locals);
    }

    else {
      yyerror("internal error: invalid node inside of program");
    }
  }

  // Build declarations
  for (struct nodelist *it = prog->body, *itdec = decls; it != NULL;
       it = it->next) {
    // Skip non-assignment nodes
    if (it->node->type != SYM_ASSIGN)
      continue;

    // Add specie
    for (struct nodelist *it3 = species; it3 != NULL; it3 = it3->next) {
      if (strcmp(((struct symassign *)it3->node)->sym->name,
                 ((struct symassign *)it->node)->sym->name) == 0) {
        struct ast *a = newtassign(SPECIE, ((struct symassign *)it3->node)->sym,
                                   ((struct symassign *)it3->node)->val);
        itdec->next = newnodelist(a, NULL);
        itdec = itdec->next;
        continue;
      }
    }

    // Add local
    for (struct nodelist *it2 = locals; it2 != NULL; it2 = it2->next) {
      if (strcmp(((struct symassign *)it2->node)->sym->name,
                 ((struct symassign *)it->node)->sym->name) == 0) {
        struct ast *a = newtassign(LOCAL, ((struct symassign *)it2->node)->sym,
                                   ((struct symassign *)it2->node)->val);
        itdec->next = newnodelist(a, NULL);
        itdec = itdec->next;
        continue;
      }
    }
  }
  prog->declarations = decls->next; // First node is dummy
}

/**
 * Define program
 */
void progdef(struct symbol *name, struct symlist *syms,
             struct nodelist *stmts) {
  struct program *program = (struct program *)malloc(sizeof(struct program));
  name->prog = program;
  program->symtab = env[currenv];
  program->parameters = syms;
  program->body = stmts;
  makedecls(program);
}