
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Obstacle.h"

class Character {
public:

    static const char TECKEN;
	Character();
	Character & walkUp();
	Character & walkDown();
	Character & walkLeft();
	Character & walkRight();

	int getX() const{return x;}
	int getY() const{return y;}
    void addItem(Obstacle *item);
    Obstacle *destroyItem(int i);
    
    bool near(int x, int y);
    
private:
    std::vector<Obstacle *> items;
	int x,y;	// positions, a coordinate to describe the characters current position
};

#endif
