#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <algorithm>
#include <SFML/Graphics.hpp>

#define FONTS_DIR ""

struct font_char{
	unsigned char ax;
	unsigned char ay;
	
	unsigned char bw;
	unsigned char bh;
	
	unsigned char bl;
	unsigned char bt;
	
	float tx;
};

struct font{
	GLuint font_tex;
	unsigned int width;
	unsigned int max_width;
	unsigned int height;
	font_char * c;
};

class FontManager{

	public:
		static int initialize();
		static int loadFont(char* fontName, int fontSize);
		static int useFont(char* fontName, int fontSize);
		static void fontToFile(font * f, char* str);
		static font * getCurrentFont();
		
	private:
		FontManager();
};
