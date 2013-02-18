#pragma once
#include "Map.h"

class RouteNode{
	public:
		RouteNode(RouteNode * next, RouteNode * parent, vec2 pos);
		RouteNode * next();
		RouteNode * previous();
		void setParent(RouteNode * parent);
		void setNext(RouteNode * next);
		vec2 getPos();
		void setPos(vec2 npos) { pos = npos;};
		
	private:
		RouteNode * next_node;
		RouteNode * parent_node;
		vec2 pos;
};

class AStarNode{
	public:
		AStarNode(Map * m, vec2 pos);
		AStarNode(Map * m, Region * r, Cell * c, AStarNode * p);
		void addNode(AStarNode * p);
		float getCost();
		float calcH(vec2 end);
		void setF(float f);
		void setG(float g);
		float getF();
		float getG();
		AStarNode * getNext();
		vec2 getPos(){ return cell_pos;};
		Cell * cell;
		int open;
		int closed;
		int route;
		AStarNode * parent;	
		AStarNode * nodes[10];
		int num_nodes;
	private:
		Map * map;
		Region * region;
		
		vec2 cell_pos;
		float heuristic_val;
		float f_val;
		float g_val;
		
};
