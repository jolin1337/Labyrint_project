
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Obstacle.h"
#include <map>

typedef std::map<std::string, std::map<int, Obstacle *> > Items;
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
    void addItem(std::string type, Obstacle *item);
    Obstacle *destroyItem(std::string type, int i=-1);

	Obstacle *findItem(std::string type, int i);
    
    bool near(int x, int y, int marginal=3);
    bool hasKey(int key);
    
private:
    Items items;
	int x,y;	// positions, a coordinate to describe the characters current position
};

#endif
