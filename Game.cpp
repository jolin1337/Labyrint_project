
#include "Game.h"
#include <iostream>
#ifndef _WIN32
#include "Linux/Linux.h"
#else
#include <conio.h>
#endif
const char Game::WALK_UP='w', Game::WALK_DOWN='s', Game::WALK_LEFT='a', Game::WALK_RIGHT='d', Game::PICKUP='?';
const char Game::KEY=':', Game::DOOR='o'; // detta blir ett ö tillsammans, tyckte jag var fyndigt :D
// Möjlig nyckel: fungerar inte ¥, 
//     			  fungerar inte ¶, 
// 				  fungerar u, 
// 				  fungerar :
// Möjlig dörr:   fungerar inte Ø, 
// 				  fungerar inte Þ, 
// 				  fungerar inte þ, 
// 				  fungerar n, 
// 				  fungerar o

Game::Game(int w, int h):playing(false) {
    maze.Create_maze(w,h);
}

Game::~Game(){
    for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
        delete *it;
}

void Game::init() {
    // sätter till ett spelbart läge
    playing = true;
    
    // initiera obstacles här:
    Obstacle *key = new Obstacle(4,8,KEY);  // TODO: generera x,y coordinater
    Obstacle *door = new Obstacle(4,1,DOOR);
    // osv
    obst.push_back(key); 
    obst.push_back(door);
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
	keyboard k;
	int c = k.getch();			// väntar på input från playern
	while(k.kbhit())		// for clearing the buffer
		k.getch();
	try{
		switch(c){
			case 27:				// escape-key
				playing = false;
				break;
			case WALK_UP:				// lower case
			case WALK_UP - ('a'-'A'):	// upper case
				if(maze[player.getY()-1][player.getX()] != Maze::WALL)
					player.walkUp();			// y--
				break;
			case WALK_DOWN:				// lower case
			case WALK_DOWN - ('a'-'A'):	// upper case
				if(maze[player.getY()+1][player.getX()] != Maze::WALL)
					player.walkDown();		// y++
				break;
			case WALK_RIGHT:				// lower case
			case WALK_RIGHT - ('a'-'A'):	// upper case
				if(maze[player.getY()][player.getX()+1] != Maze::WALL)
					player.walkRight();		// x++
				break;
			case WALK_LEFT:				// lower case
			case WALK_LEFT - ('a'-'A'):	// upper case
				if(maze[player.getY()][player.getX()-1] != Maze::WALL)
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
	}
	catch(...){}
}

void Game::printMaze(){
    if(!isPlaying()) return;
    // sets obsticles position
    std::vector<char> repls;
    for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++){
        repls.push_back(maze[(*it)->getY()][(*it)->getX()]);
        maze[(*it)->getY()][(*it)->getX()] = (*it)->getDisp();
    }
    
    // sets the player position
	char c = maze[player.getY()][player.getX()];
	maze[player.getY()][player.getX()] = Character::TECKEN;
	maze.Print_maze();

    // resets everything
    for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++){
        maze[(*it)->getY()][(*it)->getX()] = repls[0];
        repls.erase(repls.begin());
    }
	maze[player.getY()][player.getX()] = c;
}




