#include "RouteNode.h"

AStarNode::AStarNode(Map * m, vec2 pos){

	map = m;
	region = m->getRegionAt(pos);
	if (region == NULL){
		printf("Region is null in AStarNode\n");
		exit(0);
	}
	cell = region->getCellAt(pos);
	num_nodes = 0;
	open = 1;
	closed = 0;
	parent = NULL;
	f_val = 0;
	g_val = 0;
	route = 0;
	
	for (int i = 0; i < 10; i++){
		nodes[i] = NULL;
	}
	
	float s = m->getRegionSize();
	cell_pos = vec2(cell->region_pos.x*s + cell->gpos.x, cell->region_pos.y*s + cell->gpos.y);

}

AStarNode::AStarNode(Map * m, Region * r, Cell * c, AStarNode * p){
	map = m;
	region = r;
	if (region == NULL){
		printf("Region is null in AStarNode\n");
		exit(0);
	}
	cell = c;
	parent = p;
	num_nodes = 0;
	f_val = 0;
	g_val = 0;
	route = 0;
	
	open = 0;
	closed = 0;
	
	for (int i = 0; i < 10; i++){
		nodes[i] = NULL;
	}
	
	float s = m->getRegionSize();
	cell_pos = vec2(c->region_pos.x*s + c->gpos.x, c->region_pos.y*s + c->gpos.y);
}

AStarNode * AStarNode::getNext(){

	AStarNode * next = NULL;
	for (int i = 0; i < num_nodes; i++){
		if (nodes[i]->route){
			next = nodes[i];
			break;
		}
	}
	return next;
}

void AStarNode::addNode(AStarNode * n){
	nodes[num_nodes] = n;
	num_nodes ++;
}

float AStarNode::getCost(){
	if (cell->water) return 10;
	return (1-cell->movement_mod);
}

void AStarNode::setF(float f){
	f_val = f;
}

float AStarNode::getF(){
	return f_val;
}

void  AStarNode::setG(float g){
	g_val = g;
}

float AStarNode::getG(){
	return g_val;
}

float AStarNode::calcH(vec2 end){
	return (cell_pos - end).length();//(fabs(cell_pos.x - end.x) + fabs(cell_pos.y - end.y));
}

RouteNode::RouteNode(RouteNode * next, RouteNode * parent, vec2 p){
	next_node = next;
	parent_node = parent;
	pos = p;
}

RouteNode * RouteNode::next(){
	return next_node;
}

RouteNode * RouteNode::previous(){
	return next_node;
}

void RouteNode::setParent(RouteNode * parent){
	parent_node = parent;
}

void RouteNode::setNext(RouteNode * next){
	next_node = next;
}

vec2 RouteNode::getPos(){
	return pos;
}
