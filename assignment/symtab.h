#ifndef __SYMTAB__
#define __SYMTAB__

// struct program {

// };

/**
 * Symbol.
 */
struct symbol {
    char *name;             /* variable name. */
    double value;           
    struct program *prog;
};

/**
 * Symbol table of fixed size
 */
#define SYMTAB_SIZE 9997
struct symbol symtab[SYMTAB_SIZE];

/**
 * Current parsing environment.
 */
struct symbol* env[2] = {
    symtab,
    NULL
};

int curr_env = 0;

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

struct assignlist {
    struct symassign *assign;
    struct assignlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

#endif  // __SYMTAB__