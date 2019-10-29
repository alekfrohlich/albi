#ifndef STRUCTS__H
#define STRUCTS__H

	struct maplist {
		struct maplist * next;
		char * key;
		char * value;
	};

	extern struct maplist * newmaplist(char *, char *, struct maplist *);
	extern char * getmap(struct maplist*, char *);

#endif  // STRUCTS_H