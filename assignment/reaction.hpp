#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
namespace models {

	class ReactionModel {

		public:
			
			string stringFormat();

			void insertReactant(string var);
			
			void insertProduct(string var);
		
		public:
			string title;
			vector<string> reactant;
			vector<string> product;
			string rate;
	};

}