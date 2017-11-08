#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <utility>
#include <vector>
#include <math.h>
#define CONFLICT_NONE 0

struct RGB_Color {
	float r = 0;
	float g = 0;
	float b = 0;
	bool check() {
		if (r> 1 || g>1 || b>1) return false;
		if (r<0 || g<0 || b<0) return false;
		return true;
	}
};

class Object {
public:
	virtual void draw() = 0;
	virtual void move(int state) = 0;
	virtual void reflect(float &x, float &y, int conflict) = 0; // r = d-2(d.n)n
	virtual int conflict(std::vector<float> &X, std::vector<float> &Y) = 0;
	void get_pos(float &x, float &y) {
		x = this->posx;
		y = this->posy;
	}
	
protected:
	float posx = 0;
	float posy = 0;
	struct RGB_Color color; 
};

void normal (float &x , float &y) {
	float s = pow(pow(x,2)+pow(y,2),0.5);
	x = x/s;
	y = y/s;
}

#endif // OBJECTS_HPP
