#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "var_map.h"

float map_get(map_node * root, char * key) {
	
	if (root == NULL) {
		return -1;
	}

	int cmp = strcmp(root->key, key);
	if (cmp > 0) {
		return map_get(root->right, key);
	} else if (cmp < 0) {
		return map_get(root->left, key);
	}
	
	return root->value;
}

float map_insert(map_node * root, char * key, float value) {

	if (root == NULL) {
		root = (map_node *) malloc(sizeof(map_node));
		root->value = value;
		strcpy(root->key, key);
		return value;
	}

	int cmp = strcmp(root->key, key);
	if (cmp > 0) {
		return map_insert(root->right, key, value);
	} else if (cmp < 0) {
		return map_insert(root->left, key, value);
	}
	
	return root->value;
}

int map_contains(map_node * root, char * key) {
	
	if (root == NULL) {
		return 0;
	}

	int cmp = strcmp(root->key, key);
	if (cmp > 0) {
		return map_get(root->right, key);
	} else if (cmp < 0) {
		return map_get(root->left, key);
	}
	
	return 1;
}
