
#ifndef OBSTACLE_H
#define OBSTACLE_H

//  beskriver någon form av hinder i labyrinten
class Obstacle { // Var deklarerar vi tecknet/positionen sen? i game init() funktionen!
public:
    Obstacle(int x=0, int y=0, char disp=' ');
    int getX();
    int getY();
    char getDisp();
    
private:
    char disp;  // det tecknet som representerar det vissuella av detta objekt | ahh så de kan vara olika för olika objekt.
    int x, y;   // positionen av detta objekt
};

#endif