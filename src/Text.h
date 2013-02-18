#pragma once

#include "FontManager.h"
#include "Shader.h"

class Text{
	
	public:
		static void bind();
		static void unbind();
		static int initialize();
		static void renderString(float x, float y, char* string, float max_width = 0, float max_height = 0);
		static void setScale(float nscale);
		static void setColor(float acolor[3]);
	private:
		struct fontpoint{
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};
		Text();
	
};
