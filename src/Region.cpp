#include "Region.h"

Region::Region(WorldGenerator * w, GridLoc g){
}

Region::Region(WorldGenerator * w, int x, int y){
	growing = 1;
	changed = 1;
	r_pos.x = x;
	r_pos.y = y;
	worldgen = w;//new Perlin(2,5,3,seed);
	top_region = bottom_region = left_region = right_region = NULL;
	//glGenTextures(1, &tex_id);
	tex_id = 0;
	render_requested = 1;
	build_render_requested = 1;
}

Region::~Region(){
	for (int i = 0; i < region_cell_dimensions; i++){
		delete cells[i];
	}
	delete cells;
}

Cell * Region::getCellAt(vec2 cpos){
	float s = region_cell_dimensions * cell_size;
	int x = floor(cpos.x - r_pos.x*s);
	int y = floor(cpos.y - r_pos.y*s);


	if (x > region_cell_dimensions-1){
		x = region_cell_dimensions-1;
	}
	if (x < 0){
		x = 0;
	}
	if (y > region_cell_dimensions-1){
		y = region_cell_dimensions-1;
	}
	if( y < 0){
		y = 0;
	}
	return &cells[y][x];
}

Cell * Region::getCellAt(int x, int y) {
	if (x > region_cell_dimensions-1 || x < 0){
		printf("Trying to access a cell out of bounds(%i,%i)\n", x, y);
		exit(0);
	}
	if (y > region_cell_dimensions-1 || y < 0){
		printf("Trying to access a cell out of bounds(%i,%i)\n", x, y);
		exit(0);
	}
	return &cells[y][x];
};

void Region::generate(int xpos, int ypos, int region_size, float c_size){
	
	region_cell_dimensions = region_size;
	cell_size = c_size;
	int tex_size = 4*pow(region_cell_dimensions,2);
	rgbavals = new unsigned char[tex_size];
	
	pos.x = xpos*region_cell_dimensions*((cell_size));
	pos.y = ypos*region_cell_dimensions*((cell_size));
	
	
	
	cells = new Cell*[region_cell_dimensions];
	for (int y = 0; y < region_cell_dimensions; y++){
		cells[y] = new Cell[region_cell_dimensions];
	}
	
	Cell * highest_cell = NULL;
	float highest = FLT_MIN;
	vec2 center(region_cell_dimensions/2.0, region_cell_dimensions/2.0);
	
	int xoffset = xpos*region_cell_dimensions;
	int yoffset = ypos*region_cell_dimensions;
	
	for (int y = 0; y< region_cell_dimensions; y++){
		for (int x = 0; x < region_cell_dimensions; x++){
			Cell * c = &cells[y][x];
			
			c->gpos.x=x;
			c->gpos.y=y;
			c->region_pos.x = xpos;
			c->region_pos.y = ypos;
			c->num_neighbors = 0;
			c->depth = 0;
			int count_neighbors = 0;
			if (x+1 < region_cell_dimensions){
				c->neighbors[count_neighbors] = &cells[y][x+1];
				count_neighbors ++;
			}
			else{
				if (right_region) {
					Cell * r_c = right_region->getCellAt(0,y);
					c->neighbors[count_neighbors] = r_c;
					count_neighbors++;
					r_c->neighbors[r_c->num_neighbors] = c;
					r_c->num_neighbors++;
				}
			}
			if (x-1 >= 0){
				c->neighbors[count_neighbors] = &cells[y][x-1];
				count_neighbors ++;
			}
			else{
				if (left_region) {
					Cell * r_c = left_region->getCellAt(region_cell_dimensions-1,y);
					c->neighbors[count_neighbors] = r_c;
					count_neighbors++;
					r_c->neighbors[r_c->num_neighbors] = c;
					r_c->num_neighbors++;
				}
			}
			if (y+1 < region_cell_dimensions){
				c->neighbors[count_neighbors] = &cells[y+1][x];
				count_neighbors ++;
			}
			else{
				if (top_region) {
					Cell * r_c = top_region->getCellAt(x,0);
					c->neighbors[count_neighbors] = r_c;
					count_neighbors++;
					r_c->neighbors[r_c->num_neighbors] = c;
					r_c->num_neighbors++;
				}
			}
			if (y-1 >= 0){
				c->neighbors[count_neighbors] = &cells[y-1][x];
				count_neighbors ++;
			}
			else{
				if (bottom_region) {
					Cell * r_c = bottom_region->getCellAt(x,region_cell_dimensions-1);
					c->neighbors[count_neighbors] = r_c;
					count_neighbors++;
					r_c->neighbors[r_c->num_neighbors] = c;
					r_c->num_neighbors++;
				}
			}
			c->num_neighbors = count_neighbors;
			
			c->water = 0;
			
			worldgen->genCellAt(x+xoffset, y+yoffset, c);
			
		
		}
	}
	
	for (int y = 0; y< region_cell_dimensions; y++){
		for (int x = 0; x < region_cell_dimensions; x++){
		
			Cell * c = &cells[y][x];
		
			c->n_heights[0] = c->height;
			int r, u;
			r = u = 0;
			if (x+1 <= region_cell_dimensions-1){
				r = 1;
				c->n_heights[1] = cells[y][x+1].height;
			}
			if (y+1 <= region_cell_dimensions-1){
				u = 1;
				c->n_heights[3] = cells[y+1][x].height;
			}
			
			if (u && r){
				c->n_heights[2] = cells[y+1][x+1].height;
			}
			
			
			
			int junk_type;
			int junk_v_type;
			if (!r){
				Cell temp_cell;
				Cell * tmp_ptr = &temp_cell;
				worldgen->genCellAt(x+1+xoffset, y+yoffset, tmp_ptr);
				c->n_heights[1] = tmp_ptr->height;
			}
			if (!u){
				Cell temp_cell;
				Cell * tmp_ptr = &temp_cell;
				worldgen->genCellAt(x+xoffset, y+1+yoffset, tmp_ptr);
				
				c->n_heights[3] = tmp_ptr->height;
			}
			if (!u||!r){
				Cell temp_cell;
				Cell * tmp_ptr = &temp_cell;
				worldgen->genCellAt(x+1+xoffset, y+1+yoffset, tmp_ptr);
				c->n_heights[2] = tmp_ptr->height;
			}
			
			vec3 a = vec3(x, c->n_heights[0], y);
			vec3 v_vec = vec3(x+cell_size, c->n_heights[1], y) - a;
			vec3 u_vec = vec3(x, c->n_heights[3], y+cell_size) - a;
			vec3 n;
			n.x = (u_vec.y*v_vec.z - u_vec.z*v_vec.y);
			n.y = (u_vec.z*v_vec.x - u_vec.x*v_vec.z);
			n.z = (u_vec.x*v_vec.y - u_vec.y*v_vec.x);
			c->normal = n.normalize();
			float m_mod = dot(vec3(0,1,0), c->normal);
			if (m_mod < .4) m_mod = .4;
			c->movement_mod *= m_mod ;
			
		}
	}
	
	/*for (int y = 0; y< region_cell_dimensions; y++){
		for (int x = 0; x < region_cell_dimensions; x++){
			Cell * c = &cells[y][x];
			if (c->height < 0.0) continue;
			if (c->water){
				for (int i = 0; i < c->num_neighbors; i ++){
					if (c->neighbors[i]->water) continue;
					c->neighbors[i]->vegetation = 100.0;
					c->neighbors[i]->v_type = getVegetationForBiome(c->neighbors[i]->biome);
				}
			}
		}
	}*/
	
	
	buildRender();
	
}

void Region::draw(){
	glBindTexture(GL_TEXTURE_2D, tex_id);
	float side = region_cell_dimensions * cell_size;
	glPushMatrix();
	glTranslatef(pos.x, 0, pos.y);
	glBegin(GL_TRIANGLES);
		
		glTexCoord2f(0,0);
		glVertex3f(0,0,0);
		glTexCoord2f(1,0);
		glVertex3f(side,0,0);
		glTexCoord2f(1,1);
		glVertex3f(side,0,side);
		
		glTexCoord2f(0,0);
		glVertex3f(0,0,0);
		glTexCoord2f(0,1);
		glVertex3f(0,0,side);
		glTexCoord2f(1,1);
		glVertex3f(side,0,side);
		
	glEnd();
	glPopMatrix();
	
}

void Region::buildRender(){
	int pixel_pos = 0;
	
	for (int y = 0; y < region_cell_dimensions; y++){
		for (int x = 0; x < region_cell_dimensions; x++){
			Cell * c = &cells[y][x];
			
			float factor = dot(vec3(1,1,1).normalize(), c->normal)*.8 +.2;
			if (factor>1.0) factor = 1.0;
			if (factor < 0.0) factor = 0.0;

			if (c->water){
				factor = 1.5-(c->depth/15.0);
				if (factor>1.0) factor = 1.0;
				if (factor < 0.2) factor = 0.2;
				c->light = 1.0;
			}
			
			int f_rem = factor*5.0;
			factor = f_rem /5.0;
			
			
			if (!c->water) c->light = factor;
			
			if (c->walked) {
				factor -=.1;
				if (factor < 0.0) factor = 0;
			}
			tex_color pixel;
			
			pixel.a = c->water * 255;
					
			if (c->v_type != V_NONE){
				factor = (factor *.8) + .2;
				switch (c->v_type){
					case JUNGLE:
						pixel.r = 255*.0*factor;
						pixel.g = 255*.3*factor;
						pixel.b = 255*.05*factor;
						break;
					case GRASSLAND:
						pixel.r = 255*.2*factor;
						pixel.g = 255*.5*factor;
						pixel.b = 255*.1*factor;
						break;
					case DESERT_SCRUBLAND:
						pixel.r = 255*.3*factor;
						pixel.g = 255*.4*factor;
						pixel.b = 255*.1*factor;
						break;
					case TUNDRA_SCRUBLAND:
						pixel.r = 255*.2*factor;
						pixel.g = 255*.4*factor;
						pixel.b = 255*.3*factor;
						break;
					case FOREST:
						pixel.r = 255*.0*factor;
						pixel.g = 255*.2*factor;
						pixel.b = 255*.0*factor;
						break;
					default:
						pixel.r = 255*.7*factor;
						pixel.g = 255*.3*factor;
						pixel.b = 255*.7*factor;
						break;
				}
				
			}
			
			else switch (c->biome){
				case B_NONE:
					pixel.r = 255*.8*factor;
					pixel.g = 255*.2*factor;
					pixel.b = 255*.8*factor;
					break;
				case SALTWATER:
					pixel.r = 255*.1*factor;
					pixel.g = 255*.1*factor;
					pixel.b = 255*.2*factor;
					break;
				case FRESHWATER:
					pixel.r = 255*.05*factor*3;
					pixel.g = 255*.05*factor*5;
					pixel.b = 255*.4*factor;
					break;
				case DESERTSAND:
					pixel.r = 255*.9*factor;
					pixel.g = 255*.8*factor;
					pixel.b = 255*.6*factor;
					break;
				case BEACHSAND:
					pixel.r = 255*.85*factor;
					pixel.g = 255*.82*factor;
					pixel.b = 255*.62*factor;
					break;
				case SNOW:
					pixel.r = 255*.8*factor;
					pixel.g = 255*.8*factor;
					pixel.b = 255*.8*factor;
					break;
				case ROCK:
					pixel.r = 255*.47*factor;
					pixel.g = 255*.4*factor;
					pixel.b = 255*.4*factor;
					break;
				case DIRT:
					pixel.r = 255*.4*factor;
					pixel.g = 255*.3*factor;
					pixel.b = 255*.15*factor;
					break;
				case CLAY:
					pixel.r = 255*.3*factor;
					pixel.g = 255*.1*factor;
					pixel.b = 255*.05*factor;
					break;
				default:
					break;
			
			}
			
			rgbavals[pixel_pos] = pixel.r;
			rgbavals[pixel_pos+1] = pixel.g;
			rgbavals[pixel_pos+2] = pixel.b;
			rgbavals[pixel_pos+3] = pixel.a;
			pixel_pos +=4;
			
		}
	}
}

void Region::render(){

	if (render_requested) {
		if (tex_id == 0){
			tex_id = Texture::loadFromMemory(region_cell_dimensions, region_cell_dimensions, rgbavals);
			render_requested = 0;
		}
		else{
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, region_cell_dimensions, region_cell_dimensions, GL_RGBA, GL_UNSIGNED_BYTE, rgbavals);
			glBindTexture(GL_TEXTURE_2D, 0);
			render_requested = 0;
		}
	}
	
}

void Region::releaseRender(){
	glDeleteTextures(1, &tex_id);
	tex_id = 0;
	render_requested = 1;
}

void Region::update(){

	if (build_render_requested){
		buildRender();
		build_render_requested = 0;
	}
}

