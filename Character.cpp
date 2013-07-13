
#include "Character.h"

const char Character::TECKEN = 'T';

Character::Character():x(1),y(1) {

}
//typedef std::map<std::string, std::map<int, Obstacles *> > Items;
void Character::addItem(std::string type, Obstacle *item){
    if(type == "key"){
        Key *k = static_cast<Key*>(item);
        if(items[type].find(k->id) == items[type].end())
            items[type][static_cast<Key*>(item)->id] = item;
        //    items[type].insert(std::pair<int, Obstacle*>(static_cast<Key*>(item)->id, item));
        else 
            throw CharacterException();
    }
    else
        items[type][items[type].size()] = item;
    // items[type].insert(std::pair<int, Obstacle*>(items.count(), item));
}


Obstacle *Character::findItem(std::string type, int i){
    if (items[type].find(i) != items[type].end()){
        return items[type][i];
    }
    else 
        return 0;
}

Obstacle *Character::destroyItem(std::string type, int i){
    if(i == -1){
        if(items.find(type) != items.end() && items[type].begin() != items[type].end()){
            Obstacle *destroying = (items[type].begin())->second;
            items[type].erase(items[type].begin());
            return destroying;
        }
        else if(items.find(type) != items.end())
            items.erase(type);
        return 0;
    }
    if(items[type].find(i) == items[type].end())
        return 0;
    Obstacle *destroying=items[type][i];
	items[type].erase(i);
    if(items[type].size() == 0)
        items.erase(type);
	return destroying;
}
  

bool Character::isNear(int px, int py, int marginal){
	if((x-px)*(x-px) + (y-py)*(y-py) <= marginal*marginal)
	// if(this->x > x - marginal && this->x < x + marginal &&
	// 	this->y > y - marginal && this->y < y + marginal)
		return true;
    return false;
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

