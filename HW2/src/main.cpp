#include <GL/glew.h>
#include <GL/glut.h>


void handler_display();
void handler_reshape(int w, int h);
float vertices[] = {			 \
	-1.0, -1.0, 0.0, \
	1.0, -1.0, 0.0,  \
	0.0, 1.0, 0.0 };

float colors[] = {		    \
	1.0, 0.0, 0.0, 1.0, \
	0.0, 1.0, 0.0, 1.0, \
	0.0, 0.0, 1.0, 1.0 };
	
int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("VAO Test");

	glutDisplayFunc(handler_display);
	glutReshapeFunc(handler_reshape);
	glutMainLoop();
	
}


void handler_display() {

	glClear(GL_COLOR_BUFFER_BIT);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void handler_reshape(int w, int h) {
	glViewport(0,0,w,h);
}
