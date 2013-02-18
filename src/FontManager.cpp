#include "FontManager.h"

static FT_Library lib;
static std::map<char*, font *> fonts;
static font * current_font;

int FontManager::initialize(){
	
	int error = FT_Init_FreeType(&lib);
	if (error){
		fprintf(stderr, "Freetype initialization error - quitting\n");
		return 1;
	}
	fprintf(stderr, "Fontmanager initialization complete\n");
	return 0;
}

font * FontManager::getCurrentFont(){
	return current_font;
}

int FontManager::loadFont(char* fontName, int fontSize){

	FT_Face face;
	char str[64];
	strcpy(str, FONTS_DIR);
	strcat(str, fontName);
	
	if (FT_New_Face(lib, fontName, 0, &face)){
		fprintf(stderr, "Load fail on %s\n", fontName);
		return 1;
	}
	
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	
	FT_GlyphSlot g = face->glyph;
	
	int w = 0;
	int h = 0;
	int max_w = 0;
	
	for(int i = 32; i< 128; i++){
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)){
			fprintf(stderr, "Loading %c failed\n", i);
			continue;
		}
		w += g->bitmap.width;
		int tmp = g->bitmap.rows;
		if (tmp > h){
			h = tmp;
		}
		tmp = g->bitmap.width;
		if (tmp > max_w) max_w = tmp;
	}
	
	GLuint tex;
	
	glEnable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	
	glGenTextures(1, &tex);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	if (int err = glGetError()){
		fprintf(stderr, "error in bind: %s\n", gluErrorString(err));
		exit(0);
	}
	glTexImage2D(GL_TEXTURE_2D, 0,
					GL_ALPHA, w, h,
					0, GL_ALPHA,
					GL_UNSIGNED_BYTE, 0);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int x = 0;
	
	font_char * c = new font_char[128];
	for (int i = 32; i< 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			continue;
		glTexSubImage2D(GL_TEXTURE_2D, 0,
						x, 0, g->bitmap.width,
						g->bitmap.rows, GL_ALPHA,
						GL_UNSIGNED_BYTE, g->bitmap.buffer);
		c[i].ax = g->advance.x >> 6;
		c[i].ay = g->advance.y >> 6;
		
		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;
		
		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;
		
		c[i].tx = (float)x / w;
		
		x += g->bitmap.width;
	}
	
	sprintf(str, "%s%i", fontName, fontSize);
	font * f = new font;
	f->font_tex = tex;
	f->c = c;
	f->width = w;
	f->max_width = max_w;
	f->height = h;
	fonts[str] = f;
	
	//fontToFile(f, str);
	
	return 0;
}

void FontManager::fontToFile(font * f, char * str){

	glBindTexture(GL_TEXTURE_2D, f->font_tex);
	
	unsigned char * pixels;

	int height;
	int width;

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	pixels = new unsigned char[4*width * height];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	sf::Image img(width, height, (const unsigned char *) pixels);
	
	char filename[64];
	strcpy(filename, "oglfonts/");
	strcat(filename, str);
	strcat(filename, ".png");
	
	img.SaveToFile(filename);
	
	strcpy(filename, "oglfonts/");
	strcat(filename, str);
	strcat(filename, ".finfo");
	FILE * out = fopen(filename, "w");
	
	for (int i = 32; i< 128; i++){
		font_char c = f->c[i];
		fprintf(out, "%i %i %i %i %i %i %f\n", c.ax, c.ay, c.bw, c.bh, c.bl, c.bt, c.tx);
	}
	
	fclose(out);
	
	fprintf(stderr, "loaded %s\n", str);

}

int FontManager::useFont(char* fontName, int fontSize){
	char str[64];
	sprintf(str, "%s%i", fontName, fontSize);	
	
	std::map<char*, font *>::iterator it;
	it = fonts.find(str);
	
	if (it == fonts.end()){
		return 1;
	}
	
	current_font = (it->second);
	
	fprintf(stderr, "%s current\n", str);
	if (int err = glGetError()){
		fprintf(stderr, "error in usefont: %s\n", gluErrorString(err));
	}
	return 0;
}
