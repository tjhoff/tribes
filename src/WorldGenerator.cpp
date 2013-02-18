#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(){
	seed = rand();

	humid_perlin = new Perlin(2,5,3,seed);
	height_perlin = new Perlin(2,5,3,seed+1);
	temp_perlin = new Perlin(2,5,3,seed+2);

}

WorldGenerator::WorldGenerator(int a_seed){
	seed = a_seed;

	humid_perlin = new Perlin(2,5,3,seed);
	height_perlin = new Perlin(2,5,3,seed+1);
	temp_perlin = new Perlin(2,5,3,seed+2);

}

float WorldGenerator::getHeightAt(int x, int y){
	return getHeight(x, y) * 100;
}

float WorldGenerator::getHeight(int x, int y){
	float val = height_perlin->Get(x/40000.0,y/40000.0)*2.8;
	val += height_perlin->Get((x+50000)/500.0,y/500.0)*.8;
	val  += height_perlin->Get((x+500)/1000.0,(y-5000)/3000.0)*1.2;
	val  += height_perlin->Get((x-500)/50.0,(y+5000)/50.0)*.05;
	
	
	return val;
	
}

void WorldGenerator::genCellAt(int x, int y, Cell * c){
	
	
	double he_val, hu_val, te_val;
	
	int biome;
	int veg_type;
	
	bool imp_bool = 0;
	
	biome = B_NONE;
	veg_type = TUNDRA_SCRUBLAND;
	
	/*he_val = getHeight(x,y);
	hu_val = humid_perlin->Get(x/20000.0,y/20000.0)*.9 + humid_perlin->Get((-x+5000)/500.0,(y+2000)/400.0)*.1 + humid_perlin->Get(x/200.0,(y+500)/200.0)*.1;
	te_val = temp_perlin->Get(x/20000.0,y/20000.0)*.9 + temp_perlin->Get((-x+5000)/400.0,(y+2000)/500.0)*.1 + temp_perlin->Get(x/200.0,(y+500)/200.0)*.1;
	c->water = 0;
	c->movement_mod = 1.0;
	
	float val = he_val;
	
	if (he_val < 0) {
		biome = SALTWATER;
		c->water = 1;
		c->movement_mod = .2;
		c->depth = fabs(he_val)*20;
	}
	else if (he_val < .2) {
		if (te_val < -.3) {
			val = he_val +  height_perlin->Get((x+200)/50.0,(y-5000)/200.0)*.1 * (te_val +.3) * (he_val -.2);
			biome = ROCK;
		}
		else if (hu_val >.3) biome = DIRT;
		else biome = BEACHSAND;
	}
	else {
	
		if (he_val > .6){
			// mountains
			
			val =(pow(he_val,1.3) + height_perlin->Get((x+200)/200.0,(y-5000)/50.0)*.01) * (he_val -.6);
			if (te_val < .3 && hu_val > -.3) biome = SNOW;
			else biome = ROCK;
		}
		else{
			if (te_val > .5 || hu_val < -.7) biome = DESERTSAND;
			else if (hu_val > .7) biome = CLAY;
			else if (te_val < -.3 && hu_val > -.3) biome = SNOW;
			else biome = DIRT;
		}
	
	}
	veg_type = V_NONE;
	if (hu_val > -.4 && te_val <.8&& te_val > -.5) veg_type = getVegetationForBiome(biome);
	
	c->biome = biome;
	c->v_type = veg_type;
	c->height = val * 100;
	
	c->fertility = 1.0;
	c->walked = 0;*/
	
	double rval = height_perlin->Get((x+5000)/20000.0,(y-5000)/20000.0)*.9 + height_perlin->Get((x-2000)/200.0,y/200.0)*.1;
	double tval = (temp_perlin->Get((x-5000)/100000.0,(y-5000)/100000.0)*.9 + temp_perlin->Get((x-8000)/400.0,y/400.0)*.1);
	double wval1 = humid_perlin->Get((x+5000)/5000.0,(y-2500)/5000.0);
	double wval2 = humid_perlin->Get((x-200)/200.0,y/200.0);
	double wval3 = humid_perlin->Get((x-2000 + y)/500.0,(y+3000-x)/500.0);
	double wval = wval1 * .85 + wval2 * .1 + wval3 * .05;
	
	
	if (rval < -.4){
		if (wval < 0.6) biome = DESERTSAND;
		else biome = DIRT;
	}
	
	else if (rval < -.1){
		if (wval < .5) biome = DIRT;
		else biome = CLAY;
	}
	
	else if (rval < .2){
		if (tval > -.2 && wval < .5) biome = DIRT;
		else if (wval > .5) biome = CLAY;
		else biome = SNOW;
	}

	else if (rval < .45){
		if (tval > -.2) biome = ROCK;
		else biome = SNOW;
	}
	
	else {
		if (tval > .3 || wval < 0.8) biome = ROCK;
		else biome = SNOW;
	}
	
	double val = 0;
	double val1 = height_perlin->Get((x)/10000.0,(y)/3000.0)*100;
	double val2 = height_perlin->Get((x+1000)/500.0,(y)/500.0)*20 + height_perlin->Get(x/1000.0,(y+1000)/1000.0)*50;
	val += val1 + val2 + rval*100.0;
	
	
	if (biome == ROCK || biome == SNOW){
		float mod = (rval -.1) * 4;
		if (mod > 1.0) mod = 1.0;
		if (mod < 0.0) mod = 0.0;
		val += height_perlin->Get((x)/100.0,(y)/100.0)*5 * mod;
	}
	
	if (biome == DESERTSAND){
		val = val1+ val2;
	}
	
	if (biome == CLAY){
		float mod = (rval+.4) * 3.3;
		if (mod > 1.0) mod = 1.0;
		if (mod < 0.0) mod = 0.0;
		val = val1 + val2 + (rval*100.0 ) * mod;
	}
	
	if (biome == DIRT){
		float mod = (rval+.4) * 3.3;
		if (mod > 1.0) mod = 1.0;
		if (mod < 0.0) mod = 0.0;
		val = val1 + val2 + (rval*100.0 ) * mod;
	}
	
	
	float nwval = (wval1*.9 + wval3 * .35 + wval2*.05);
	if (nwval > .9){
		biome = FRESHWATER;
		veg_type = V_NONE;
		c->depth = (nwval-.9)*20.0;
	}
	else if (wval > -.5 && rval < .9 && tval > -.5) veg_type = getVegetationForBiome(biome);
	else veg_type = V_NONE;
	
	c->biome = biome;
	c->v_type = veg_type;
	if (biome == SALTWATER || biome == FRESHWATER){
		c->water = 1;
	}
	
	float fertility;
	float movement_mod;
	switch (biome){
	
		case B_NONE:
			fertility = 0;
			movement_mod = 0.0;
			break;
		case SALTWATER:
			fertility = 0;
			movement_mod = 0.05;
			break;
		case FRESHWATER:
			fertility = 0;
			movement_mod = 0.1;
			break;
		case DESERTSAND:
			fertility = .2;
			movement_mod = .8;
			break;
		case SNOW:
			fertility = .2;
			movement_mod = .2;
			break;
		case ROCK:
			fertility = .5;
			movement_mod = .4;
			break;
		case DIRT:
			fertility = .8;
			movement_mod = .8;
			break;
		case CLAY:
			fertility = 1.0;
			movement_mod = .6;
			break;
		default:
			fertility = .0;
			movement_mod = .1;
			break;		
	
	}	
	
	c->humidity = (wval + 1.0)*.5;
	c->temp = (tval  + .5) *100;
	c->height = val;
	c->fertility = fertility;
	c->movement_mod = movement_mod;
	c->walked = 0;
	
}
