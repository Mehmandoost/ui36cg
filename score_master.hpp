#ifndef SCORE_MASTER_HPP
#define SCORE_MASTER_HPP

#include "objects.hpp"
#include "settings.hpp"
#include "used_strings.hpp"
#include <stdlib.h>
#include <GL/glut.h>
#include <string>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <map>


class ScoreMaster: public Object {
	
public:
	ScoreMaster(Settings *cfg){
		try {
			this->posx = std::stof (cfg->get_Value(SCORE_MASTER, SCORE_MASTER_XPOS));
			this->posy =  std::stof(cfg->get_Value(SCORE_MASTER, SCORE_MASTER_YPOS));
			this->color.r = std::stoi(cfg->get_Value(SCORE_MASTER, SCORE_MASTER_COLOR_R))/256.0;
			this->color.g = std::stoi(cfg->get_Value(SCORE_MASTER, SCORE_MASTER_COLOR_G))/256.0;
			this->color.b = std::stoi(cfg->get_Value(SCORE_MASTER, SCORE_MASTER_COLOR_B))/256.0;
			this->cfg = cfg;
		} catch(...){
			std::cout << "failed to create Score Master." << std::endl;
			exit(EXIT_FAILURE);
		}
	};
	
	void draw() {
		glColor3f(this->color.r, this->color.g, this->color.b);
		std::string text = "";
		for (std::map<std::string, int>::iterator it=this->scores.begin(); it!=this->scores.end(); ++it){
			text += it->first + ": " + std::to_string(it->second) + " ";
		}

		/* https://www.youtube.com/watch?v=elE__Nouv54 */
		glMatrixMode(GL_PROJECTION);
		double *matrix = new double[16];
		glGetDoublev(GL_PROJECTION_MATRIX,matrix);
		glLoadIdentity();
		glOrtho(0,800,0,600,-5,5);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2i(posx,posy);
		for ( int i = 0 ; i < text.length() ; i++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matrix);
		glMatrixMode(GL_MODELVIEW);
    	
	};
	
	void move(int state) {
	};
 
        void reflect(float &x, float &y, int confilict) {
		
	};
	
	int conflict(std::vector<float> &X, std::vector<float> &Y) {		
		return CONFLICT_NONE;
		
	};

	bool got_a_confilict(int state) {
		bool return_state  = false;
		for (std::map<std::string, int>::iterator it=this->score_loc.begin(); it!=this->score_loc.end(); ++it){
			if(it->second == state){
				this->scores[it->first]++;
				return_state = true;
			}
		}
		
		return return_state;
	};
	
	void register_a_player(std::string name) {
		std::string score = this->cfg->get_Value(name, PLAYER_SCORE);
		boost::to_upper(score);
		this->scores[name] = 0;
		if (score == "CONFLICT_FRAME_TOP"){
			this->score_loc[name] = 1;
			return;
		}
	        if (score == "CONFLICT_FRAME_DOWN"){
			this->score_loc[name] = 2;
			return;
		}
		if (score == "CONFLICT_FRAME_LEFT" ){
			this->score_loc[name] = 3;
			return;
		}
		if (score == "CONFLICT_FRAME_RIGHT"){
			this->score_loc[name] = 4;
			return;
		}
		if (score == "CONFLICT_PLAYER_TOP" ){
			this->score_loc[name] = 5;
			return;
		}
		if (score == "CONFLICT_PLAYER_DOWN"){
			this->score_loc[name] = 6;
			return;
		}
		if (score == "CONFLICT_PLAYER_LEFT"){
			this->score_loc[name] = 7;
			return;
		}
		if (score == "CONFLICT_PLAYER_RIGHT"){
			this->score_loc[name] = 8;
			return;
		}

		std::cout << "[Error] unknown score location[" << score << "] for player[" << name << "]." << std::endl;
		exit(EXIT_FAILURE);
		
	};
	
private:
	std::map <std::string, int> score_loc;
	std::map <std::string, int> scores;
	float size_x = 0.1;
	float size_y = 0.5;
	float speed = 0.001;
	Settings *cfg;
};

#endif // SCORE_MASTER_HPP
