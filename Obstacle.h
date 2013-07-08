
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Maze/Maze.h"
#include "Linux/Linux.h"

//  beskriver någon form av hinder i labyrinten
class Obstacle { // Var deklarerar vi tecknet/positionen sen? i game init() funktionen!
public:
    Obstacle(int x=0, int y=0, char disp=' ');
    static void generateObstacle(Maze solved, Obstacle *door, Obstacle *key);
    int getX();
    int getY();
    char getDisp();
    
protected:
    char disp;  // det tecknet som representerar det vissuella av detta objekt | ahh så de kan vara olika för olika objekt.
    int x, y;   // positionen av detta objekt
};

struct Key : public Obstacle {
    Key(int x=0, int y=0, char disp=' '):id(-1), Obstacle(x,y,disp){}
    int id;
};
struct Door : public Obstacle {
    Door(int x=0, int y=0, char disp=' '):id(-1), Obstacle(x,y,disp){}
    int id;
};
    
#endif
    
    