#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "objects.hpp"
#include "settings.hpp"
#include "frame.hpp"
#include <stdlib.h>
#include <GL/glut.h>
#include <string>

#define CONFLICT_PLAYER_TOP 5
#define CONFLICT_PLAYER_DOWN 6
#define CONFLICT_PLAYER_LEFT 7
#define CONFLICT_PLAYER_RIGHT 8

#include <iostream>
class Player: public Object {
public:
	Player(Settings *cfg, std::string name, Frame *frame){
		try {
		this->posx = std::stof (cfg->get_Value(name, PLAYER_XPOS));
		this->posy =  std::stof(cfg->get_Value(name, PLAYER_YPOS));
		this->size_x =  std::stof(cfg->get_Value(name, PLAYER_XSIZE));
		this->size_y =  std::stof(cfg->get_Value(name, PLAYER_YSIZE));
		this->color.r = std::stoi(cfg->get_Value(name, PLAYER_COLOR_R))/256.0;
		this->color.g = std::stoi(cfg->get_Value(name, PLAYER_COLOR_G))/256.0;
		this->color.b = std::stoi(cfg->get_Value(name, PLAYER_COLOR_B))/256.0;
		this->speed =  std::stof(cfg->get_Value(name, PLAYER_SPEED));
		this->x_moving_direction = std::stof(cfg->get_Value(name, PLAYER_MOVING_DIRECTION_X));
		this->y_moving_direction = std::stof(cfg->get_Value(name, PLAYER_MOVING_DIRECTION_Y));
		float n = sqrt(pow(x_moving_direction,2) + pow(y_moving_direction,2));
		if (n != 0 ){
		this->x_moving_direction = this->x_moving_direction/n;
		this->y_moving_direction = this->y_moving_direction/n;
		}
		this->fwrd_key = cfg->get_Value(name, PLAYER_FORWARD_KEY).at(0);
		this->bkwrd_key = cfg->get_Value(name, PLAYER_BACKWARD_KEY).at(0);
		this->frame = frame;
		} catch( ... ) {
			std::cout << "failed to create Player" << std::endl;
			exit(EXIT_FAILURE);
		}

		
	};
	
	void draw() {
		glColor3f(this->color.r, this->color.g, this->color.b);
		glBegin(GL_POLYGON);
		glVertex2f(this->posx, this->posy);
		glVertex2f(this->posx+size_x, this->posy);
		glVertex2f(this->posx+size_x, this->posy+size_y);
		glVertex2f(this->posx, this->posy+size_y);
		glEnd();
	
	};
	
	virtual void move(int state) {
		if (state == 0) return;
		float new_x = posx;
		float new_y = posy;
		
		if (state == fwrd_key){
			new_x += (this->x_moving_direction * this->speed);
		        new_y += (this->y_moving_direction * this->speed);
		}
		else if (state == bkwrd_key){
		        new_x += ((-1.0)*this->x_moving_direction * this->speed);
			new_y += ((-1.0)*this->y_moving_direction * this->speed);
		}
		std::vector<float> x;
		std::vector<float> y;
		x.push_back(new_x);
		y.push_back(new_y);
		
		x.push_back(new_x);
		y.push_back(new_y+size_y);
		
		x.push_back(new_x+size_x);
		y.push_back(new_y);

		x.push_back(new_x+size_x);
		y.push_back(new_y+size_y);
		
		if (this->frame->conflict(x, y) == CONFLICT_NONE){
			posx = new_x;
			posy = new_y;
		}
		
	};
 
        void reflect(float &x, float &y, int confilict) {
		float n_x = 0;
		float n_y = 0;
		switch(confilict) {
		case CONFLICT_PLAYER_TOP:
		case CONFLICT_PLAYER_DOWN:
			y = -y;
			break;
	        
		case CONFLICT_PLAYER_RIGHT:
		case CONFLICT_PLAYER_LEFT:
			x = -x;
			break;
		}
	};
	
	int conflict(std::vector<float> &X, std::vector<float> &Y) {
		for (int i = 0; i < X.size(); ++i) {
			float x = X.at(i);
			float y = Y.at(i);
			if ( x >= posx && x<= posx+size_x && y >= posy && y<= posy+size_y) {
				int conflict = CONFLICT_PLAYER_DOWN;
				float min_distance = pow(y-posy,2);
				float d = pow( y-(posy+size_y), 2 );
				if ( d < min_distance ) {
					min_distance = d;
					conflict = CONFLICT_PLAYER_TOP;
				}

				d= pow (x-posx ,2);
				if ( d < min_distance ) {
					min_distance = d;
					conflict = CONFLICT_PLAYER_LEFT;
				}

				d = pow ( x-(posx+size_x),2 );
				if ( d < min_distance ) {
					min_distance = d;
					conflict = CONFLICT_PLAYER_RIGHT;
				}
				return conflict;
			}
		}
		return CONFLICT_NONE;
	};
	
private:
	float size_x = 0.1;
	float size_y = 0.5;
	float speed = 0.001;
	float x_moving_direction = 0;
	float y_moving_direction = 0;

	Frame *frame;
	int fwrd_key = 0;
	int bkwrd_key = 0;
};

#endif // PLAYER_HPP
