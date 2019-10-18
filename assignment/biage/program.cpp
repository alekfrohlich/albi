#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "program.hpp"

using namespace std;


void models::ProgramModel::declare_specie(string var) {
	if (this->variable_table.find(var) != this->variable_table.end()) {
		this->specie_set.insert(var);
	} else {
		// GENERATE ERROR
		// yyerror();
	}
}

void models::ProgramModel::declare_parameter(string var) {
	if (this->variable_table.find(var) != this->variable_table.end()) {
		this->parameter_set.insert(var);
	} else {
		// GENERATE ERROR
		// yyerror();
	}
}

bool models::ProgramModel::is_specie(string var) {
	return this->specie_set.find(var) == this->specie_set.end();
}

bool models::ProgramModel::is_parameter(string var) {
	return this->parameter_set.find(var) != this->parameter_set.end();
}

static models::ProgramModel * program_copy(models::ProgramModel * program) {
	models::ProgramModel * copy = (models::ProgramModel *) malloc(sizeof(models::ProgramModel));
	*copy = *program;

	return copy;
}

models::ProgramModel * merge_programs(models::ProgramModel * arr, int size) {
	models::ProgramModel * program = new models::ProgramModel;

	set<string> dependence;
	for (int i = 0; i < size; i++) {
		for (auto j = arr[i].dependence.begin(); j != arr[i].dependence.end(); ++j) {
			dependence.insert(*j);
		}
	}

	for (int i = 0; i < size; i++) {
		for (auto p = arr[i].variable_table.begin(); p != arr[i].variable_table.end(); ++p) {
			if (dependence.find(p->first) != dependence.end()) {
				program->variable_table[p->first] = p->second;

				if (arr[i].is_specie(p->first)) {
					program->specie_set.insert(p->first);
				}

				if (arr[i].is_parameter(p->first)) {
					program->parameter_set.insert(p->first);
				}

			} else {
				string cat = arr[i].title;
				cat.push_back('_');
				cat.append(p->first);
				program->variable_table[cat] = p->second;

				if (arr[i].is_specie(p->first)) {
					program->specie_set.insert(cat);
				}

				if (arr[i].is_parameter(p->first)) {
					program->parameter_set.insert(cat);
				}
			}
		}
	}

	return program;
}

bool models::ProgramModel::valid_dependence() {
	
	for (auto i = this->dependence.begin(); i != this->dependence.end(); ++i) {
		if (this->variable_table.find(*i) != this->variable_table.end()) {
			return false;
		}
	}

	return true;
}
