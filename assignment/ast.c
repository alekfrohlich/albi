/*	  
 *    Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com> 
 *    & Gustavo Biage <gustavo.c.biage@gmail.com>.
 *
 * 	  This file is a part of Albi.
 * 
 *    Albi is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Albi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <string.h>
#include "ast.h"

/**
 * Construct new AST node.
 */
struct node * new_node(int type, struct node * left_of, struct node * right_of) {
	
	struct node * node = (struct node *) malloc(sizeof(struct node));

	node->type = type;
	
	if (left_of != NULL)
		left_of->left = node;

	if (right_of != NULL)
		right_of->right = node;

	return node;
}

void search_tree(struct node * node, FILE * yyout) {
	switch(node->type) {
		case START_OF_PROG_TYPE:
			if (node->right != NULL) {
				search_tree(node->right, yyout);
			}
			if (node->left != NULL) {
				search_tree(node->left, yyout);
			}
			break;
		case PARAMETER_TYPE:
			fprintf(yyout, "%s = %0.4lf\n", node->var, node->value);
			break;
		case RATE_TYPE:
		case SPECIE_TYPE:
			fprintf(yyout, "compartment %s = 1.0\n",  node->compartment);
			// fprintf(yyout, "%s = 1.0;\n", node->compartment);
			search_program(node, node->compartment, yyout);
			break;
	}
}

void search_program(struct node * node, char * compartment, FILE * yyout) {
	
	if (node->right != NULL) {
		search_program(node->right, compartment, yyout);
	}

	if (node->type == SPECIE_TYPE) {
		// fprintf(yyout, "specie %s\n", node->var);
		fprintf(yyout, "species %s;\n", node->var);
		fprintf(yyout, "%s in %s\n", node->var, compartment);
		fprintf(yyout, "%s = %0.4lf\n", node->var, node->value);
	} else if(node->type == RATE_TYPE) {
		for (int i = 0; i < node->n_reac; i++) {
			fprintf(yyout, "%s ", node->reac[i]);
		}
		fprintf(yyout, "->");
		for (int i = 0; i < node->n_prod - 1; i++) {
			if (i == 0) {
				fprintf(yyout, "s ");
			}
			fprintf(yyout, "%s ", node->prod[i]);
		}

		if (node->n_prod > 0 && node->n_prod != 1)
			fprintf(yyout, "%s; %s\n", node->prod[node->n_prod - 1], node->var);
		else if (node->n_prod == 1)
			fprintf(yyout, " $%s; %s\n", node->prod[node->n_prod - 1], node->var);
	}
}
