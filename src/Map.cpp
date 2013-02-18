#include "Map.h"

Map::Map(){
	seed =rand();
	w = new WorldGenerator(seed);
	GridLoc g;
	g.x = 0;
	g.y = 0;
	
	max_active = 200;
	max_drawn = 500;
	
	cell_size = 1.0;
	region_size = 50;
	
	regionShader = new Shader("shaders/region.vs", "shaders/region.fs");
	
	center_region.x = -1;
	center_region.y = -1;
}

void Map::update(){
	map<GridLoc, Region*>::iterator it;
	
	if (active_regions.size() > max_active){
		while (active_regions.size() > max_active){
			active_regions.erase(region_queue.front()->r_pos);
			region_queue.pop();
		}
	}
	
	boost::thread group_1[active_regions.size()];
	boost::thread group_2[active_regions.size()];
	
	int thread_id = 0;
	for (it = active_regions.begin(); it != active_regions.end(); it++){
		Region * r = (*it).second;
		int cb = 0;
		GridLoc g = r->r_pos;
		g.x = fabs(g.x);
		g.y = fabs(g.y);
		if ((g.x%2) == 1){
			if ((g.y%2) == 0){
				cb = 1;
			}
		}
		else if ((g.y%2)==1){
			cb = 1;
		}
		if (cb){
			group_1[thread_id] = boost::thread(boost::bind(&Region::update, r));
			thread_id ++;
		}
	}
	for (int i = 0; i < thread_id; i++){
		group_1[i].join();
	}
	thread_id = 0;
	for (it = active_regions.begin(); it != active_regions.end(); it++){
		Region * r = (*it).second;
		int cb = 0;
		GridLoc g = r->r_pos;
		g.x = fabs(g.x);
		g.y = fabs(g.y);
		if ((g.x%2) == 1){
			if ((g.y%2) == 1){
				cb = 1;
			}
		}
		else if ((g.y%2)==0){
			cb = 1;
		}
		if (cb){
			group_2[thread_id] = boost::thread(boost::bind(&Region::update, r));
			thread_id ++;
		}
	}
	for (int i = 0; i < thread_id; i++){
		group_2[i].join();
	}
	
	
	
}

void Map::draw(){
	map<GridLoc, Region*>::iterator it;
	
	regionShader->bind();
	glEnable(GL_TEXTURE_2D);
	for (it = displayed_regions.begin(); it != displayed_regions.end(); it++){
		Region * r = (*it).second;
		if (r){
			r->render();
			glBindTexture(GL_TEXTURE_2D, r->tex_id);
			
			regionShader->setUniform1i("tex", 0);
			regionShader->setUniform1f("time", GameState::getGameTime());
			regionShader->setUniform2f("pos", r->r_pos.x, r->r_pos.y);
			r->draw();
		}
	}
	
	regionShader->release();
	
	if (displayed_regions.size() > max_drawn){
		while (displayed_regions.size() > max_drawn){
			Region * r = region_draw_queue.front();
			displayed_regions.erase(r->r_pos);
			r->releaseRender();
			region_draw_queue.pop();
		}
	}
}

void Map::loadRegionsAround(vec2 pos, int num_around){
	GridLoc g;
	float s = getRegionSize();
	int x = floor(pos.x/ s);
	int y = floor(pos.y / s);
	
	g.x = (x);
	g.y = (y);
	
	loadRegionsAround(g, num_around);
}

void Map::loadRegionsAround(GridLoc g, int num_around){
	if (g.x == center_region.x && g.y == center_region.y) return;
	
	center_region.x = g.x;
	center_region.y = g.y;

	for (int i = -num_around; i <= num_around; i++){
		for (int j = -num_around; j <=num_around; j++){
			GridLoc n;
			n.x = g.x + i;
			n.y = g.y + j;
			loadRegion(n);
		}
	}
	
	
	
}

void Map::drawRegionsAround(vec2 pos, int num_around){
	GridLoc g;
	float s = getRegionSize();
	int x = floor(pos.x/ s);
	int y = floor(pos.y / s);
	
	g.x = (x);
	g.y = (y);
	
	drawRegionsAround(g, num_around);
}

void Map::drawRegionsAround(GridLoc g, int num_around){
	
	for (int i = -num_around; i <= num_around; i++){
		for (int j = -num_around; j <=num_around; j++){
			GridLoc n;
			n.x = g.x + i;
			n.y = g.y + j;
			if (displayed_regions[n] == NULL){
				Region * r = loadRegion(n);
				displayed_regions[n] = r;
				region_draw_queue.push(r);
			}
		}
	}
}

bool Map::sortFunction(GridLoc rhs, GridLoc lhs){
	GridLoc n_rhs, n_lhs;
	n_rhs.x = fabs(rhs.x - center_region.x);
	n_rhs.y = fabs(rhs.y - center_region.y);
	n_lhs.x = fabs(lhs.x - center_region.x);
	n_lhs.y = fabs(lhs.y - center_region.y);
	if (n_lhs.x > n_rhs.x){
		return true;
	}
	else if (n_lhs.x == n_rhs.x){
		if (n_lhs.y > n_rhs.y){
			return true;
		}
		return false;
	}
	return false;
}

Region * Map::getRegionAt(GridLoc g){
	return regions[g];
}

Region * Map::getRegionAt(vec2 pos){
	GridLoc g;
	float s = getRegionSize();
	
	int x = floor(pos.x/s);
	int y = floor(pos.y/s);
	
	g.x = (x);
	g.y = (y);
	return regions[g];
}

int Map::getSeed(){
	return seed;
}

int Map::numberOfActiveRegions(){
	return (int)active_regions.size();
}

int Map::numberOfTotalRegions(){
	return (int)regions.size();
}

int Map::numberOfDrawnRegions(){
	return (int)displayed_regions.size();
}

Region * Map::loadRegion(GridLoc g){
	GridLoc n;
	n.x = 0;
	n.y = 0;
	Region * a_r = active_regions[g];
	if (a_r == NULL){
		if (regions[g] == NULL){
		
			Region * r = new Region(w,g.x,g.y);
			n.x = g.x;
			n.y = g.y+1;
			r->top_region = regions[n];
			if (r->top_region != NULL){
				r->top_region->bottom_region = r;
			}
			
			n.x = g.x+1;
			n.y = g.y;
			r->right_region = regions[n];
			if (r->right_region != NULL){
				r->right_region->left_region = r;
			}
			
			n.x = g.x;
			n.y = g.y-1;
			r->bottom_region = regions[n];
			if (r->bottom_region != NULL){
				r->bottom_region->top_region = r;
			}
			
			n.x = g.x-1;
			n.y = g.y;
			r->left_region = regions[n];
			if (r->left_region != NULL){
				r->left_region->right_region = r;
			}
			
			r->generate(g.x,g.y, region_size, cell_size);
			regions[g] = r;
			active_regions[g] = r;
			region_queue.push(r);
			return r;
		}
		else {
			Region * r = regions[g];
			active_regions[g] = r;
			region_queue.push(r);
			return r;
		}
	}
	return a_r;
}
