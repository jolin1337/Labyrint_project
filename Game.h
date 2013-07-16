
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
	// an exception class for efficent track of the error gaind by Game
	struct GameException : public std::exception {
		GameException(const char *msg):msg(msg){}
		const char* what() const throw() {
			return msg;
		}
		const char *msg;	// the error-message 
	};
	/**
	 * starts the game and calls init
	 * @param w width of the labyrinth/Maze
	 * @param h height of the labyrinth/Maze
	 */
	void start(int w, int h=-1);
	/**
	 * inits the resources
	 * @return reutrns 1 if success else 0 
	 */
	int init();
	/**
	 * creates doors and keys if possible
	 */
	void createObstacles();
	/**
	 * indicates if the player is playing the game
	 * @return true if playing else false
	 */
	bool isPlaying() const;

				// oneChar will indicate if it is a special key or not
	/**
	 * updates the keys for idle function
	 * @param key     the key that was pressed
	 * @param oneChar indicates if it is a special key ie not a character(or ascii value)
	 */
	void checkEvents(unsigned char key, bool oneChar=true);
	/**
	 * updates the games animations and players movements
	 */
	void idle();
	/**
	 * renders the maze with focus on the player
	 */
	void printMaze();
	/**
	 * this constructor tries to call start(int, int) if w >3
	 */
	Game(int w=-1, int h=-1);
	/** 
	 * removes all resources
	 */
	~Game();
private:
	/**
	 * All the symbols and keyboard-tangents characters
	 */
	enum Inputs{
		WALK_UP='w', WALK_DOWN='s', WALK_LEFT='a', WALK_RIGHT='d', ACTION_BTN=' ',
		KEY=':', DOOR='o'
	};

	
	std::vector<Obstacle *> obst;		// contains all obstacles in the labyrinth

protected:
	bool playing;						// indicates if player plays the game
	Maze maze, printing_maze;			// the maze data
	GLobject primitiv_wall, primitiv_key;// models data
	Character player;					// the player data
	static const char *vertexShaderFileName, *fragmentShaderFileName; // vertex and fragment shader-filenames
};

#endif