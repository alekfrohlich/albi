
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "program.h"

void declare_parameter(struct program * program, struct symassign * assign)
{
    struct assignlist * locals = program->locals;

    locals = newassignlist(assign, locals);
    program->locals = locals;
}

/**
 * Move a specie from local's list to
 * the specie's list.
 */
static void declare_specie(struct program * program, struct symassign * assign)
{
    // Symbol name.
    char * var = assign->sym->name;

    struct assignlist * locals = program->locals; // ta uma merda (iter). QUEM REMOVEU
    
    // Remove head of local's list.
    if (locals != NULL && 
        strcmp(locals->assign->sym->name, var) == 0)
    {
    
        program->locals = program->locals->next;
    
    } 
    
    // Remove middle element.
    else if (locals != NULL)
    {
        
        // Find symbol in locals.
        while (locals->next != NULL && 
            strcmp(locals->next->assign->sym->name, var) != 0)
        {
            locals = locals->next;
        }

        /**
         * Symbol is not in local's.
         * Using undeclared variable!
         */
        if (locals->next == NULL)
        {
            yyerror("Undeclared variable");
            abort();
        }

        struct assignlist * aux = locals->next;
        locals->next = locals->next->next;
        locals = aux;
    }
    
    /**
     * Empty list.
     * Using undeclared variable!
     */
    else
    {
        yyerror("Undeclared variable");
        abort();
    }
    
    /**
     * Add new specie.
     */
    locals->next = program->species;
    program->species = locals;
}

int valid_dependence(struct program * program)
{
	return program->dependence == NULL;
}

static struct assignlist * declare_reactant(struct reaction * reaction, struct symassign* assign)
{
    reaction->reactant = newassignlist(assign, reaction->reactant);
    return reaction->reactant;
}

static struct assignlist * declare_product(struct reaction * reaction, struct symassign* assign)
{
    reaction->product = newassignlist(assign, reaction->product);
    return reaction->product;
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
    for (struct assignlist * list = rate->assigns; list != NULL; list = list->next)
    {
        if (list->assign->val->type == MINUS)
        {
            declare_reactant(reac, list->assign);
            declare_specie(program, list->assign);
        } 
        
        else if (list->assign->val->type == PLUS)
        {
            declare_product(reac, list->assign);
            declare_specie(program, list->assign);
        }
        
        else // Invalid expression
        {
        }
    }
}