#ifndef STRUCTS__H
#define STRUCTS__H

	/**
	 * Map implemented
	 * as linked list
	 */
	struct maplist {
		struct maplist *next;
		char *key;
		char *value;
	};

	// Forward definitions
	extern struct maplist *newmaplist(char *key, char *value, struct maplist *next);
	extern char *getmap(struct maplist *map, char *key);

#endif  // STRUCTS_H