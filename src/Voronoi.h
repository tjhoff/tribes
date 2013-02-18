#include "Vec2.h"
#include <stdio.h>
#include <stack>
#include <vector>

#define DELTA .000001

struct Line{
	vec2 a;
	vec2 b;
};

struct Point{
	vec2 pos;
	int id;
};

struct Triangle{
	Point * a;
	Point * b;
	Point * c;
	float diameter;
	vec2 circumcenter;
	int candidate;
	Line ab;
	Line bc;
};

vec2 lineIntersection(Line a, Line b, int &error);

std::vector<Triangle> voronoize(Point * parray, int num_points);
