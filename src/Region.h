#pragma once
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include "Cell.h"
#include "Perlin.h"
#include "Vec2.h"
#include "Texture.h"
#include "WorldGenerator.h"
#include <map>
#include <vector>
#include <algorithm>
#include <float.h>
#include <stdio.h>
#include <time.h>

using namespace std;

struct tex_color{
	unsigned char r;
	unsigned char g;
	unsigned char b; 
	unsigned char a;
};

class Region{
	public:
		Region(WorldGenerator * w, int x, int y);
		Region(WorldGenerator * w, GridLoc g);
		~Region();
		
		void generate(int x, int y, int region_size, float c_size);
		// Generate creates all the cells based on the heightfunction.
		// It also gives the cell its region and cell positions.
		GridLoc r_pos;
		void draw();
		void update();
		void render();
		void requestRender() { 
			render_requested = 1;
			build_render_requested = 1;
		};
		void releaseRender();
		
		Cell * getCellAt(int x, int y);
		Cell * getCellAt(vec2 pos);
		Region * top_region;
		Region * bottom_region;
		Region * right_region;
		Region * left_region;
		
		unsigned int tex_id;
	private:
		/*float heightFunction(float x, float y, int &type, int &v_type);
		int tempFunction(float x, float y);
		int aridFunction(float x, float y);*/
		
		void buildRender();
		
		int growing;
		int changed;
		int build_render_requested;
		int render_requested;
		
		GridLoc pos;
		unsigned char * rgbavals;
		Cell ** cells;
		int region_cell_dimensions;
		float cell_size;
		WorldGenerator * worldgen;
	
};
