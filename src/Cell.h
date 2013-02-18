#pragma once
#include <math.h>
#include "Vec3.h"

struct GridLoc{
	int x;
	int y;
};

enum BIOME{
	B_NONE,
	SALTWATER,
	FRESHWATER,
	ICE,
	DESERTSAND,
	BEACHSAND,
	SNOW,
	ROCK,
	DESERTROCK,
	DIRT,
	CLAY,
};

enum VEGETATION{
	V_NONE,
	JUNGLE,
	GRASSLAND,
	DESERT_SCRUBLAND,
	TUNDRA_SCRUBLAND,
	FOREST
};

struct gclcmp{
	bool operator ()(const GridLoc& lhs, const GridLoc& rhs) const{
		if (lhs.x > rhs.x){
			return true;
		}
		else if (lhs.x == rhs.x){
			if (lhs.y > rhs.y){
				return true;
			}
			return false;
		}
		return false;
	}
};

struct Cell{

	GridLoc gpos;
	GridLoc region_pos;
	float light;
	int biome;
	int v_type;
	float movement_mod;
	float n_heights[4];
	vec3 normal;
	Cell *neighbors[4];
	int num_neighbors;
	float height;
	float depth;
	bool water;
	bool walked;
	bool impassable;
	float fertility;
	float temp;
	float humidity;
};

int getVegetationForBiome(int biome);
