#include "Tribe.h"
#include "Man.h"

void Tribe::addPerson(Man * person){
	people.push_back(person);
	person->tribe = this;

}
