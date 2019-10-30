#ifndef STRUCTS__H
#define STRUCTS__H

	typedef struct maplist * MAP;

	/**
	 * Dictionary implemented
	 * as linked list.
	 */
	struct maplist {
		struct maplist * next;
		char * key;
		char * value;
	};

	// Forward definitions.
	extern struct maplist * newmaplist(char *, char *, struct maplist *);
	extern char * getmap(struct maplist*, char *);

#endif  // STRUCTS_H