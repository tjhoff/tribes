#version 120

void main(){
	
	float len = (gl_FragCoord - vec4(300,400,0,0)).length();

	float luminosity = 1.0 - (len/10.0);
	
	luminosity = clamp(luminosity, 0, 1);

	gl_FragColor = vec4(vec3(luminosity), 1);
}
