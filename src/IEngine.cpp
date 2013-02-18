//////////////////////////////////////////
///           IEngine.cpp
//////////////////////////////////////////

int mouse_x = 0;
int mouse_y = 0;
float gl_x = 0;
float gl_y = 0;

#include "IEngine.h"

#include <stdlib.h>

IEngine::IEngine(int argc, char** argv)
{
	printf("Initializing IEngine\n");
	initGL(argc, argv);
	m_width = 800;
	m_height = 600;
	gl_size = 100;
	resize(m_width, m_height);
	m_time = 0.0;
	frames = 0;
	m_updateRate = 1/100.0;


	//ext crap
	
	int growing = 1;
	
	camera.x = 0;
	camera.y = 900;
	camera.z = 0;
	camera.pitch = -90;
	camera.yaw = 180;
	
	dest = vec2(0,0);
	
	num_mans = 10;
	
	current_cell = NULL;
	
	mans = new Man[num_mans];
	double f_s, f_c;
	
	
	glEnable(GL_TEXTURE_2D);
	Text::initialize();

	mouseCoords = vec2(0,0);
	man_tex = Texture::loadTexture("tex/man.png");
	m = new Map();
	for (int i = 0; i < num_mans; i++){
		mans[i].init();
		mans[i].map = m;
		tribe.addPerson(&mans[i]);
	}
	
	
	//	
	
}

void IEngine::initGL(int argc, char** argv)
{
	m_window = new sf::Window(sf::VideoMode(800, 600, 32), "IEngine");
	m_clock = new sf::Clock();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPointSize(5.0);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	
	if (int err = glGetError()){
		fprintf(stderr, "error in initgl: %s\n", gluErrorString(err));
		exit(0);
	}
}


void IEngine::checkKeys(){

	
	const sf::Input& input = m_window->GetInput();
	
	float mx = input.GetMouseX();
	float my = input.GetMouseY();
	
	// Convert pixel space into (1*ratio,-1*ratio,1,-1) unit space
	mx = 2*(mx - m_width / 2.0) / m_width * m_width_ratio;
	my = -2*(my - m_height / 2.0) / m_height;
	
	// Convert into gl unit space
	mx = mx * 100.0;
	my = my * 100.0;
	
	mouseCoords.x = mx;
	mouseCoords.y = my;
	mb = MB_NONE;
	bool lbutton = input.IsMouseButtonDown(sf::Mouse::Left);
	bool rbutton = input.IsMouseButtonDown(sf::Mouse::Right);
	
	if (lbutton){
		mb = MB_LEFT;
	}
	if (rbutton){
		mb = MB_RIGHT;
	}
	
	bool space = input.IsKeyDown(sf::Key::Space);
	
	bool up = input.IsKeyDown(sf::Key::Up);
	bool down = input.IsKeyDown(sf::Key::Down);
	bool right = input.IsKeyDown(sf::Key::Right);
	bool left = input.IsKeyDown(sf::Key::Left);
	
	bool shift = input.IsKeyDown(sf::Key::LShift) || input.IsKeyDown(sf::Key::RShift);
	
	bool m = input.IsKeyDown(sf::Key::M);
	bool w = input.IsKeyDown(sf::Key::W);
	bool d = input.IsKeyDown(sf::Key::D);
	bool s = input.IsKeyDown(sf::Key::S);
	bool a = input.IsKeyDown(sf::Key::A);
	double x, y;
	
	if (m){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (w){
		camera.y*=.95;
	}	
	
	if (s){
		camera.y*=1.05;
	}	
	
	if (a){
			camera.roll ++;
	}
	
	if (d){
			camera.roll --;
	}
	
	
	if (up){
		/*double pitch_s, pitch_c, yaw_s, yaw_c;
		sincos(radians(camera.pitch), &pitch_s, &pitch_c);
		sincos(radians(camera.yaw), &yaw_s, &yaw_c);
		camera.x += (-yaw_s * pitch_c)*3;
		camera.y += (pitch_s)*3;
		camera.z += (-yaw_c * pitch_c)*3;*/
		double roll_s, roll_c;
		sincos(radians(camera.roll), &roll_s, &roll_c);
		dest.y += 5.0 * (camera.y/1000.0) * roll_c;
		dest.x += 5.0 * (camera.y/1000.0)* roll_s;
	}
	
	if (down){
		/*double pitch_s, pitch_c, yaw_s, yaw_c;
		sincos(radians(camera.pitch), &pitch_s, &pitch_c);
		sincos(radians(camera.yaw), &yaw_s, &yaw_c);
		camera.x += (yaw_s * pitch_c)*3;
		camera.y += (-pitch_s)*3;
		camera.z += (yaw_c * pitch_c)*3;*/
		double roll_s, roll_c;
		sincos(radians(camera.roll), &roll_s, &roll_c);
		dest.y -= 5.0 * (camera.y/1000.0)* roll_c;
		dest.x -= 5.0 * (camera.y/1000.0)* roll_s;
	}
	
	if (left){
		double roll_s, roll_c;
		sincos(radians(camera.roll), &roll_s, &roll_c);
		dest.y -= 5.0 * (camera.y/1000.0)* roll_s;
		dest.x += 5.0 * (camera.y/1000.0)* roll_c;
	}
	if (right){
		double roll_s, roll_c;
		sincos(radians(camera.roll), &roll_s, &roll_c);
		dest.y += 5.0 * (camera.y/1000.0)* roll_s;
		dest.x -= 5.0 * (camera.y/1000.0)* roll_c;
	}
	
	if (space){
		for (int i = 0; i < num_mans; i++){
			if (mans[i].traveling) continue;
			if (mans[i].pathing_bool){
				mans[i].setDestination(randSpread(dest, 5.0));
			}
		}
	}
	
}

int IEngine::begin()
{

	while(m_window->IsOpened())
	{
		sf::Event Event;
		while(m_window->GetEvent(Event))
		{
			if(Event.Type == sf::Event::Closed)
			{
				m_window->Close();
				return 0;
			}
			
			else if(Event.Type == sf::Event::KeyPressed)
			{
				// For one-press keys: (starts to repeat after a while)
				if(Event.Key.Code == sf::Key::Escape)
				{
					m_window->Close();
					return 0;
				}
			}
			else if(Event.Type == sf::Event::Resized)
			{
				resize(Event.Size.Width, Event.Size.Height);
			}
			
		}
		
		checkKeys();
		
		update();
		
		drawScene();
		
		m_window->Display();
	}
	return 0;
}


void IEngine::drawScene()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,m_width_ratio,5,5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	glRotatef(-camera.roll,0,0,1);
	glRotatef(-camera.pitch, 1 , 0, 0);
	
	glRotatef(-camera.yaw, 0, 1, 0);
	
	glTranslatef(-camera.x,-camera.y,-camera.z);
	long int t = clock();
	
	
	glDisable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
	
	float c_x = 100 * camera.y;
	float c_z = 100 * camera.y;
	
	glColor3f(.3,.3,.3);
	
	glBegin(GL_TRIANGLES);
	
	glVertex3f(-c_x, 0, -c_z);
	glVertex3f(c_x, 0, -c_z);
	glVertex3f(c_x, 0, c_z);
	
	glVertex3f(-c_x, 0, -c_z);
	glVertex3f(-c_x, 0, c_z);
	glVertex3f(c_x, 0, c_z);
	
	glEnd();
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	
	glColor3f(1.0,1,1);
	m->draw();
	draw_ticks = clock()-t;
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, man_tex);
	
	glBegin(GL_TRIANGLES);
		
		for (int i = 0; i < num_mans; i++){
			Man man = mans[i];
			float lval = .9*man.light + .1;
			glColor3f(1.0*lval,0*lval,0*lval);
			glTexCoord2f(0,0);
			glVertex3f(man.pos.x-man.size, 0, man.pos.y-man.size);
			glTexCoord2f(0,1);
			glVertex3f(man.pos.x-man.size, 0, man.pos.y+man.size);
			glTexCoord2f(1,1);
			glVertex3f(man.pos.x+man.size, 0, man.pos.y+man.size);
			
			glTexCoord2f(0,0);
			glVertex3f(man.pos.x-man.size, 0, man.pos.y-man.size);
			glTexCoord2f(1,0);
			glVertex3f(man.pos.x+man.size, 0, man.pos.y-man.size);
			glTexCoord2f(1,1);
			glVertex3f(man.pos.x+man.size, 0, man.pos.y+man.size);
		}
		
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0 * m_width_ratio,5.0 * m_width_ratio,-5.0,5.0,-5.0,5.0);
	//gluPerspective(45.0,m_width_ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	
	Text::bind();
	
	char str[128];
	Text::setScale(.2);
	float white[3] = {1,1,1};
	Text::setColor(white);
	sprintf(str, "%2.0f", fps);
	Text::renderString(-5*m_width_ratio,5,str);
	sprintf(str, "x: %f y: %f z: %f\n", camera.x, camera.y, camera.z);
	Text::renderString(-5*m_width_ratio,4.8,str);
	sprintf(str, "%i/%i/%i active/drawing/total\n", m->numberOfActiveRegions(), m->numberOfDrawnRegions(), m->numberOfTotalRegions());
	Text::renderString(-5*m_width_ratio,4.6,str);
	
	sprintf(str, "seed:%i \n", m->getSeed());
	Text::renderString(-5*m_width_ratio,4.4,str);
	if (current_cell != NULL){
		sprintf(str, "Cell (%2i,%2i) temp %3.3f humidity %2.2f height %3.3f", current_cell->gpos.x, current_cell->gpos.y, current_cell->temp, current_cell->humidity, current_cell->height);
		Text::renderString(-5*m_width_ratio,4.2,str);
	}
	float blue[3] = {0,1,1};
	float green[3] = {0,1,0};
	for (int i = 0; i < tribe.people.size(); i++){
		Text::setColor(blue);
		sprintf(str, "%s", tribe.people[i]->name);
		Text::renderString(-5*m_width_ratio,4.0-i*.4,str);
		Text::setColor(green);
		sprintf(str, "\t(%3.0f,%3.0f)",  tribe.people[i]->pos.x,  tribe.people[i]->pos.y);
		Text::renderString(-5*m_width_ratio,3.8-i*.4,str);
		
	}


	Text::unbind();
	
	glEnable(GL_DEPTH_TEST);
	
}


void IEngine::update()
{

	
	float newTime = m_clock->GetElapsedTime();
	float diff = newTime-time;
	
	float multiplier = 1.0;
	
	while(diff < m_updateRate)
	{
		newTime = m_clock->GetElapsedTime();
		diff = newTime-time;
    	continue;
    }
    time = newTime;

   	if(diff > m_updateRate)
	{
		multiplier = time / m_updateRate;
	}
	
	GameState::addGameTime(diff);
	
	fps = 1/diff;
	
	frames++;

	// ext crap
	
	camera.x = dest.x;
	camera.z = dest.y;
	
	
	
	GridLoc g;
	float s = m->getRegionSize();
	int x = floor(dest.x/ s);
	int y = floor(dest.y / s);
	
	g.x = (x);
	g.y = (y);
	
	m->loadRegionsAround(g,2);
	m->drawRegionsAround(g,2);
	
	Region * c_r;
	
	c_r = m->getRegionAt(dest);
	current_cell = c_r->getCellAt(dest);
	
	for (int i = 0; i < num_mans; i++){
		Man * man = &mans[i];
		
		man->update();
	}
	
	long int t = clock();
	m->update();
	update_ticks = clock()-t;
	
	//w.checkMouse(mouseCoords+b.pos, mb);
	
	
	//
}

void IEngine::resize(int width, int height)
{

	m_width_ratio = (height>0) ? (GLfloat)width/height : 1;
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,m_width_ratio,1,100000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
