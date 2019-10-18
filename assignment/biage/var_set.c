#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "var_set.h"

int set_insert(set_node * root, char * value) {

	if (root == NULL) {
		root = (set_node *) malloc(sizeof(set_node));
		root->value = value;
		return 1;
	}

	int cmp = strcmp(root->value, value);
	if (cmp > 0) {
		return set_insert(root->right, value);
	} else if (cmp < 0) {
		return set_insert(root->left, value);
	}
	
	return 0;
}

int set_contains(set_node * root, char * value) {
	
	if (root == NULL) {
		return 0;
	}

	int cmp = strcmp(root->value, value);
	if (cmp > 0) {
		return set_contains(root->right, value);
	} else if (cmp < 0) {
		return set_contains(root->left, value);
	}
	
	return 1;
}