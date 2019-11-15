#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

#define MAX_PIEZAS 30

// Globals.
float getDistance(float x, float y, float x2, float y2);
float refresh = 100.0;
static float W,H;
static int clics = 0;
int movX = 0;
int movY = 0;
int movZ = 0;
float ex=-15.0;
float ey=24.0;
float ez=22.0;
float cx=0.0;
float cy=10.0;
float cz=0.0;
float ux=0.0;
float uy=1.0;
float uz=0.0;
int cont=0;
int cont2=0;
static GLUquadricObj *qobj; // Pointer to GLU quadric object.

bool inicio = true;
bool col = false;

void piso(){
	int i;
   	float z;

   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	   glLoadIdentity();
	   //gluLookAt(0.0, 5.0, 30.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	   gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
	   // Draw floor as a stack of triangle strips.
	   i = 0;
	   for(z = 100.0; z > -100.0; z -= 5.0)
	   {
	      
	      glBegin(GL_TRIANGLE_STRIP);
	      for(float x = -100.0; x < 100.0; x += 5.0)
		  {
	         if (i % 2) glColor4f(0.0, 0.5, 0.5, 1.0); 
	         else glColor4f(1.0, 1.0, 1.0, 1.0); 
	         glNormal3f(0.0, 1.0, 0.0);
			 glVertex3f(x, 0.0, z - 5.0);
		     glVertex3f(x, 0.0, z);
			 i++;
		  }
	      glEnd();
		  i++;
	   }

	   //glFlush();	
}

class Pieza{
	private:
		float x = 0;
		float y = 0;
		float z = 0;
		float vertexs[8][3];
		float cX;
		float cY;
		float cZ;
		float auxX=0;
		float auxY=0;
	public:
		Pieza(){};
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
			auxY=disY;
			auxX=disX;
		};
		void drawPrism(GLenum mode);
		float getCx();
		float getCy();
		float getCz();
		float getDistX();
		float getDistY();
		float getDistZ();
		//No se utiliza
		bool printCoordenates(float xval, float yval);
};

vector<Pieza> vPieza;
vector<Pieza>::iterator iPieza; 

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
//No se utiliza yeyeye
bool Pieza::printCoordenates(float xval, float yval){
    if (getDistance(auxX, auxY, xval, yval) < 7){
        return true;
    }
}
//No se utiliza
bool updatePoint(float x, float y){
    iPieza = vPieza.begin();
   while(iPieza != vPieza.end() ){
   		if (iPieza->printCoordenates(x, y)){
           return true;
        }
      iPieza++;
   }
}

float getDistance(float x, float y, float x2, float y2){
    return sqrt(pow(x2-x, 2)+pow(y2-y, 2));
}

Pieza piezas[MAX_PIEZAS];

void armarTorre(){
	float posX = 1.5;
	float posY = 0;
	float posZ = 1.5;
	int piezasPorLinea = 0;
	bool subir = false;
	bool g1 = true;

	glTranslatef(-3.5, -5.0, -4.0);
	//glRotatef(90, 0.0, 1.0, 0.0);

	for(int i = 0; i < 9; i++){
		if(g1)
			glTranslatef(0.0, 0.0, posZ);
		else
			glTranslatef(0.0, 0.0, -posZ);

		glColor3f(1.0, 0.0, 0.0);
		piezas[i].drawPrism(GL_FILL);
		glColor3f(0.0, 0.0, 0.0);		
		piezas[i].drawPrism(GL_LINE);
		
		piezasPorLinea++;

		if(piezasPorLinea == 3){
			posY = 1.5;
			piezasPorLinea = 0;
			subir = true;
			
			if(g1){
				//glRotatef(-90, 0.0, 1.0, 0.0);
				glTranslatef(-3.0, posY, 0.0);
				g1 = false;
			} else{
				//glRotatef(90, 0.0, 1.0, 0.0);
				glTranslatef(-4.5, posY, 4.0);
				g1 = true;
			}
			
		}	
	}
}
void crearPiezas(){
	cont=0;
	cont2=0;
	glTranslatef(-4, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(int i = 0; i < MAX_PIEZAS; i++){
		if(cont<3){
			piezas[i] = Pieza(9, 3, 3);
			cont++;
			//cout<<"Entre 1"<<endl;
		}else{
			piezas[i] = Pieza(3, 3, 9);
			cont2++;
			//cout<<"Entre 2"<<endl;
		}
		if(cont2==3){
			cont=0;
			cont2=0;
		}
	}
}
void armarTorre2(){
	//float posX=0;
	float posY=0;
	float posZ=3;
	int filas=0;
	float posXarray[3]={0.0,3.0,6.0};
	//float posYarray[3]={3.0,6.0,9.0};
	float posZarray[3]={3.0,6.0,9.0};
	cont=0;
	cont2=0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int i = 0; i < MAX_PIEZAS; i++){

		if(filas==3){
			filas=0;
		}
		//Switch para hacer cambios entre la 'Rotación' de la figura
		if(cont<3){
			glPushMatrix();
			glTranslatef(0.0, posY, posZarray[filas]);
			glColor3f(1.0, 0.0, 0.0);
			piezas[i].drawPrism(GL_FILL);
			glColor3f(0.0, 0.0, 0.0);		
			piezas[i].drawPrism(GL_LINE);
			glPopMatrix();
			cont++;
			if(cont==3)posY+=3;
		}else{
			glPushMatrix();
			glTranslatef(posXarray[filas], posY, posZ);
			glColor3f(0.0, 1.0, 0.0);
			piezas[i].drawPrism(GL_FILL);
			glColor3f(0.0, 0.0, 0.0);		
			piezas[i].drawPrism(GL_LINE);
			glPopMatrix();
			cont2++;
			if(cont2==3)posY+=3;
			//cout<<"posY: "<<posY<<endl;
		}
		//Reiniciar el switch
		if(cont2==3){
			cont=0;
			cont2=0;
		}
		filas++;
	}
}
//bool disparador=false;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
	gluQuadricDrawStyle(qobj, GLU_FILL); 
	piso();
	crearPiezas();
	armarTorre2();
	//cout << (p1.getCx() + p1.getDistX() / 2) + movX << " " << (p2.getCx() - p2.getDistX() / 2) + (100 - movX) << endl;

	/*if(((p1.getCx() + p1.getDistX() / 2) + movX) - ((p2.getCx() - p2.getDistX() / 2) + (100 - movX)) == 0){
		cout << "colision" << endl;
		col = true;
	}*/

	glutSwapBuffers();
}
void mouseControl(int button, int state, int x, int y){
	
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
      y = H - y; // Correct from mouse to OpenGL co-ordinates.
	  // Click outside canvas - do nothing.
      if ( x < 0 || x > W || y < 0 || y > H ) ;

      cout<<"X: "<<x<<endl;
      cout<<"Y: "<<y<<endl;

      
  }
}
// Initialization routine.
void setup(void) 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.
   // Initialize GLU quadric object.
   	qobj = gluNewQuadric();
   	gluQuadricDrawStyle(qobj, GLU_LINE);  
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);    

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = {0.0, 1.0, 0.0, 0.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
   
   // Flat shading to obtain the checkered pattern of the floor.
   glShadeModel(GL_FLAT);

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	W = w;
   	H = h;
	//glOrtho(0, 100, 0, 100, -100, 100);
	glFrustum(-5.0, 5.0, -5.0, 5.0, 3.0, 100.0);
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
	switch(key)
   {
     	case 27:
        	exit(0);
        break;
      	case 'a':
      		ex--;
      		cout<<"ex: "<<ex<<endl;
        break;
        case 'd':
      		ex++;
      		cout<<"ex: "<<ex<<endl;
        break;

        case 'q':
        	ey++;
        	cout<<"ey: "<<ey<<endl;
        break;
        case 'e':
        	ey--;
        	cout<<"ey: "<<ey<<endl;
        break;

        case 'w':
        	ez--;
        	cout<<"ez: "<<ez<<endl;
        break;
        case 's':
        	ez++;
        	cout<<"ez: "<<ez<<endl;
        break;
    }
    glutPostRedisplay();

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

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Jenga");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	//glutTimerFunc(0, Timer, 0);
	glewInit(); 
	printInteraction();
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl); 
	setup(); 

	glutMainLoop();   	
}
