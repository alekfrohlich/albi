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

#ifndef __AST_H__
#define __AST_H__

/**
 * AST node.
 */
struct node {
	int type;
	struct node * left;
	struct node * right;
	
	char * compartment;
	char * var;
	int n_prod, n_reac;
	float value;
};

/**
 * AST interface.
 */
void search_program(struct node * node, char * compartment);
void search_tree(struct node * node);
struct node * new_node(int type, struct node * left_of, struct node * right_of);
struct node * head;

/**
 * AST node types.
 */
#define START_OF_PROG_TYPE 1
#define RATE_TYPE 2
#define PARAMETER_TYPE 3
#define SPECIE_TYPE 4
#define STATEMENT_TYPE 5

#endif  // __AST_H__