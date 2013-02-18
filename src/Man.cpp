#include "Man.h"
#include "Tribe.h"

void Man::setDestination(vec2 adest){
	dest_coords = adest;
	need_path = 1;
}

void Man::update(){
	
	GridLoc g;
	
	float s = map->getRegionSize();
	
	//map->loadRegionsAround(pos,2);
	//map->drawRegionsAround(pos,2);
	
	if (!traveling && !need_path && 0){
		//CellRay c(map);
		
		Region * r = map->getRegionAt(pos);
		if (!r){
			map->loadRegionsAround(pos,1);
			r = map->getRegionAt(pos);
		}
		
		Cell * cell = r->getCellAt(pos);
		
		float highest = FLT_MIN;//cell->height;
		vec2 highest_pos = pos;
		int found = 0;
		
		for (int i = 0; i < 16; i++){
			float angle = ((2*PI)/16.0) * i;
			
			double ds, dc;
			
			sincos(angle, &ds, &dc);
			
			vec2 newpos = pos + vec2(dc, ds)*50;
			
			r = map->getRegionAt(newpos);
			if (!r){
				map->loadRegionsAround(newpos,1);
				r = map->getRegionAt(newpos);
			}
			
			cell = r->getCellAt(newpos);
			
			
			
			float height = cell->height;//c.getMovementCost(pos, newpos);
			
			if (height > highest){
				highest = height;
				highest_pos = newpos;
				found = 1;
			}
			
		}
		
		if (!found){
			
		}
		
		prev_dest = dest_coords;
		dest_coords = highest_pos;
		need_path = 1;
		
	}
	
	if (need_path){	
		route = calcRoughRoute(map, pos, dest_coords, 2);
		if (route){
			destination = route->getStart();
			traveling = 1;
			need_path = 0;
		}
		else need_path =0;
	}
	
	if (traveling){
		if (destination == NULL){
			traveling = 0;
			need_path = 0;
			return;
		}
		
		vec2 current_dest;
		vec2 dest_diff;
		
		
		current_dest = destination->getPos();
		// head to the center of the current cell
		dest_diff = (current_dest - pos);
		
		Region * r = map->getRegionAt(pos);
	
		Cell * c = r->getCellAt(pos);
		
		float mod_speed = speed * c->movement_mod;
		
		if (!c->water && c->biome != ROCK && !c->walked){
			c->walked = 1;
			c->v_type = V_NONE;
			r->requestRender();
		}
		light = c->light;
		if (dest_diff.length() <= mod_speed){
			pos = current_dest;
			RouteNode * new_dest = destination->next();
			if (new_dest == NULL){
				traveling = 0;
				if (!pathing_bool) delete destination;
				else delete route;
				return;
			}
			
			else {
				destination = new_dest;
			}
		}
		
		else {
		
			pos = pos + dest_diff.normalize() * mod_speed;
		
		}
		
	}	
}

void Man::init(){
	double f_s, f_c;
	
	speed = .5 + (rand()%11)*.1;
	size = .5;
	prev_dest = vec2(-10000,-1000);
	formation_pos = randSpread(vec2(0,0), 5.0);
	pos = vec2(0,0) + formation_pos;
	dest_coords = formation_pos*-1;
	light = 1.0;
	need_path = 1;
	pathing_bool = 1;
	
	int vowel_streak =0;
	int cons_streak = 0;
	int apostrophe = 0;
	int name_size = 3 + rand()%8;
	char * phoenemes[] = {"fre", "ah", "tel","zul", "ji","nu", "wey","pul","kre","ute", "hes", "tet", "ka", "bo", "wol", "an", "wo", "em","ya","bi", "lo"};
	/*char vowels[] = {'a','e','i','o','u','y'};
	char consonants[] = {'b','c','d','f','h','j','k','l','m','n','p','q','r','s','t','v','x','y','z'};
	for (int i = 0; i < name_size; i++){
	
		if (!apostrophe && ((rand()%10) / 1.0)==0 && i < name_size -1 && i > 0) {
			
			name[i] = '\'';
			apostrophe = 1;
		}
		else if ((rand()%2 || cons_streak >= 2)){
			name[i] = vowels[rand()%6];
			vowel_streak ++;
			cons_streak = 0;
		}
		else{
			name[i] = consonants[rand()%6];
			cons_streak ++;
			vowel_streak = 0;
		}
	}*/
	int last = -1;
	int phoeneme_count = 21;
	int num_phoenemes_in_name = rand()%2 + 1;
	int modifier = 0;
	strcpy(name, phoenemes[rand()%phoeneme_count]);
	for (int i = 0; i < num_phoenemes_in_name; i++){
		if (!modifier){
			if ((rand()%8) == 0) {
				strcat(name, "'");
				modifier = 1;
			}	
			else if ((rand()%10) == 0){
				strcat(name, "-");
				modifier = 1;
			}
		}
		int index = rand()%phoeneme_count;
		while (index == last){
			index = rand()%phoeneme_count;
		}
		strcat(name, phoenemes[index]);
		last = index;
	}
	
	/*strcat(name, " ");
	int char_at_last = strlen(name);
	strcat(name, phoenemes[rand()%18]);
	if ((rand()%4) == 0) strcat(name, "'");
	strcat(name, phoenemes[rand()%phoeneme_count]);*/
	
	name[0] = name[0] - 32;
	//name[char_at_last] = name[char_at_last] - 32;
}
