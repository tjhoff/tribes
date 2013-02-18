attribute vec3 vertex;
attribute vec3 normal;

varying vec3 vx_transform;

void main(){
	gl_Position = vec4(v_vertex, 1) * gl_ModelViewProjectionMatrix;
}
