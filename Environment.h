#ifndef ENV_H
#define ENV_H

#include "Game.h"

struct Environment {
	static Game game;
	static GLint screen_width, screen_height;
	static bool isFullscreen;
};

#endif