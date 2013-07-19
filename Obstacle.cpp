
#include "Obstacle.h"
#include <stdlib.h>

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

void Obstacle::generateObstacle(Maze solved, Obstacle *door, Obstacle *key){
    if(!key||!door || solved.Get_width() < 3 || solved.Get_height() < 3)
        return;
    // initiera obstacles här:
    solved.Solve();
    try{
        int posI = rand()%solved.getPathLength();            // posI är avståndet från slutet till den ända dörren
        int x = solved.Get_width()-2,y=solved.Get_height()-2;// end-positionen
        int prevDir = -1;                                   // denna finns för att förhindra att loopen går mot slutet istället för mot start
        // generate door
        while(posI || 
                !( ((solved[y+1][x] == Maze::WALL && solved[y-1][x] == Maze::WALL)
                   || (solved[y][x+1] == Maze::WALL && solved[y][x-1] == Maze::WALL))) ){   // stega igenom varje position tills vi hittat positionen med steglängd posI
            if(posI)
                posI--;
            if(solved[y][x+1] == Maze::SOLVEDPATH && prevDir != 0) {         // om lösningen går åt vänster
                prevDir = 1;
                x++;                                                         // gå till höger
            }
            else if(solved[y][x-1] == Maze::SOLVEDPATH && prevDir != 1) {    // om lösningen går åt höger
                prevDir = 0;
                x--;                                                         // gå till vänster
            }
            else if(solved[y+1][x] == Maze::SOLVEDPATH && prevDir != 2) {    // om lösningen går neråt
                prevDir = 3;
                y++;                                                         // gå ner
            }
            else if(solved[y-1][x] == Maze::SOLVEDPATH && prevDir != 3) {    // om lösningen går uppåt
                prevDir = 2;
                y--;                                                         // gå upp
            }
            else return;                                                     // annars är vi i startpositionen ...
        }
        // generate key
        int xk = x, yk = y;
        while(1){
            if(solved[yk][xk+1] != Maze::SOLVEDPATH && solved[yk][xk+1] != Maze::WALL && prevDir != 0){
                prevDir = 1;
                xk++;
            }
            else if(solved[yk][xk-1] != Maze::SOLVEDPATH && solved[yk][xk-1] != Maze::WALL && prevDir != 1){
                prevDir = 0;
                xk--;
            }
            else if(solved[yk+1][xk] != Maze::SOLVEDPATH && solved[yk+1][xk] != Maze::WALL && prevDir != 2){
                prevDir = 3;
                yk++;
            }
            else if(solved[yk-1][xk] != Maze::SOLVEDPATH && solved[yk-1][xk] != Maze::WALL && prevDir != 3){
                prevDir = 2;
                yk--;
            }
            else{
                if( solved[yk][xk+1] != Maze::SOLVEDPATH && 
                    solved[yk][xk-1] != Maze::SOLVEDPATH && 
                    solved[yk+1][xk] != Maze::SOLVEDPATH && 
                    solved[yk-1][xk] != Maze::SOLVEDPATH)
                        break;
                if(solved[yk][xk+1] == Maze::SOLVEDPATH && prevDir != 0) {         // om lösningen går åt vänster
                    prevDir = 1;
                    xk++;                                                         // gå till höger
                }
                else if(solved[yk][xk-1] == Maze::SOLVEDPATH && prevDir != 1) {    // om lösningen går åt höger
                    prevDir = 0;
                    xk--;                                                         // gå till vänster
                }
                else if(solved[yk+1][xk] == Maze::SOLVEDPATH && prevDir != 2) {    // om lösningen går neråt
                    prevDir = 3;
                    yk++;                                                         // gå ner
                }
                else if(solved[yk-1][xk] == Maze::SOLVEDPATH && prevDir != 3) {    // om lösningen går uppåt
                    prevDir = 2;
                    yk--;                                                         // gå upp
                }
                else return;                                                     // annars är vi i startpositionen ...
            }
        }
        key->x = xk;
        key->y = yk;
        door->x = x;
        door->y = y;
    }catch(...){
        key->x = -1;
        key->y = -1;
        door->x = -1;
        door->y = -1;
    }
}








