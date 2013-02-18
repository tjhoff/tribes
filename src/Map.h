#pragma once
#include "Region.h"
#include "Shader.h"
#include "GameState.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Map{
	public:
		Map();
		void draw();
		void update();
		void loadRegionsAround(vec2 pos, int num_around);
		void loadRegionsAround(GridLoc pos, int num_around);
		void drawRegionsAround(vec2 pos, int num_around);
		void drawRegionsAround(GridLoc pos, int num_around);
		int numberOfActiveRegions();
		int numberOfTotalRegions();
		int numberOfDrawnRegions();
		int getSeed();
		float getRegionSize(){ return region_size * cell_size;};
		Region * getRegionAt(GridLoc pos);
		Region * getRegionAt(vec2 pos);
	private:
		int max_active;
		int max_drawn;
		int seed;
		bool sortFunction(GridLoc rhs, GridLoc lhs);
		
		GridLoc center_region;
		
		float cell_size;
		int region_size;
		
		Shader * regionShader;
		
		void unloadRegion(GridLoc pos);
		Region * loadRegion(GridLoc pos);
		queue<Region*> region_queue;
		queue<Region*> region_draw_queue;
		map<GridLoc, Region*, gclcmp> regions;
		map<GridLoc, Region*, gclcmp> active_regions;
		map<GridLoc, Region*, gclcmp> displayed_regions;
		
		WorldGenerator * w;
};
