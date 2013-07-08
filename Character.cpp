
#include "Character.h"

const char Character::TECKEN = 'T';

Character::Character():x(1),y(1) {

}

void Character::addItem(Obstacle *item){
    items.push_back(item);
}
Obstacle *Character::destroyItem(int i){
	if(i < 0 || i >= items.size()) return 0;
	Obstacle *destroying=items[i];
	items.erase(items.begin()+i);
	return destroying;
}

Character & Character::walkUp(){
	if(y>0)
		y--;
	return *this;
}
Character & Character::walkDown(){
	y++;
	return *this;
}
Character & Character::walkLeft(){
	if(x>0)
		x--;
	return *this;
}
Character & Character::walkRight(){
	x++;
	return *this;
}

