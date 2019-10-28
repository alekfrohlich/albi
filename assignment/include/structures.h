#ifndef __COMMON_STRUCTS__
#define __COMMON_STRUCTS__

#include <stdlib.h>

struct maplist {
	struct maplist * next;
	char * key;
	char * value;
};

extern struct maplist * newmaplist(char *, char *, struct maplist *);
extern char * getmap(struct maplist*, char *);

#endif  // __COMMON_STRUCTS__