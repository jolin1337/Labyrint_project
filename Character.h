
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Obstacle.h"
#include <map>
#include <exception>

typedef std::map<std::string, std::map<int, Obstacle *> > Items;
class Character {
public:
	struct CharacterException : public std::exception{
		const char* what() const throw() {
			return "addItem() error, item already exists.";
		}
	};
    static const char TECKEN;
	static const float stepSize;
    Character();
	Character & walkUp();
	Character & walkDown();
	Character & walkLeft();
	Character & walkRight();

	float getX() const{return x;}
	float getY() const{return y;}
    void addItem(std::string type, Obstacle *item);
    Obstacle *findItem(std::string type, int i);
    Obstacle *destroyItem(std::string type, int i=-1);
    
    bool isNear(float px, float py, float marginal=3);
    
private:
    Items items;
	float x,y;	// positions, a coordinate to describe the characters current position
};

#endif







