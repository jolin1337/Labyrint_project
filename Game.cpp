
#include "Game.h"
#include <iostream>
#ifndef _WIN32
#include "Linux/Linux.h"
#else
#include <conio.h>
#endif
// const char Game::WALK_UP='w', Game::WALK_DOWN='s', Game::WALK_LEFT='a', Game::WALK_RIGHT='d', Game::ACTION_BTN=VK_SPACEBAR;
// const char Game::KEY=':', Game::DOOR='o'; // detta blir ett ö tillsammans, tyckte jag var fyndigt :D
// Möjlig nyckel: fungerar u, 
//                fungerar F
// Möjlig dörr:   fungerar E, 
//                fungerar n,
//                   fungerar o

Game::Game(int w, int h):playing(false) {
    maze.Create_maze(w,h);
	printing_maze = maze;
	for(int i=printing_maze.Get_height()-1; i > 1; i--)
		for(int j=printing_maze.Get_width()-1; j > 1; j--)
			printing_maze[i-1][j-1] = ' ';
	
}

Game::~Game(){
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		delete *it;
	Obstacle *ob;
	while((ob = player.destroyItem("key")) != 0)
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
	std::string all="";
	all+=c;
	while(k.kbhit())		// for clearing the buffer
		all += k.getch();
#else
	int c = getch();    		// väntar på input från playern
	std::string all="";
	all+=c;
	while(kbhit())		// for clearing the buffer
		all += getch();
#endif
	try{
		bool oneChar = all.size() == 1;
		char pc;                        // position character (pc)
		if(oneChar && c == VK_ESCAPE)
			playing = false;
		else if(all == VK_UP || (oneChar && (c == WALK_UP || c == WALK_UP - ('a'-'A')))){
			pc = maze[player.getY()-1][player.getX()];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkUp();			// y--
		}
		else if(all == VK_DOWN || (oneChar && (c == WALK_DOWN || c == WALK_DOWN - ('a'-'A')))){
			pc = maze[player.getY()+1][player.getX()];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkDown();		// y++
		}
		else if(all == VK_RIGHT || (oneChar && (c == WALK_RIGHT || c == WALK_RIGHT - ('a'-'A')))){
			pc = maze[player.getY()][player.getX()+1];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkRight();		// x++
		}
		else if(all == VK_LEFT || (oneChar && (c == WALK_LEFT || c == WALK_LEFT - ('a'-'A')))){
			pc = maze[player.getY()][player.getX()-1];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkLeft();			// x--
		}
            // Door opener
		else if(oneChar && c == ACTION_BTN){ 
    		for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
                if(player.near((*it)->getX(), (*it)->getY(), 1) && player.findItem("key", static_cast<Key*>(*it)->id) != 0){
                    obst.erase(it);
					 maze[(*it)->getY()][(*it)->getX()] = Maze::PATH;
                    break;
                }
		}

			// ....
		if(maze[player.getY()][player.getX()] == Maze::END){ // If goal reached
			std::cout << "You've reached the exit!" << std::endl << "Congratulations!" << std::endl << std::endl;
			playing = false;
		}
		if(maze[player.getY()][player.getX()] == Game::KEY){ // If key found
			for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
				if(player.getY() == (*it)->getY() && player.getX() == (*it)->getX()){ // If the key is on players location
					 player.addItem("key", *it);
					 obst.erase(it);
					 maze[player.getY()][player.getX()] = Maze::PATH;
					 break;
				}
		}
	}
	catch(...){}
}


void Game::printMaze(){
	for(int i=printing_maze.Get_height(); i > 0; i--)
		for(int j=printing_maze.Get_width(); j > 0; j--)
			if(player.near(j-1,i-1))
				printing_maze[i-1][j-1] = maze[i-1][j-1];
			// else
			//     printing_maze[i-1][j-1] = Maze::PATH;
	if(!isPlaying()) return;
	// sets obstacles position
	Maze m = printing_maze;
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		if(player.near((*it)->getX(), (*it)->getY()))
			m[(*it)->getY()][(*it)->getX()] = (*it)->getDisp();
	
	// sets the player position
	m[player.getY()][player.getX()] = Character::TECKEN;
	m.Print_maze();
}

