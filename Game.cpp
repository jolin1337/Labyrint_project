
#include "Game.h"
#include <iostream>
#ifndef _WIN32
#include "Linux/Linux.h"
#else
#include <conio.h>
#endif
const char Game::WALK_UP='w', Game::WALK_DOWN='s', Game::WALK_LEFT='a', Game::WALK_RIGHT='d', Game::PICKUP='?';
const char Game::KEY=':', Game::DOOR='o'; // detta blir ett ö tillsammans, tyckte jag var fyndigt :D
// Möjlig nyckel: fungerar u, 
//                fungerar F
// Möjlig dörr:   fungerar E, 
//                fungerar n,
//     			  fungerar o

Game::Game(int w, int h):playing(false) {
	maze.Create_maze(w,h);
}

Game::~Game(){
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		delete *it;
	Obstacle *ob;
	while((ob = player.destroyItem(0)) != 0)
		delete ob;
}

void Game::init() {
	// sätter till ett spelbart läge
	playing = true;
	createObstacles();
}
void Game::createObstacles() {
	if(obst.size())
		return;
	// initiera obstacles här:
	Maze solved = maze;
	Obstacle *door = new Door(-1,-1, DOOR), *key = new Key(-1,-1, KEY);
	Obstacle::generateObstacle(solved, door, key);
	
		std::cout << door->getX() << "," << door->getY() << "\n";
	if(door->getX() > -1 && door->getY() > -1 && key->getX() > -1 && key->getY() > -1){
		((Door*)door)->id = ((Key*)key)->id = obst.size()+1;
		obst.push_back(door);
		obst.push_back(key);
		maze[door->getY()][door->getX()] = DOOR;
		maze[key->getY()][key->getX()] = KEY;
	}
}

bool Game::isPlaying() const {
	return playing;
}

void Game::start(int w, int h){
	Game main_game(w,h);
	// Här gör vi en init
	main_game.init();
	while(main_game.isPlaying()){
		main_game.printMaze();			// skriv ut den färdigrenderade mazen med items props och karaktärer
		main_game.checkEvents();		// skapa en event funktion(kanske ett objekt event inuti klassen beroende på hur komplext det blir)
	}
}

void Game::checkEvents(){
	if(!isPlaying()) return;
#ifndef _WIN32
	keyboard k;
	int c = k.getch();			// väntar på input från playern
	while(k.kbhit())		// for clearing the buffer
		k.getch();
#else
	int c = getch();    		// väntar på input från playern
	while(kbhit())		// for clearing the buffer
		getch();
#endif
	try{
		char pc;                        // position character (pc)
		switch(c){
			case 27:				// escape-key
				playing = false;
				break;
			case WALK_UP:				// lower case
			case WALK_UP - ('a'-'A'):	// upper case
				pc = maze[player.getY()-1][player.getX()];
				if(pc != Maze::WALL && pc != DOOR)
					player.walkUp();			// y--
				break;
				
			case WALK_DOWN:				// lower case
			case WALK_DOWN - ('a'-'A'):	// upper case
				pc = maze[player.getY()+1][player.getX()];
				if(pc != Maze::WALL && pc != DOOR)
					player.walkDown();		// y++
				break;
				
			case WALK_RIGHT:				// lower case
			case WALK_RIGHT - ('a'-'A'):	// upper case
				pc = maze[player.getY()][player.getX()+1];
				if(pc != Maze::WALL && pc != DOOR)
					player.walkRight();		// x++
				break;
				
			case WALK_LEFT:				// lower case
			case WALK_LEFT - ('a'-'A'):	// upper case
				pc = maze[player.getY()][player.getX()-1];
				if(pc != Maze::WALL && pc != DOOR)
					player.walkLeft();			// x--
				break;
			// ....
			default:
				//....
				break;
		}
		if(maze[player.getY()][player.getX()] == Maze::END){ // If goal reached
			std::cout << "You've reached the exit!" << std::endl << "Congratulations!" << std::endl << std::endl;
			playing = false;
		}
		if(maze[player.getY()][player.getX()] == Game::KEY){ // If key found
			for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
				if(player.getY() == (*it)->getY() && player.getX() == (*it)->getX()){ // If the key is on players location
					 player.addItem(*it);
					 obst.erase(it);
					 maze[player.getY()][player.getX()] = Maze::PATH;
					 break;
				}
		}
	}
	catch(...){}
}


void Game::printMaze(){
	if(!isPlaying()) return;
	// sets obstacles position
	Maze m = maze;
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++){
		m[(*it)->getY()][(*it)->getX()] = (*it)->getDisp();
	}
	
	// sets the player position
	m[player.getY()][player.getX()] = Character::TECKEN;
	m.Print_maze();

}

