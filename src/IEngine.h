//////////////////////////////////////////
///           IEngine.h
//////////////////////////////////////////

#pragma once
#define GL_GLEXT_PROTOTYPES
#include <math.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/thread.hpp>
#include "Vec2.h"
#include "Vec3.h"
#include "Matrix.h"
#include "Text.h"
#include "Window.h"
#include "Texture.h"
#include "GameState.h"
#include "CellRay.h"
#include "Route.h"
#include "Map.h"
#include "Man.h"
#include "Tribe.h"
#include <vector>
#include <stack>
#include <list>

struct Camera{
	float x;
	float y;
	float z;
	float pitch;
	float yaw;
	float roll;
};

class IEngine
{
	public:
		IEngine(int argc, char** argv);
		void initGL(int argc, char** argv);
		void checkKeys();
		void drawScene();
		void update();
		void resize(int width, int height);
		int begin();
		
		// ext crap

		//
		
	private:
	
		// ext crap
		
		Map * m;
		Shader * regionShader;
		
		unsigned int man_tex;
		
		Cell * current_cell;
		
		Man * mans;
		Tribe tribe;
		int num_mans;
		vec2 dest;
		
		int stringdisplay;
		
		Camera camera;
		
		int cooldown;
		
		vec2 mouseCoords;
		
		Window w;
		
		mousebutton mb;
		
		//
	
		double m_time;
		sf::Window* m_window;
		sf::Clock* m_clock;
		int frames;
		float time;
		long int update_ticks;
		long int draw_ticks;
		float fps;
		float m_updateRate;
		
		float gl_size;
		
		float m_width_ratio;
		float m_width;
		float m_height;
		
		float gl_width;
		float gl_height;
		
		float gl_min_width;
		float gl_min_height;
		
};

