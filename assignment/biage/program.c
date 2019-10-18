#include <stdlib.h>
#include <stdio.h>
#include "program.h"

#define NOT_FOUND 0

void declare_specie(t_program * program, char * var) {
	if (!map_contains(program->variable_list, var)) {
		set_insert(program->species_list, var);
	} else {
		// GENERATE ERROR
		// yyerror();
	}
}

void declare_parameter(t_program * program, char * var) {
	if (!map_contains(program->parameter_list, var)) {
		set_insert(program->parameter_list, var);
	} else {
		// GENERATE ERROR
		// yyerror();
	}
}

int is_specie(t_program * program, char * var) {
	if (set_contains(program->species_list, var)) {
		return 1;
	}

	return 0;
}

int is_parameter(t_program * program, char * var) {
	if (set_contains(program->parameters_list, var)) {
		return 1;
	}

	return 0;
}

t_program * program_copy(t_program * program) {
	t_program * copy = (t_program *) malloc(sizeof(t_program));
	*copy = *program;

	return copy;
}

void share_variable(t_program * arr, int size, char * var) {

	int sharee;
	for (int i = 0; i < size; i++) {
		if (map_contains(arr[i].variable_list, var)) {
			if (sharee != -1) {
				sharee = i;
			} else {
				// GENERATE ERROR
				// yyeror();
			}
		}
	}

	for (int i = 0; i < size; i++) {
		if (i == sharee)
			continue;
		map_insert(arr[i].variable_list, var, map_get(arr[i].variable_list, var));
	}
}

int valid_dependence(t_program * program) {
	
	for (int i = 0; i < program->dependence_list_size; i++) {
		if (map_get(program->variable_list, program->dependence[i]) == NOT_FOUND) {
			// GENERATE ERROR
			// yyeror();
		}
	}
}

int main() {
	return 0;
}