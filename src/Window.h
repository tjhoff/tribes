#pragma once
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include "Vec2.h"
#include "Text.h"
#include "Callback.h"
#include <vector>
#include <stdio.h>

enum mousebutton{
	MB_NONE,
	MB_LEFT,
	MB_RIGHT,
	MB_MIDDLE
};

struct TextBox{
	float u;
	float l;
	float w;
	float h;
	char str[64];
};

struct Button{
	int id;
	float u;
	float l;
	float w;
	float h;
	bool active;
	bool disabled;
	bool clicked;
	fCallback * action;
	char str[64];
};

class Window{
	public:
		Window();
		void addButton(Button b);
		void addTextBox(TextBox tb);
		void draw();
		void checkMouse(vec2 mouse_pos, mousebutton mb = MB_NONE);
	private:
		std::vector<Button> buttons;
		std::vector<TextBox> texts;
		vec2 tl;
		float w;
		float h;
		bool moving;
		bool moved;
		bool locked;
		vec2 lastPos;
};
