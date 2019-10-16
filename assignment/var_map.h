#include <stdlib.h>
#include <stdio.h>

typedef struct m_node {

	char * key;
	float value;
	struct m_node * left;
	struct m_node * right;
} map_node;

/*

*/
float map_get(map_node * root, char * key);

/*

*/
float map_insert(map_node * root, char * key, float value);

/*

*/
int map_contains(map_node * root, char * key);
