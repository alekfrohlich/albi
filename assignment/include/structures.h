#ifndef STRUCTS__H
#define STRUCTS__H

    /**
     * Map implemented
     * as linked list
     */
    struct maplist {
        char *key;
        char *value;
        struct maplist *next;
    };

    // Forward definitions
    extern struct maplist *newmaplist(char *key, char *value, struct maplist *next);
    extern char *getmap(struct maplist *map, char *key);

#endif  // STRUCTS_H