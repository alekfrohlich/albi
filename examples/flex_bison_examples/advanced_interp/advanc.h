

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char *s, ...);

// BEGIN SYMBOL TABLE INTERFACE

/* symbol. */
struct symbol {
    char *name;             /* a variable name */
    double value;           
    struct ast *func;       /* statement for the function */
    struct symlist *syms;   /* list of args. */
};

/* simple symtab of fixed size */
#define SYMTAB_SIZE 9997
struct symbol symtab[SYMTAB_SIZE];

struct symbol *lookup(char *);

/* list of symbols, for an argument list. */
struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

// END SYMBOL TABLE INTERFACE

// BEGIN AST INTERFACE

/** node types
 * + - * / |
 * 0-7 comparison ops, bit coded 04 equalm 02 less, 01 greater 
 * M unary minus.
 * L expression or statement list
 * I IF statement
 * W WHILE statement
 * N symbol ref
 * = assignment
 * S list of symbols
 * F built in function call
 * C user function call
 */

enum __builtins {
    __builtin_sqrt = 1,
    __builtin_exp,
    __builtin_log,
    __builtin_print
};

/**
 * nodes in the abstract syntax tree,
 * all have common initial nodetype.
 */

/* nodes in the abstract syntax tree */
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall {
    int nodetype;   /* built-in function. */
    struct ast *l;  /* type F */
    enum __builtins functype;
};

struct ufncall {
    int nodetype;        /* user function. */
    struct ast *l;      /* type C. */
    struct symbol *s;   /* list of arguments. */
};

struct flow {
    int nodetype;      /* type I or W. */
    struct ast *cond; /* condition. */
    struct ast *tl;   /* then branch or do list. */
    struct ast *el;   /* optional else branch. */
};

struct numval {
    int nodetype;     /* type K for constant. */
    double number;
};

struct symref {
    int nodetype;    /* type N. */
    struct symbol *s;
};

struct symassign {
    int nodetype;   /* type =. */
    struct symbol *s;
    struct ast *v;  /* value. */
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newassign(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* define a function. */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

// END AST INTERFACE