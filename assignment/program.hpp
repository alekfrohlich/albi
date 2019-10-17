#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "reaction.hpp"

namespace models {
	class ProgramModel {

		public:
			/*
				Adiciona uma variavel ainda nao identificada para o conjunto de especies
			*/
			void declare_specie(string var);

			/*
				Adiciona uma variavel ainda nao identificada para o conjunto de parametros
			*/
			void declare_parameter(string var);

			/*
				Retorna verdadeiro se a variavel e uma especie e falso caso contrario
			*/
			bool is_specie(string var);

			/*
				Retorna verdadeiro se a variavel e um parametro e falso caso contrario
			*/
			bool is_parameter(string var);

			/*
				Copie o program para poder ser mudado de acordo com as suas dependencias
			*/
			static models::ProgramModel * program_copy(models::ProgramModel * program);

			/*
				Constroi um program a partir do array de programs passados como parametros.
				Se deve checar a conformidade dos programs dados antes de juntalos.
			*/
			models::ProgramModel * merge_programs(models::ProgramModel * arr, int size);
			
			/*
				Valida se todas as dependencias de um programa foi cumprida
			*/
			bool valid_dependence();


		public:
			string title;
			map<string, float> variable_table;
			set<string> specie_set;
			set<string> parameter_set;

			set<string> dependence;

			vector<ReactionModel> reaction;
	};
}
