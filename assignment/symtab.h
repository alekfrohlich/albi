#ifndef __SYMTAB__
#define __SYMTAB__

struct symbol {
    char *name;
    double value;           
    struct program *prog;
};

#define SYMTAB_SIZE 9997

struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct assignlist {
    struct symassign *assign;
    struct assignlist *next;
};

// extern defines.
extern struct symbol symtab[SYMTAB_SIZE];
extern struct symbol* env[2];
extern int curr_env;
extern struct symbol *lookup(char *);
extern struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
extern void symlistfree(struct symlist *sl);

#endif  // __SYMTAB__