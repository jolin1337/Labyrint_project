
#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, char disp):x(x), y(y), disp(disp){}

int Obstacle::getX(){
    return x;
}
int Obstacle::getY(){
    return y;
}
char Obstacle::getDisp(){
    return disp;
}
