#include "Game.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

#include "Environment.h"


int init_resources(){
	Environment::game.start(30,30);

	return Environment::game.isPlaying();
}

void onReshape(int width, int height) {
	Environment::screen_width = width;
	Environment::screen_height = height;
	glViewport(0, 0, Environment::screen_width, Environment::screen_height);
	// glutWarpPointer(Environment::screen_width/2, Environment::screen_height/2);
}

void idle(){
	// glutWarpPointer(Environment::screen_width/2, Environment::screen_height/2);
	if(Environment::game.isPlaying())
		Environment::game.idle();
	else
		exit(0);
}

void onDisplay() {
	// glClearColor(1.0, 1.0, 1.0, 1.0);
	// Clear the background as black 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	Environment::game.printMaze();
	// Light:
	// GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	// glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Display the result 
	glutSwapBuffers();
}

void onMouseMove(int x, int y){
	float speed = 0.02;
}

void onKeyDown(unsigned char Key, int x, int y){
	switch(Key){
		case 27:
			exit(0);
			break;
		case 'f':
			if(Environment::isFullscreen && Environment::screen_height > 0 && Environment::screen_width > 0){
				Environment::isFullscreen = false;
				glutPositionWindow(0,0);
			}
			else{
				glutFullScreen();
				Environment::isFullscreen = true;
			}
			break;
		default:
			Environment::game.checkEvents(Key);
	}
	
}
void onKeyUp(unsigned char Key, int x, int y){
}
void onKeyDownSpecial(int Key, int x, int y){
	// keys[Key] = true;
	Environment::game.checkEvents(Key, false);
}
void onKeyUpSpecial(int Key, int x, int y){
	// keys[Key] = false;
}
