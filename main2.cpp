#include <stdio.h>
#include <stdarg.h>
#include <math.h>
// #define GL_GLEXT_PROTOTYPES
// #ifdef __APPLE__
// #include <GLUT/glut.h>
// #else
// #include <GL/freeglut.h>
// #include <GL/glut.h>
// #endif
#include <GL/glew.h>
#include <GL/glut.h>
#include "render.h"

 
int main(int argc, char* argv[]){

    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    // Create window
    glutInitWindowSize(Render::screen_width, Render::screen_height); // sets the default window size
    glutCreateWindow("Maze Game Johannes and Marcus");               // create a window with a title

    if(Render::init() == 1){
        //  Pass control to GLUT for events
        glutMainLoop();
    }

    //  Return to OS
    return 0;

}
