#version 130
 
uniform sampler2D tex;
uniform vec2 pos;
uniform float time;
 
int LFSR_Rand_Gen(in int n)
{
  // <<, ^ and & require GL_EXT_gpu_shader4.
  n = (n << 13) ^ n; 
  return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}

float LFSR_Rand_Gen_f( in int n )
{
  return float(LFSR_Rand_Gen(n));
}

float noise3f(in vec3 p)
{
  ivec3 ip = ivec3(floor(p));
  vec3 u = fract(p);
  u = u*u*(3.0-2.0*u);

  int n = ip.x + ip.y*57 + ip.z*113;

  float res = mix(mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*0)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*0)),u.x),u.y),
                 mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*1)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*1)),u.x),u.y),u.z);

  return 1.0 - res*(1.0/1073741824.0);
}
 
void main(void) {
	vec2 t_coords = gl_TexCoord[0].xy;
	vec4 color = vec4(texture2D(tex, t_coords).rgba);// vec4(1, 1, 1, texture2D(tex, texcoord).a)* vec4(1,1,1,1);
	if (color.a >0.0){
		float add;
		float x = abs(t_coords.x + pos.x);
		float y = abs(t_coords.y + pos.y);
		float r = atan(y, x);
		float l = length(vec2(x,y));
		add = noise3f(vec3((color.b*10+cos(l*.1)*5+x)*10, (color.b*10+sin(l*.1)*5+y)*10, (time+color.b+sin(time- color.b *5)*.1 -sin(time+color.b*10))*.4));
		if (add > .8) add = (1-color.b)-.2;
		else if (add < -.2) add = -.1;
		else add = 0;
		color += vec4(add);
	}
	color.a = 1.0;
	gl_FragColor = color;
}
