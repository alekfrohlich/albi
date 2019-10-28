
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

void declare_parameter(struct program * program, struct symassign * assign) {
    struct assignlist * locals = program->locals;

    locals = newassignlist(assign, locals);
    program->locals = locals;
}

void declare_specie(struct program * program, struct symassign * assign) {

    char * var = assign->sym->name;

    struct assignlist * locals = program->locals;
    
    if (locals != NULL && 
    strcmp(locals->assign->sym->name, var) == 0) {
    
        program->locals = program->locals->next;
    
    } else if (locals != NULL) {
        
        while (locals->next != NULL && 
        strcmp(locals->next->assign->sym->name, var) != 0) {
            locals = locals->next;
        }

        if (locals->next == NULL) {
            //var does not exist
            //GENERATE ERROR
            return;
        }

        struct assignlist * aux = locals->next;
        locals->next = locals->next->next;
        locals = aux;
    } else {
        //var does not exist
        //GENERATE ERROR
        return;
    }
    
    struct assignlist * specie = program->species;
    if (specie == NULL) {
        program->species = locals;
    } else {
        while(specie->next != NULL) {
            specie = specie->next;
        }
        specie->next = locals;
    }
}

int valid_dependence(struct program * program) {
	return program->dependence == NULL;
}

struct assignlist * declare_reactant(struct reaction * reaction, struct symassign* assign) {
    reaction->reactant = newassignlist(assign, reaction->reactant);
    return reaction->reactant;
}

struct assignlist * declare_product(struct reaction * reaction, struct symassign* assign) {
    reaction->product = newassignlist(assign, reaction->product);
    return reaction->product;
}

int is_reactant(struct reaction * reac, struct symassign * assign) {
    return 0;
}

int is_product(struct reaction * reac,  struct symassign * assign) {
    return 0;
}

struct reactionlist * newreactionlist(struct reaction * reaction, struct reactionlist * next) {
    struct reactionlist * rl = malloc(sizeof(struct reactionlist));
    rl->reac = reaction;
    rl->next = next;
    return rl;
}

void newreaction(struct program * program, struct rate * rate) {
    struct reaction * reac = (struct reaction *) malloc(sizeof(struct reaction));
    program->reactions = newreactionlist(reac, program->reactions);

    reac->rate = rate->exp;
    struct assignlist * list;
    while (list != NULL) {
        if (list->assign->val->type == MINUS) {
            declare_reactant(reac, list->assign);
            declare_specie(program, list->assign);
        } else if (list->assign->val->type == PLUS) {
            declare_product(reac, list->assign);
            declare_specie(program, list->assign);
        } else {
            //incorrect format
            //GENERATE ERRO
        }
        list = list->next;
    }
}
