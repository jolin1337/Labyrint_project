#ifndef RENDERH
#define RENDERH
#include "Game.h"

class Render{
public:
	static int init();
    static void display();
    static void idle();
    static void keyEvent(unsigned char key, int x, int y);
    static void specialKeyEvent(int key, int x, int y);
    static void onReshape(int width, int height);
    static Game game;
    static int screen_width, screen_height;
};
#endif
