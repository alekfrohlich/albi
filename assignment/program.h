#include <stdlib.h>
#include <stdio.h>
#include "var_map.h"
#include "var_set.h"

typedef struct s_program {

	char * title;

	map_node * variable_list;
	set_node * species_list;
	set_node * parameters_list;

	char ** dependence;

	int var_list_size;
	int species_list_size;
	int parameter_list_size;

	char ** reactions;

	int dependence_list_size;
} t_program;

/*

*/
void declare_specie(t_program * program, char * var);

/*

*/
void declare_parameter(t_program * program, char * var);

/*

*/
int is_specie(t_program * program, char * var);

/*

*/
int is_parameter(t_program * program, char * var);

/*

*/
t_program * program_copy(t_program * program);

/*

*/
void share_variable(t_program * arr, int size, char * var);

/*

*/
int valid_dependence(t_program * program);
