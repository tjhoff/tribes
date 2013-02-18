#include "Cell.h"
int getVegetationForBiome(int biome){
	int v_type = V_NONE;
	switch (biome){
		case B_NONE:
			v_type = V_NONE;
			break;
		case DESERTSAND:
			v_type = DESERT_SCRUBLAND;
			break;
		case SNOW:
			v_type = TUNDRA_SCRUBLAND;
			break;
		case ROCK:
			v_type = FOREST;
			break;
		case DIRT:
			v_type = GRASSLAND;
			break;
		case CLAY:
			v_type = JUNGLE;
			break;
		default:
			v_type = V_NONE;
			break;
	}
	return v_type;
}
