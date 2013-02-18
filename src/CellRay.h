#pragma once
#include "Map.h"
#include "Vec2.h"

class CellRay{

	public:
		CellRay(Map * amap);
		float getMovementCost(vec2 start, vec2 end);
		
	private:
		Map * map;

};
