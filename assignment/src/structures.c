#include "structures.h"
#include <string.h>
struct maplist * newmaplist(char * key, char *value, struct maplist * next) {
	struct maplist * ml = (struct maplist *) malloc(sizeof(struct maplist));
	ml->key = key;
	ml->value = value;
	ml->next = next;
	return ml;
}

char * getmap(struct maplist* list, char * key) {
	if (list == NULL)
		return NULL;
	if (strcmp(list->key, key) == 0)
		return list->value;
	return getmap(list->next, key);
}