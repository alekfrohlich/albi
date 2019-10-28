#include <stdlib.h>
#include <stdio.h>
#include "parsing.h"
#include "ast.h"

#ifndef __PROGRAM__
#define __PROGRAM__

struct program {
	struct symbol ** symtab;
    struct symlist * parameters;
	
    struct assignlist * species;
	struct assignlist * locals;

	struct symlist * dependence;

	struct reactionlist * reactions;
    int reactions_size;
};

struct reactionlist {
    struct reaction * reac;
    struct reactionlist * next;
};

struct reaction {
    struct assignlist * reactant;
    struct assignlist * product;
    struct ast * rate;
};

void declare_parameter(struct program * program, struct symassign * assign);
int valid_dependence(struct program * program);
void newreaction(struct program *, struct rate *);

#endif  // __PROGRAM__