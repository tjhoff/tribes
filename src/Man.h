#pragma once

class Tribe;

#include "Map.h"
#include <queue>
#include "Vec2.h"
#include "RouteNode.h"
#include "Route.h"
#include "Cell.h"
#include "CellRay.h"
#include <queue>
#include <boost/thread.hpp>
#include <map>

class Man{
	public:
		void init();
		void update();
		void setDestination(vec2 adest);
		int calculating;
		int done_calculating;
		char name[32];
		float speed;
		float size;
		float light;
		int need_path;
		int traveling;
		int pathing_bool;
		Map * map;
		Tribe * tribe;
		std::map<string, float> feewings;
		boost::thread * route_thread;
		RouteNode * destination;
		Route * route;
		vec2 pos;
		vec2 formation_pos;
		vec2 prev_dest;
		vec2 dest_coords;
};
