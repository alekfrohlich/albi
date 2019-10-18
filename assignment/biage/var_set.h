#include <stdlib.h>
#include <stdio.h>

typedef struct s_node {

	char * value;
	struct s_node * left;
	struct s_node * right;
} set_node;

/*

*/
int set_insert(set_node * root, char * value);

/*

*/
int set_contains(set_node * root, char * value);
