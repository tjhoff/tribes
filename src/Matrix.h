#pragma once

#include "Vec2.h"
#include <stdio.h>

class mat3x3{
	public:
		mat3x3();
		mat3x3(const mat3x3& m);
		
		mat3x3 identity();
		
		mat3x3 operator=(const mat3x3& source);
		mat3x3 operator+(const mat3x3& rhs){};
		mat3x3 operator-(const mat3x3& rhs){};
		mat3x3 operator*(const mat3x3& rhs);
		vec2 operator*(const vec2& rhs);
		
		mat3x3 translate(float x, float y);
		mat3x3 scale(float xscale, float yscale);
		mat3x3 rotate(float angle);
		
		bool operator==(const mat3x3& rhs);
		
		float vals[3][3];
};

void printMatrix(mat3x3 m);
