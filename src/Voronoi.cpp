#include "Voronoi.h"

vec2 lineIntersection(Line a, Line b, int &error){

	float ua = (b.b.x - b.a.x) * (a.a.y - b.a.y) - (b.b.y - b.a.y) * (a.a.x - b.a.x);
	float uad = (b.b.y - b.a.y) * (a.b.x - a.a.x) - (b.b.x - b.a.x) * (a.b.y - a.a.y);
	ua /= uad;
	if (fabs(uad) < DELTA){
		printf("Two lines are parallel...\n");
		error = 1;
		return a.a;
	}
	
	vec2 ret;
	ret.x = a.a.x + ua * (a.b.x - a.a.x);
	ret.y = a.a.y + ua * (a.b.y - a.a.y);
	
	return ret;

}

std::vector<Triangle> voronoize(Point * points, int num_points){

	std::stack<Triangle> possibles;
	std::vector<Triangle> actuals;
	
	Point * origin, * b, * c;
	vec2 m_ab, m_bc, n_ab, n_bc;
	
	for (int i = 0; i < num_points; i++){
		
		origin = &points[i];
	
		for (int j = 0; j < num_points; j++){
			if (j == i) continue;
			b = &points[j];
			
			Line ab_bisect;
		
			vec2 ab_vec = b->pos - origin->pos;
			float ab = (ab_vec).length();
			m_ab = (origin->pos + b->pos) * .5;
			n_ab = vec2(ab_vec.y, -ab_vec.x);
	
			ab_bisect.a = m_ab;
			ab_bisect.b = m_ab + n_ab;
		
			for (int k = 0; k < num_points; k++){
				if (k == i) continue;
				if (j == k) continue;
				c = &points[k];
			
				Triangle t;
				t.a = origin;
				t.b = b;
				t.c = c;
		
				Line bc_bisect;
				vec2 bc_vec = c->pos - b->pos;
				float bc = (bc_vec).length();
				m_bc = (b->pos + c->pos) * .5;
				n_bc = vec2(bc_vec.y, -bc_vec.x);
		
				bc_bisect.a = m_bc;
				bc_bisect.b = m_bc + n_bc;
		
				float ca = (c->pos - origin->pos).length();

				t.ab = ab_bisect;
				t.bc = bc_bisect;
				int error = 0;
				t.circumcenter = lineIntersection(ab_bisect, bc_bisect, error);
				if (error) continue;
				
				t.diameter = (2*ab*bc*ca) / sqrt((ab+bc+ca)*(-ab+bc+ca)*(ab-bc+ca)*(ab+bc-ca));
				possibles.push(t);
			}
		}
	}
	printf("%i\n", possibles.size());
	while (!possibles.empty()){
		
		Triangle t = possibles.top();
		possibles.pop();
		int invalid_triangle = 0;
		t.candidate = 1;
		for (int i = 0; i < num_points; i ++){
			int id = points[i].id;
			if ((id==t.a->id)||(id==t.b->id)||(id==t.c->id)){
				//printf("%i in [%i,%i,%i]\n", id, t.a->id, t.b->id,t.c->id);
				continue;
			}
			float dist = (points[i].pos - t.circumcenter).length();
			float radius = (t.diameter*.5);
			if (dist < radius){
				invalid_triangle = 1;
				t.candidate = 0;
				break;
			}
		}
		if (!invalid_triangle) actuals.push_back(t);
		
	}
	
	return actuals;
}
