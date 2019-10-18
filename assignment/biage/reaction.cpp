#include "reaction.hpp"

using namespace std;

string models::ReactionModel::stringFormat() {
	string format = "->";

	for (int i = 0; i < this->reactant.size(); i++) {
		format.insert(0, this->reactant[i]);
	}

	for (int i = 0; i < this->product.size(); i++) {
		format.append(this->product[i]);
	}

	format.push_back(';');
	format.append(this->rate);

	return format;
}

void models::ReactionModel::insertReactant(string var) {
	this->reactant.push_back(var);
}

void models::ReactionModel::insertProduct(string var) {
	this->product.push_back(var);
}
