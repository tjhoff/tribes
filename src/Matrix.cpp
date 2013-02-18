#include "Matrix.h"

mat3x3::mat3x3(){
	vals[0][0] = 1;
	vals[0][1] = 0;
	vals[0][2] = 0;
	
	vals[1][0] = 0;
	vals[1][1] = 1;
	vals[1][2] = 0;
	
	vals[2][0] = 0;
	vals[2][1] = 0;
	vals[2][2] = 1;
	
}

mat3x3::mat3x3(const mat3x3& m){
	vals[0][0] = m.vals[0][0];
	vals[0][1] = m.vals[0][1];
	vals[0][2] = m.vals[0][2];
	
	vals[1][0] = m.vals[1][0];
	vals[1][1] = m.vals[1][1];
	vals[1][2] = m.vals[1][2];
	
	vals[2][0] = m.vals[2][0];
	vals[2][1] = m.vals[2][1];
	vals[2][2] = m.vals[2][2];
}

mat3x3 mat3x3::operator=(const mat3x3& source){
	vals[0][0] = source.vals[0][0];
	vals[0][1] = source.vals[0][1];
	vals[0][2] = source.vals[0][2];
	
	vals[1][0] = source.vals[1][0];
	vals[1][1] = source.vals[1][1];
	vals[1][2] = source.vals[1][2];
	
	vals[2][0] = source.vals[2][0];
	vals[2][1] = source.vals[2][1];
	vals[2][2] = source.vals[2][2];
}

mat3x3 mat3x3::identity(){
	
	mat3x3 m;
	
	m.vals[0][0] = 1;
	m.vals[0][1] = 0;
	m.vals[0][2] = 0;
	
	m.vals[1][0] = 0;
	m.vals[1][1] = 1;
	m.vals[1][2] = 0;
	
	m.vals[2][0] = 0;
	m.vals[2][1] = 0;
	m.vals[2][2] = 1;
	
	return m;
	
}

mat3x3 mat3x3::operator*(const mat3x3& rhs){
	
	mat3x3 m;
	
	float sum;
	// FOR EACH ROW IN A
	for (int r = 0; r < 3; r++){
		
		// FOR EACH COLUMN IN B
		for (int c = 0; c < 3; c ++){
			sum = 0;
			// FOR EACH ELEMENT IN A AND B
			for (int e = 0; e < 3; e ++){
				sum += vals[r][e] * rhs.vals[e][c];
			}
			
			m.vals[r][c] = sum;
		}
	}
	return m;
}

vec2 mat3x3::operator*(const vec2& rhs){
	float v[3];
	float o[3];
	
	v[0] = rhs.x;
	v[1] = rhs.y;
	v[2] = 1.0;
	
	float sum;
	// FOR EACH ROW IN A
	for (int r = 0; r < 3; r++){
		
		sum = 0;
		// FOR EACH ELEMENT IN A ROWS AND V
		for (int e = 0; e < 3; e ++){
			sum += vals[r][e] * v[e];
		}
		
		o[r] = sum;
	}
	
	vec2 vec;
	
	vec.x = o[0];
	vec.y = o[1];
	
	return vec;
}

mat3x3 mat3x3::translate(float x, float y){
	
	mat3x3 m;
	
	m.vals[0][2] = x;
	m.vals[1][2] = y;
	
	return (*this) * m;
	

}

mat3x3 mat3x3::scale(float xscale, float yscale){
	
	mat3x3 m;
	
	m.vals[0][0] = xscale;
	m.vals[1][1] = yscale;
	
	return (*this) * m;
	

}

mat3x3 mat3x3::rotate(float angle){

	double s, c;
	sincos(angle, &s, &c);
	
	s = sin(angle);
	c = cos(angle);
	
	mat3x3 m;
	
	m.vals[0][0] = c;
	m.vals[0][1] = -s;
	m.vals[1][0] = s;
	m.vals[1][1] = c;
	
	return (*this)* m ;
	

}

void printMatrix(mat3x3 m){
	for (int i = 0; i < 3; i++){
		printf("%f %f %f\n", m.vals[i][0], m.vals[i][1], m.vals[i][2]);
	}
}
