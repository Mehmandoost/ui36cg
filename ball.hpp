#ifndef BALL_HPP
#define BALL_HPP
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>


#include "objects.hpp"
#include "settings.hpp"
#include "score_master.hpp"

class Ball: public Object {
public:
	Ball(Settings *cfg, std::vector<Object *> *objs, ScoreMaster *sm) : Object() {
		try {
		this->set_center();
		this->color.r = std::stoi(cfg->get_Value(BALL, BALL_COLOR_R))/256.0;
		this->color.g = std::stoi(cfg->get_Value(BALL, BALL_COLOR_G))/256.0;
		this->color.b = std::stoi(cfg->get_Value(BALL, BALL_COLOR_B))/256.0;
		if (!this->color.check()) throw 1;
		this->size = std::stof(cfg->get_Value(BALL, BALL_SIZE));
		if (this->size > 1 || this->size <= 0) throw 1;
		this->score_master= sm;
		} catch(...) {
			std::cout << "failed to create Ball" << std::endl;
			exit(EXIT_FAILURE);
		}
		this->objs = objs;
	};
	
	virtual void draw() {
		glColor3f(this->color.r, this->color.g, this->color.b);
		glBegin(GL_POLYGON);
		glVertex2f(this->posx, this->posy);
		glVertex2f(this->posx+size, this->posy);
		glVertex2f(this->posx+size, this->posy+size);
		glVertex2f(this->posx, this->posy+size);
		glEnd();
	};
	
	void set_center(){
		this->posy = this->posx = (0.0 - size/2);
		this->dirx = this->diry = 0;
	};
	void inc_speed (float speed) {
		this->dirx *= speed;
		this->dirx *= speed;
	}

	void dec_speed (float speed) {
		this->dirx /= speed;
		this->dirx /= speed;
	}
	

	bool is_centered() {
		if (this->posy == this->posx && this->posx == (0.0 - size/2))
			return true;
		else false;
	}
	
	int conflict(std::vector<float> &X, std::vector<float> &Y) {
		return CONFLICT_NONE;
	};

	void move(int state) {
		this->posx += this->dirx;
		this->posy += this->diry;

		std::vector<float> x;
		std::vector<float> y;
		x.push_back(this->posx);
		y.push_back(this->posy);
		
		x.push_back(this->posx);
		y.push_back(this->posy+size);
		
		x.push_back(this->posx+size);
		y.push_back(this->posy);

		x.push_back(this->posx+size);
		y.push_back(this->posy+size);
		
		for ( auto obj : *objs) {
			int state = obj->conflict(x, y);
			if (state != CONFLICT_NONE) {
				obj->reflect(this->dirx, this->diry, state);
				if (this->score_master->got_a_confilict(state))
					this->set_center();
				break;
			}

		}
	};

	void set_direction(float x, float y) {
		this->dirx = x;
		this->diry = y;
	}

	void reflect(float &x, float &y, int conflict) {
	};
	
private:
	float size = 0.1;
	float dirx = 0;
	float diry = 0;
	std::vector<Object *> *objs;
	ScoreMaster *score_master = NULL;
};

#endif // BALL_HPP
