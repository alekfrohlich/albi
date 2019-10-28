#include <stdlib.h>
#include <stdio.h>
#include "parsing.h"
#include "ast.h"

#define NOT_FOUND 0

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

// program
void declare_specie(struct program * program, struct symassign * assign);
void declare_parameter(struct program * program, struct symassign * assign);
int is_specie(struct program * program, char * var);
int is_parameter(struct program * program, char * var);
struct program * program_copy(struct program * program);
int valid_dependence(struct program * program);

// reaction
struct assignlist * declare_reactant(struct reaction *, struct symassign * assign);
struct assignlist * declare_product(struct reaction *, struct symassign * assign);
int is_reactant(struct reaction * reac, struct symassign * assign);
int is_product(struct reaction * reac,  struct symassign * assign);
struct reactionlist * newreactionlist(struct reaction *, struct reactionlist *);
void newreaction(struct program *, struct rate *);
