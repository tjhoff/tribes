#pragma once
#include "RouteNode.h"
#include "CellRay.h"
#include <queue>
#include <map>

class Route{
	public:
		Route(RouteNode * start_node, RouteNode* end_node);
		~Route();
		RouteNode * getStart();
		RouteNode * getEnd();
		void smooth();
	private:
		RouteNode * start;
		RouteNode * end;
};

class nodecomparison{
	public:
		bool operator()(AStarNode * lhs, AStarNode * rhs) const{
			return (lhs->getF() > rhs->getF());
		}
};

Route * calcRoute(Map * m, vec2 start, vec2 end);
Route * calcRoughRoute(Map * m, vec2 start, vec2 end, float stepsize);
