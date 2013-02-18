#pragma once

#include "Perlin.h"
#include "Cell.h"
#include "Vec2.h"
#include <stdlib.h>

class WorldGenerator{

	public:
		WorldGenerator(int seed);
		WorldGenerator();
		void genCellAt(int x, int y, Cell * c);
		float getHeightAt(int x, int y);
	private:
		float getHeight(int x, int y);
		int seed;
		Perlin * humid_perlin;
		Perlin * height_perlin;
		Perlin * temp_perlin;
	
	

};
