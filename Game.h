
#ifndef GAME_H
#define GAME_H
#include "Maze/Maze.h"
#include "Character.h"
#include "Obstacle.h"

#include <vector>

class Game {
public:
    static void start(int w, int h);
private:
	static const char WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT, PICKUP;
	static const char KEY, DOOR;
	Game(int w, int h);
	~Game();
	void init();
    void createObstacles();
	bool isPlaying() const;
    void checkEvents();
	void printMaze();
    
    std::vector<Obstacle *> obst;

protected:
	Game(){}
	bool playing;
	Maze maze, printing_maze;
	Character player;
};

#endif