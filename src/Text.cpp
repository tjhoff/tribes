#include "Text.h"

static GLuint vbo;
static GLuint attribute_coord;
static GLuint uniform_tex;
static GLuint uniform_scale;
static GLuint uniform_color;
static GLuint program;
static GLfloat scale;
static float color[3];

void Text::bind(){
}

void Text::unbind(){
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
}

int Text::initialize(){

	scale = 1.5;
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;

	FontManager::initialize();
	char * fname = "fonts/SVBasicManual.ttf";
	FontManager::loadFont(fname , 64);
	FontManager::useFont(fname , 64);
	
	Shader sh((char*)"shaders/text.vs", (char*)"shaders/text.fs");
	program = sh.getID();

	const char * attribute_name;
	attribute_name = "coord";
	attribute_coord = glGetAttribLocation(program, attribute_name);
	
	if(attribute_coord == -1){
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	const char * uniform_name;
	uniform_name = "tex";
	uniform_tex = glGetUniformLocation(program, uniform_name);
	if (uniform_tex == -1){
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}
	
	uniform_name = "color";
	uniform_color = glGetUniformLocation(program, uniform_name);
	if (uniform_tex == -1){
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}
	
	glGenBuffers(1 , &vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
	return 0;
}

void Text::renderString(float x, float y, char* str, float max_width, float max_height){

	
	font * f = FontManager::getCurrentFont();
	
	// Shader junk
	
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, f->font_tex);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(uniform_tex, 0);
	glUniform3f(uniform_color, color[0],color[1],color[2]);
	
	// VBO junk
	
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	float original_x = x;
	float original_y = y;
	
	fontpoint coords[6*strlen(str)];
	int n = 0;

	for (const char * p = str; *p; p++){
	
		if (*p == '\n'){
			y += scale;
			x = original_x;
			continue;
		}
		if (*p == '\t'){
			font_char c = f->c[' '];
			x+= 4 * scale * (c.ax / (float)f->max_width);
			continue;
		}
		if (max_width && x > (original_x + max_width)){
			y += scale;
			x = original_x;
		}
		if (max_height && y > (original_y + max_height)){
			continue;
		}
		font_char c = f->c[*p];
		float width = scale * c.bw / (float)f->max_width;
		float newx = x;
		float height = scale * c.bh / (float)f->height;
		float newy = y - height - scale *  (1 - (c.bt/(float)f->height));// (scale * ((f->height - c.bt) / (float)f->height));
		
		float width_part = c.bw / (float)f->width;
		float height_part = c.bh / (float)f->height;
		
		coords[n] = (fontpoint){newx, newy, c.tx, height_part}; n++;
		coords[n] = (fontpoint){newx + width, newy, c.tx + width_part, height_part}; n++;
		coords[n] = (fontpoint){newx, newy + height, c.tx, 0}; n++;
		coords[n] = (fontpoint){newx + width, newy, c.tx + width_part, height_part}; n++;
		coords[n] = (fontpoint){newx, newy + height, c.tx, 0}; n++;
		coords[n] = (fontpoint){newx + width, newy + height, c.tx + width_part, 0}; n++;
		
		x+= scale *(c.ax / (float)f->max_width);
		
	}
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
	
	glDrawArrays(GL_TRIANGLES, 0, n);
	
	glDisableVertexAttribArray(attribute_coord);
	
}

void Text::setScale(float nscale){
	scale = nscale;
}

void Text::setColor(float acolor[3]){
	color[0] = acolor[0];
	color[1] = acolor[1];
	color[2] = acolor[2];
}
