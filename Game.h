
#ifndef GAME_H
#define GAME_H
#include "Maze/Maze.h"
#include "Character.h"
#include "Obstacle.h"
#include "utils/GLobject.h"

#include <vector>
#include <exception>

class Game {
public:
	struct GameException : public std::exception {
		GameException(const char *msg):msg(msg){}
		const char* what() const throw() {
			return "dsads";//msg.c_str();
		}
	 	const char *msg;
	};
	void start(int w, int h=-1);
	int init();
    void createObstacles();
	bool isPlaying() const;
    void checkEvents(unsigned char key);
	void idle();
	void printMaze();
	Game(int w=-1, int h=-1);
	~Game();
private:
    enum Inputs{
		WALK_UP='w', WALK_DOWN='s', WALK_LEFT='a', WALK_RIGHT='d', ACTION_BTN=' ',
		KEY=':', DOOR='o'
	};
	// static const char WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT, ACTION_BTN;
	// static const char KEY, DOOR;

    
    std::vector<Obstacle *> obst;

protected:
	bool playing;
	Maze maze, printing_maze;
	GLobject primitive_wall;
	Character player;
	static const char *vertexShaderFileName, *fragmentShaderFileName;
};

#endif