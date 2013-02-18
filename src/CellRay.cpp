#include "CellRay.h"

CellRay::CellRay(Map * amap){
	map = amap;
}

float CellRay::getMovementCost(vec2 start, vec2 end){
	Region * r;
	Cell * c;
	float cost=0;
	
	bool steep = abs(end.y - start.y) > abs(end.x - start.x);
	float x0 = start.x;
	float y0 = start.y;
	float x1 = end.x;
	float y1 = end.y;
	if (steep){
		x0 = start.y;
		x1 = end.y;
		y0 = start.x;
		y1 = end.x;
	}
	if (x0 > x1){
		float tmp = x0;
		x0 = x1;
		x1 = tmp;
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	
	int dx = x1-x0;
	int dy = abs(y1-y0);
	int error = dx /2;
	int ystep;
	int y = y0;
	if (y0<y1) ystep = 1;
	else ystep = -1;
	
	for (int i = x0; i < x1; i++){
		int x = i;
		vec2 coords;
		if (steep){
			coords.x = y;
			coords.y = x;
		}
		else {
			coords.x = x;
			coords.y = y;
		}
		
		r = map->getRegionAt(coords);
		if (!r){
			map->loadRegionsAround(coords,1);
			r = map->getRegionAt(coords);
		}
		
		c = r->getCellAt(coords);
		if (c->impassable){
			return -1;
		}
		else cost += 10*(1-c->movement_mod);
		
		
		error = error - dy;
		if (error < 0){
			y = y + ystep;
			error = error + dx;
		}
	}
	return cost;
}
