#include "render.h"
#include <GL/glut.h>
#include <stdio.h>

Game Render::game; 
int Render::screen_width = 500, Render::screen_height=600; 

int Render::init(){

    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Callback functions
    glutReshapeFunc(Render::onReshape); // if the window resizes onReshape will be called
    glutDisplayFunc(Render::display);
    glutIdleFunc(Render::idle);
    glutSpecialFunc(Render::specialKeyEvent);
    glutKeyboardFunc(Render::keyEvent);

    //Render::game.start(10,10);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0,   // fov
					1.0f*screen_width/screen_height,    // aspect ratio
					0.5,   // z near
					200.0   // z far
	);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 3.0, 6.0,    // eye 
		0.0, 0.0, 0.0,          // center 
		0.0, 1.0, 0.0);         // up direction 
	return 1;					// successfully initialized game world
}
void Render::keyEvent(unsigned char  key, int x, int y){
	switch(key){
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}
void Render::specialKeyEvent( int key, int x, int y ) {
 
}

void Render::idle(){
	
	glutPostRedisplay();
}


void Render::display(){
	glClearColor(0,0,0,1);
	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		gluLookAt(0.0, 3.0, 6.0,    // eye 
			0.0, 0.0, 0.0,          // center 
			0.0, 1.0, 0.0);         // up direction 
		glColor3f(1.0f,1.0f,1.0f);
		// glBegin(GL_TRIANGLES);
		// glVertex3f(  1.0, 0.0, 0.0 );
		// glVertex3f(  0.0, 0.0, 1.0 );
		// glVertex3f(  0.0, 0.0, 0.0 );

		// glVertex3f(  1.0, 0.0, 0.0 );
		// glVertex3f(  0.0, 1.0, 0.0 );
		// glVertex3f(  0.0, 0.0, 0.0 );
		// glEnd();
		glutSolidCube(0.5f);
	glPopMatrix();
	glutSwapBuffers();

}


void Render::onReshape(int width, int height){

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0,   // fov
					1.0f*screen_width/screen_height,    // aspect ratio
					1.0,   // z near
					100.0   // z far
	);
	glMatrixMode(GL_MODELVIEW);
}
