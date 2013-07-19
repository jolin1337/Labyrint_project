
#ifndef GAME_H
#define GAME_H
#include "Maze/Maze.h"
#include "Character.h"
#include "Obstacle.h"

#include <vector>

#if defined _WIN32 || defined __CYGWIN__
#include <conio.h>
#include <Windows.h>
#undef VK_UP
#undef VK_LEFT
#undef VK_DOWN
#undef VK_RIGHT

#define VK_UP "\033[A"    // W-key
#define VK_LEFT "\033[D"  // A-key
#define VK_DOWN "\033[B"  // S-key
#define VK_RIGHT "\033[C" // D-key
#else
#include "Linux/Linux.h"
#endif

class Game {
public:
    Game(){}
    void start(int w, int h);
    enum Inputs{
    	WALK_UP='w', WALK_DOWN='s', WALK_LEFT='a', WALK_RIGHT='d', ACTION_BTN=VK_SPACE,
		KEY=':', DOOR='o'
	};
	// static const char WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT, ACTION_BTN;
	// static const char KEY, DOOR;
	Game(int w, int h);
	~Game();
    void createObstacles();
	bool isPlaying() const;
    void checkEvents();
	void printMaze();
private:

    void init();
	bool playing;
	Maze maze, printing_maze;
	Character player;
    std::vector<Obstacle *> obst;
};


#endif











