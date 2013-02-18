#include "Route.h"

Route::Route(RouteNode* start_node, RouteNode* end_node){
	start = start_node;
	end = end_node;
}

Route::~Route(){

	RouteNode * node = start;
	
	while (node!=NULL){
		RouteNode * n2 = node->next();
		delete node;
		node = n2;
	}

}

void Route::smooth(){

	RouteNode * node = start;

	while (node != NULL){
		RouteNode * n2 = node->next();
		
		if (n2 == NULL){
			break;
		}
		
		if (n2->getPos().x == node->getPos().x || n2->getPos().y == node->getPos().y){
			RouteNode * n3 = n2->next();
			
			node->setNext(n3);
			
			if (n3 != NULL){
				n3->setParent(node);
				node == n3;
			}
			else break;
			
			delete n2;
			
		}
		
		else node = n2;
		
	}

}

RouteNode * Route::getStart(){
	return start;
}

RouteNode * Route::getEnd(){
	return end;
}

Route * calcRoute(Map * m, vec2 start, vec2 end){

	priority_queue<AStarNode *, vector<AStarNode*>, nodecomparison> pq;
	map<GridLoc, AStarNode *, gclcmp> open_map;
	
	Region * r = m->getRegionAt(start);
	Cell * c = r->getCellAt(start);
	
	float s = m->getRegionSize();
	
	AStarNode * root = new AStarNode(m, start);
	root->setG(0);
	root->setF(root->calcH(end));
	
	GridLoc g;
	g.x = c->gpos.x + c->region_pos.x * s;
	g.y = c->gpos.y + c->region_pos.y * s;
	
	open_map[g] = root;
	
	pq.push(root);
	
	
	int searching = 1;
	
	AStarNode * goalNode = NULL;
	
	
	
	while (searching){
		AStarNode * best_node;
		if (!pq.empty()){
			best_node = pq.top();
		}
		else {
			printf("No path found!\n");
			
			return NULL;
		}
		
		vec2 best_pos = best_node->getPos();
		
		if (fabs(best_pos.x-end.x) + fabs(best_pos.y - end.y) < 1.0){
			goalNode = best_node;
			searching = 0;
			break;
		}
		
		pq.pop();
		
		best_node->closed = 1;
		
		for (int j = 0; j < best_node->cell->num_neighbors; j++){
			Cell * n = best_node->cell->neighbors[j];

			if (best_node->parent != NULL){
				AStarNode * parent = best_node->parent;
				if (n->gpos.x == parent->cell->gpos.x &&
					 n->gpos.y == parent->cell->gpos.y){
				continue;
				}
			}
			
			GridLoc g;
			g.x = n->gpos.x + n->region_pos.x * s;
			g.y = n->gpos.y + n->region_pos.y * s;
			
			AStarNode * rc = open_map[g];
			
			if (rc == NULL){
				rc = new AStarNode(m, m->getRegionAt(n->region_pos), n, best_node);
				open_map[g] = rc;
				float cost = best_node->getG() + rc->getCost();
			
				rc->setG(cost);
			
				float hval = rc->calcH(end);
			
				rc->setF(cost + hval);
			
			
				pq.push(rc);
				best_node->addNode(rc);
			}
			
			else {
				if (rc->closed){
					float cost = best_node->getG() + rc->getCost();
					if (cost < rc->getG()){
						rc->setG(cost);
						rc->setF(cost + rc->calcH(end));	
						pq.push(rc);
						rc->closed = 0;				
					}
				}
				
				
				
				
			}
		}
		
	}

	AStarNode * p=NULL;
	AStarNode * parent= goalNode;
	
	RouteNode * path_iterator = NULL;
	RouteNode * path_start = new RouteNode(NULL, NULL, parent->getPos());
	RouteNode * path_end = path_start;
	while(parent!=NULL){
	
		p = parent;
		
		path_iterator = path_start;
		path_start = new RouteNode(path_iterator, NULL, p->getPos());
		path_iterator->setParent(path_start);
		p->route = 1;
		parent = p->parent;
	}
	
	Route * route = new Route(path_start, path_end);
	
	map<GridLoc, AStarNode *, gclcmp>::iterator it;
	
	for (it = open_map.begin(); it != open_map.end(); it ++){
		AStarNode * a = (*it).second;
		delete a;
	}
	route->smooth();
	return route;
}

Route * calcRoughRoute(Map * m, vec2 start, vec2 end, float stepsize){


	priority_queue<AStarNode *, vector<AStarNode*>, nodecomparison> pq;
	map<GridLoc, AStarNode *, gclcmp> open_map;
	
	Region * r = m->getRegionAt(end);
	Cell * c = r->getCellAt(end);
	
	if (c->impassable) {
		return NULL;
	}
	
	r = m->getRegionAt(start);
	c = r->getCellAt(start);
	
	
	
	float s = m->getRegionSize();
	
	AStarNode * root = new AStarNode(m, start);
	root->setG(0);
	root->setF(root->calcH(end));
	
	GridLoc g;
	g.x = c->gpos.x + c->region_pos.x * s;
	g.y = c->gpos.y + c->region_pos.y * s;
	
	open_map[g] = root;
	
	pq.push(root);
	
	
	int searching = 1;
	
	AStarNode * goalNode = NULL;
	
	CellRay ray(m);
	
	
	while (searching){
		
		AStarNode * best_node;
		if (!pq.empty()){
			best_node = pq.top();
		}
		else {
			printf("No path found!\n");
			
			return NULL;
		}
		
		vec2 best_pos = best_node->getPos();
		
		if (fabs(best_pos.x-end.x) + fabs(best_pos.y - end.y) < stepsize){
			goalNode = best_node;
			searching = 0;
			break;
		}
		
		pq.pop();
		
		best_node->closed = 1;
		
		
		for (int j = 0; j < 10; j++){
			float angle = ((2.0*PI)/10.0)*j;
			
			double ds, dc;
			
			sincos(angle, &ds, &dc);
			
			vec2 newpos;
			vec2 pos(best_pos.x,best_pos.y);
			
			newpos.x = pos.x + dc*stepsize;
			newpos.y = pos.y + ds*stepsize;
			
			Region * r = m->getRegionAt(newpos);
			
			Cell * cell = r->getCellAt(newpos);
			
			GridLoc g;
			g.x = cell->gpos.x + cell->region_pos.x * s;
			g.y = cell->gpos.y + cell->region_pos.y * s;
			
			AStarNode * node = open_map[g];
			
			
			if(node==NULL){
				
				float cost = ray.getMovementCost(pos, newpos);//best_node->getG() + node->getCost();
				if (cost == -1) continue;
			
				node = new AStarNode(m, r, cell, best_node);
				open_map[g] = node;
			
				node->setG(cost);
			
				float hval = node->calcH(end);
			
				node->setF(cost + hval);
			
				pq.push(node);
				best_node->addNode(node);
			}	
			else {
				if (node->closed){
					float cost = best_node->getG() + node->getCost();
					if (cost < node->getG()){
						node->setG(cost);
						node->setF(cost + node->calcH(end));	
						pq.push(node);
						node->closed = 0;				
					}
				}
				
			}		
			
		}
		
	}

	AStarNode * p=NULL;
	AStarNode * parent= goalNode;
	
	RouteNode * path_iterator = NULL;
	RouteNode * path_start = new RouteNode(NULL, NULL, parent->getPos() + vec2(.5,.5));
	RouteNode * path_end = path_start;
	while(parent!=NULL){
	
		p = parent;
		
		path_iterator = path_start;
		path_start = new RouteNode(path_iterator, NULL, p->getPos()+ vec2(.5,.5));
		path_iterator->setParent(path_start);
		p->route = 1;
		parent = p->parent;
	}
	
	Route * route = new Route(path_start, path_end);
	
	map<GridLoc, AStarNode *, gclcmp>::iterator it;
	
	for (it = open_map.begin(); it != open_map.end(); it ++){
		AStarNode * a = (*it).second;
		delete a;
	}
	//route->smooth();
	return route;
	
}
