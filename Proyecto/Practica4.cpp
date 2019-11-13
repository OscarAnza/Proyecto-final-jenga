#include <iostream>
#include <math.h>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

// Globals.
float refresh = 100.0;

int movX = 0;
int movY = 0;
int movZ = 0;

bool col = false;

class Pieza{
	private:
		float x = 0;
		float y = 0;
		float z = 0;
		float vertexs[8][3];
		float cX;
		float cY;
		float cZ;
	public:
		Pieza(float disX, float disY, float disZ){
			vertexs[0][0] = x;    vertexs[0][1] = y;    vertexs[0][2] = z;
			vertexs[1][0] = disX; vertexs[1][1] = y;    vertexs[1][2] = z;
			vertexs[2][0] = disX; vertexs[2][1] = disY; vertexs[2][2] = z;
			vertexs[3][0] = x;    vertexs[3][1] = disY; vertexs[3][2] = z;
			vertexs[4][0] = x;    vertexs[4][1] = y;    vertexs[4][2] = disZ;
			vertexs[5][0] = disX; vertexs[5][1] = y;    vertexs[5][2] = disZ;
			vertexs[6][0] = disX; vertexs[6][1] = disY; vertexs[6][2] = disZ;
			vertexs[7][0] = x;    vertexs[7][1] = disY; vertexs[7][2] = disZ;

			// Calcular centro de la pieza
			cX = (vertexs[0][0] + vertexs[1][0] / 2);
			cY = (vertexs[0][1] + vertexs[3][1] / 2);
			cZ = (vertexs[0][2] + vertexs[4][2] / 2);
		};
		void drawPrism(GLenum mode);
		float getCx();
		float getCy();
		float getCz();
		float getDistX();
		float getDistY();
		float getDistZ();
};

void Pieza::drawPrism(GLenum mode){
	glPolygonMode(GL_FRONT_AND_BACK, mode);

	// Front
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[0]);
	glVertex3fv(vertexs[1]);
	glVertex3fv(vertexs[2]);
	glVertex3fv(vertexs[3]);
	glEnd();

	// Base
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[0]);
	glVertex3fv(vertexs[1]);
	glVertex3fv(vertexs[5]);
	glVertex3fv(vertexs[4]);
	glEnd();

	// Top
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[3]);
	glVertex3fv(vertexs[2]);
	glVertex3fv(vertexs[6]);
	glVertex3fv(vertexs[7]);
	glEnd();

	// Back
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[4]);
	glVertex3fv(vertexs[5]);
	glVertex3fv(vertexs[6]);
	glVertex3fv(vertexs[7]);
	glEnd();

	// Left
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[0]);
	glVertex3fv(vertexs[4]);
	glVertex3fv(vertexs[7]);
	glVertex3fv(vertexs[3]);
	glEnd();

	// Right
	glBegin(GL_POLYGON);
	glVertex3fv(vertexs[1]);
	glVertex3fv(vertexs[5]);
	glVertex3fv(vertexs[6]);
	glVertex3fv(vertexs[2]);
	glEnd();
}

float Pieza::getCx(){
	return cX;
}

float Pieza::getCy(){
	return cY;
}

float Pieza::getCz(){
	return cZ;
}

float Pieza::getDistX(){
	return vertexs[6][0];
}

float Pieza::getDistY(){
	return vertexs[6][1];
}

float Pieza::getDistZ(){
	return vertexs[6][2];
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//glRotatef(45, 1.0, 0.0, 0.0);
	//glRotatef(10, 0.0, 0.0, 1.0);

	Pieza p1 = Pieza(20, 10, 10);
	Pieza p2 = Pieza(20, 10, 10);

	// Position of the base
	glTranslatef(movX, 50, 0);
	glColor3f(1.0, 0.0, 0.0);
	p1.drawPrism(GL_LINE);

	cout << (p1.getCx() + p1.getDistX() / 2) + movX << " " << (p2.getCx() - p2.getDistX() / 2) + (100 - movX) << endl;

	if(  - ((p2.getCx() - p2.getDistX() / 2) + (100 - movX)) == 0){
		cout << "colision" << endl;
		col = true;
	}

	glTranslatef(100 - movX * 2, -5.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	p2.drawPrism(GL_LINE);

	glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -100, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Timer(int value) {
	if(movX < 50 && !col) movX++;

	glutPostRedisplay();
	glutTimerFunc(refresh, Timer, 0);
}

void keyInput(unsigned char key, int x, int y)
{

}

void specialKeyInput(int key, int x, int y)
{

}


void printInteraction(){

}

// Main routine.
int main(int argc, char **argv) 
{
	glutInit(&argc, argv);

	glutInitContextVersion(2, 1); 
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Robotic Arm");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutTimerFunc(0, Timer, 0);
	glewInit(); 
	printInteraction();

	setup(); 

	glutMainLoop();   	
}
