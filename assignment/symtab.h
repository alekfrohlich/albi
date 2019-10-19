#ifndef __SYMTAB__
#define __SYMTAB__

/**
 * Symbol.
 */
struct symbol {
    char *name;             /* variable name. */
    double value;           
    struct ast *prog;       /* statement for the program. */
    struct symlist *args;   /* list of args. */
};

/**
 * Symbol table of fixed size
 */
#define SYMTAB_SIZE 9997
struct symbol symtab[SYMTAB_SIZE];

/**
 * Look-up symbol in symbol table.
 */
struct symbol *lookup(char *);

/** 
 * List of symbols,
 * for an argument list.
 */
struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

#endif  // __SYMTAB__