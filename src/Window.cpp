#include "Window.h"

Window::Window(){
	tl = vec2(-20,50);
	w = 40.0;
	h = 12.0;
	moving = false;
	moved = false;
	lastPos = tl;
	locked = false;
}

void Window::addButton(Button b){
	buttons.push_back(b);
}

void Window::addTextBox(TextBox tb){
	texts.push_back(tb);
}

void Window::draw(){
	glColor3f(.2,.2,.2);
	glBegin(GL_QUADS);
	glVertex2f(tl.x, tl.y);
	glVertex2f(tl.x + w, tl.y);
	glVertex2f(tl.x + w, tl.y - h);
	glVertex2f(tl.x, tl.y - h);
	glEnd();
	glBegin(GL_QUADS);
	for (int i = 0; i<buttons.size(); i++){
		Button b = buttons[i];
		
		float u_bound = tl.y + b.u;
		float b_bound = u_bound - b.h;
		float l_bound = tl.x + b.l;
		float r_bound = l_bound + b.w;
		
		glColor3f(.6,.65,.7);
		if (b.clicked){
			glColor3f(.1,.1,.1);
		}
		else if (b.active){
			glColor3f(.4,.4,1.0);
		}
		glVertex2f(r_bound, u_bound);
		glVertex2f(r_bound, b_bound);
		glVertex2f(l_bound, b_bound);
		glVertex2f(l_bound, u_bound);
	}
	glEnd();
	
	Text::bind();
	
	for (int i = 0; i<buttons.size(); i++){
		Button b = buttons[i];
		float u_bound = tl.y + b.u;
		float l_bound = tl.x + b.l;
		Text::renderString(l_bound, u_bound, b.str);
	}
	
	for (int i = 0; i<texts.size(); i++){
		TextBox tb = texts[i];
		float u_bound = tl.y + tb.u;
		float l_bound = tl.x + tb.l;
		Text::renderString(l_bound, u_bound, tb.str);
	}
	
	Text::unbind();
	
}

void Window::checkMouse(vec2 mouse_pos, mousebutton mb){

	bool bclick = false;
	if (!moving){
		for (int i = 0; i<buttons.size(); i++){
			buttons[i].active = false;
			strcpy(buttons[i].str, "ON");
			Button b = buttons[i];
		
			if (b.clicked){
				strcpy(buttons[i].str, "OFF");
			}
		
			float u_bound = tl.y + b.u;
			float b_bound = u_bound - b.h;
			float l_bound = tl.x + b.l;
			float r_bound = l_bound + b.w;
		
			if (mouse_pos.x > l_bound && mouse_pos.x < r_bound){
				if (mouse_pos.y > b_bound && mouse_pos.y < u_bound){
					switch (mb){
						case (MB_NONE):
							buttons[i].active = true;
							break;
						case (MB_LEFT):
							buttons[i].active = true;
							if (!b.clicked){
								if (b.action != NULL){
									b.action->execute(b.id);
								}
							}
							buttons[i].clicked = true;
							bclick = true;
							break;
						case (MB_RIGHT):
							buttons[i].active = false;
							buttons[i].clicked = false;
							bclick = true;
							break;
							
					}
				}
			}
		}
	}
	if (!bclick && !locked){
		if (moving){
			if (mb == MB_LEFT){
				vec2 diff = (tl - lastPos) - (tl - mouse_pos);
				tl = tl + diff;
			}
			else {
				moving = false;
			}
		}
		else if (mb == MB_LEFT){
			if (mouse_pos.x < tl.x + w && mouse_pos.x > tl.x){
				if (mouse_pos.y < tl.y && mouse_pos.y > tl.y - h){
					moving = true;
					if (!moved){
						lastPos = mouse_pos;
						moved = true;
						return;
					}
					vec2 diff = (tl - lastPos) - (tl - mouse_pos);
					tl = tl + diff;
				
				}
			}
			else {
				moving = false;
			}
		}
	}
	lastPos = mouse_pos;
}
