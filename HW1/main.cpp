#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "used_strings.hpp"
#include "settings.hpp"
#include "objects.hpp"
#include "ball.hpp"
#include "frame.hpp"
#include "player.hpp"
#include "score_master.hpp"

void init_window(Settings *cnfg);
void handlers_register();
void handler_display();
void handler_mouse(int button, int state, int x, int y);
void handler_physics();
void handler_reshape(int x, int y);
void handler_keyboard(unsigned char key, int x, int y);
void handler_menu_main(int id);
void handler_menu_speed(int id);

std::vector <Object *> objs;
int window_width;
int window_height;
Ball *ball;
Frame *frame;
ScoreMaster *score_master;

float ball_speed = 0;
float ball_speed_change = 1;

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cout << "Please pass config file location." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string settings_file(argv[1]);
	Settings cnfg(settings_file);

	ball_speed = std::stof(cnfg.get_Value(BALL, BALL_INIT_SPEED));
	ball_speed_change = std::stof(cnfg.get_Value(BALL, BALL_SPEED_CHANGE));
	
	if ( ball_speed_change == 0 ) {
		std::cout << "0 is not Acceptable for" << BALL << ":" << BALL_SPEED_CHANGE <<"." <<std::endl;
		exit(EXIT_FAILURE);
	}

	if ( ball_speed == 0 ) {
		std::cout << "0 is not Acceptable for" << BALL << ":" << BALL_INIT_SPEED <<"." <<std::endl;
		exit(EXIT_FAILURE);
	}

	
	frame = new Frame();
	objs.push_back(static_cast<Object *>(frame));

	score_master = new ScoreMaster(&cnfg);
	objs.push_back(static_cast<Object *>(score_master) );

	std::string players_string  = cnfg.get_Value(PLAYER_PLAYERS, PLAYER_PLAYERS);
	std::vector<std::string> player_names;
	boost::split(player_names, players_string, boost::is_any_of(", "), boost::token_compress_on);

	for ( auto name : player_names ) {
		Player * player = new Player(&cnfg, name, frame);
		objs.push_back(static_cast<Object *>(player));
		score_master->register_a_player(name);
	}
	
	ball = new Ball(&cnfg, &objs, score_master);
	objs.push_back(static_cast<Object *>(ball));
	
	
	init_window(&cnfg);
	
}


void init_window(Settings *cnfg) {
	try {
		int i = 0;
		glutInit(&i, NULL);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

		int width = std::stoi(cnfg->get_Value(WINDOW, WINDOW_WIDTH));
		int height = std::stoi(cnfg->get_Value(WINDOW, WINDOW_HEIGHT));
		glutInitWindowSize(width, height);
		window_height = height;
		window_width = width;

		int x_pos = std::stoi(cnfg->get_Value(WINDOW, WINDOW_X_POS));
		int y_pos = std::stoi(cnfg->get_Value(WINDOW, WINDOW_Y_POS));
		glutInitWindowPosition(x_pos, y_pos);

		glutCreateWindow(cnfg->get_Value(WINDOW, WINDOW_TITLE).c_str());

		handlers_register();
		glutMainLoop();
		
	}
	catch (...) {
		std::cout << "Could not init window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void handlers_register() {
	glutDisplayFunc(handler_display);
	glutMouseFunc(handler_mouse);
	glutReshapeFunc(handler_reshape);
	glutIdleFunc(handler_physics);
	glutKeyboardFunc(handler_keyboard);
	int speed_menu = glutCreateMenu(handler_menu_speed);
	glutAddMenuEntry("Increase", 2);
	glutAddMenuEntry("Decreese", 3);
	glutCreateMenu(handler_menu_main);
	glutAddSubMenu("Speed", speed_menu);
	glutAddMenuEntry("Exit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void handler_display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto i : objs) {
		i->draw();
	}
	glFlush();
}


void handler_mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		
		if (!ball->is_centered())
			return;
		
		float f_x = x - (window_width /2);
		float f_y = -1 * (y - (window_height/2));
		normal(f_x, f_y);

		ball->set_direction(f_x*ball_speed, f_y*ball_speed);
	}
}

void handler_physics() {
	for (auto obj : objs)
		obj->move(0);
	handler_display();
}

void handler_reshape(int w, int h) {
	window_width = w;
	window_height = h;
	glViewport(0,0,w,h);
}

void handler_keyboard(unsigned char key, int x, int y) {
	for (auto obj : objs)
		obj->move(key);
}


void handler_menu_main(int id) {
	switch (id) {
	case 1:
		exit(0);
		break;
	}
}

void handler_menu_speed(int id){
	switch (id) {
	case 2: // increase
		ball_speed *= ball_speed_change;
		ball->inc_speed(ball_speed_change);
		break;
	case 3: //decrease
		ball_speed /= ball_speed_change;
		ball->dec_speed(ball_speed_change);
		break;
	}
	
}
