#ifndef FRAME_HPP
#define FRAME_HPP

#include "objects.hpp"
#include "settings.hpp"
#include <stdlib.h>
#include <GL/glut.h>


#define CONFLICT_FRAME_TOP 1
#define CONFLICT_FRAME_DOWN 2
#define CONFLICT_FRAME_LEFT 3
#define CONFLICT_FRAME_RIGHT 4

#include <iostream>
class Frame: public Object {
public:
	Frame(){
	};
	
	void draw() {
	
	};
	
	void move(int state) {
	}
 
        void reflect(float &x, float &y, int confilict) {
		float n_x = 0;
		float n_y = 0;
		switch(confilict) {
		case CONFLICT_FRAME_TOP:
		case CONFLICT_FRAME_DOWN:
			y = -y;
			break;
	        
		case CONFLICT_FRAME_RIGHT:
		case CONFLICT_FRAME_LEFT:
			x = -x;
			break;
		}
	}
	
	int conflict(std::vector<float> &X, std::vector<float> &Y) {
		for (auto x : X) {
			if (x >= 1) return CONFLICT_FRAME_RIGHT;
			if (x <= -1) return CONFLICT_FRAME_LEFT;
		}
		for (auto y : Y) {
			if (y >= 1) return CONFLICT_FRAME_TOP;
			if (y <= -1) return CONFLICT_FRAME_DOWN;
		}
		return CONFLICT_NONE;
	};
	
	
private:
};

#endif // FRAME_HPP
