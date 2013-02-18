#pragma once

class Man;

#include <vector>

class Tribe{
	public:
		void addPerson(Man * person);
		std::vector<Man *> people;
};
