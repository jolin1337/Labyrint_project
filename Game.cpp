
#include "Game.h"
#include <iostream>
#include "setup.h"


const char 	*Game::vertexShaderFileName = "shaders/wall.v-shader", 
			*Game::fragmentShaderFileName = "shaders/wall.f-shader";
	// const char Game::WALK_UP='w', Game::WALK_DOWN='s', Game::WALK_LEFT='a', Game::WALK_RIGHT='d', Game::ACTION_BTN=VK_SPACEBAR;
	// const char Game::KEY=':', Game::DOOR='o'; // detta blir ett ö tillsammans, tyckte jag var fyndigt :D
	// Möjlig nyckel: fungerar u, 
	//                fungerar F
	// Möjlig dörr:   fungerar E, 
	//                fungerar n,
	//                   fungerar o

Game::Game(int w, int h):playing(false) {
	if( w < 3) return;
	start(w,h);
}
void Game::start(int w, int h){
	if( w < 3) throw GameException("Small width");
	if(h < 3)
		h = w;

    maze.Create_maze(w,h);
	printing_maze = maze;
	for(int i=printing_maze.Get_height()-1; i > 1; i--)
		for(int j=printing_maze.Get_width()-1; j > 1; j--)
			printing_maze[i-1][j-1] = ' ';
	init();
}

Game::~Game(){
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		delete *it;
	Obstacle *ob;
	while((ob = player.destroyItem("key")) != 0)
		delete ob;
}

int Game::init() {
	// sätter till ett spelbart läge
	playing = true;


	try{
		primitive_wall.loadObj("models/wall.obj");
		// primitive_wall.setScale(Vector3(0.5,0.5,0.5));

		setupBuffers(primitive_wall);
		Uniform u("m_transform");
		Attribute a1("position"),
				  // a2("texcoord"), 
				  a3("color"),
				  a4("normal");
		primitive_wall.addUniform(u);
		primitive_wall.addAttribute(a1);
		// primitive_wall.addAttribute(a2);
		primitive_wall.addAttribute(a3);
		primitive_wall.addAttribute(a4);
		return primitive_wall.loadShaders(vertexShaderFileName, fragmentShaderFileName);
	}catch(int i){
		std::cout << "Failed to start the game" << std::endl;
		playing = false;
		return 0;
	}

	createObstacles();
	return 1;
}
void Game::createObstacles() {
	if(obst.size())
		return;
	// initiera obstacles här:
	Maze solved = maze;
	Obstacle *door = new Door(-1,-1, DOOR), *key = new Key(-1,-1, KEY);
	Obstacle::generateObstacle(solved, door, key);
	
	if(door->getX() > -1 && door->getY() > -1 && key->getX() > -1 && key->getY() > -1){
		((Door*)door)->id = ((Key*)key)->id = obst.size()+1;
		obst.push_back(door);
		obst.push_back(key);
		maze[door->getY()][door->getX()] = DOOR;
		maze[key->getY()][key->getX()] = KEY;
	}
}

bool Game::isPlaying() const {
	return playing;
}

void Game::checkEvents(unsigned char key){
	unsigned char &c = key;
	/*if(!isPlaying()) return;
#ifndef _WIN32
	keyboard k;
	int c = k.getch();			// väntar på input från playern
	std::string all="";
	all+=c;
	while(k.kbhit())		// for clearing the buffer
		all += k.getch();
#else
	int c = _getch();    		// väntar på input från playern
	std::string all="";
	all+=c;
	while(kbhit())		// for clearing the buffer
		all += getch();
#endif*/
	try{
		bool oneChar = true;
		char pc;                        // position character (pc)
		if(oneChar && c == 27)
			playing = false;
		else if(oneChar && (c == WALK_UP || c == WALK_UP - ('a'-'A'))){
			pc = maze[player.getY()-1][player.getX()];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkUp();			// y--
		}
		else if(oneChar && (c == WALK_DOWN || c == WALK_DOWN - ('a'-'A'))){
			pc = maze[player.getY()+1][player.getX()];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkDown();		// y++
		}
		else if(oneChar && (c == WALK_RIGHT || c == WALK_RIGHT - ('a'-'A'))) {
			pc = maze[player.getY()][player.getX()+1];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkRight();		// x++
		}
		else if(oneChar && (c == WALK_LEFT || c == WALK_LEFT - ('a'-'A'))){
			pc = maze[player.getY()][player.getX()-1];
			if(pc != Maze::WALL && pc != DOOR)
				player.walkLeft();			// x--
		}
            // Door opener
		else if(oneChar && c == ACTION_BTN){ 
    		for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
                if(player.isNear((*it)->getX(), (*it)->getY(), 1) && player.findItem("key", static_cast<Key*>(*it)->id) != 0){
                    obst.erase(it);
					maze[(*it)->getY()][(*it)->getX()] = Maze::PATH;
                    break;
                }
		}

			// ....
		if(maze[player.getY()][player.getX()] == Maze::END){ // If goal reached
			std::cout << "You've reached the exit!" << std::endl << "Congratulations!" << std::endl << std::endl;
			playing = false;
		}
		if(maze[player.getY()][player.getX()] == Game::KEY){ // If key found
			for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
				if(player.getY() == (*it)->getY() && player.getX() == (*it)->getX()){ // If the key is on players location
					 player.addItem("key", *it);
					 obst.erase(it);
					 maze[player.getY()][player.getX()] = Maze::PATH;
					 break;
				}
		}
	}
	catch(std::exception &e){}
}
void Game::idle(){
	// static Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, 0.0)),
	// 			   view = Matrix4().lookAt( Vector3(0.0, 10.0, 3.0), 		// eye position
	// 			   							Vector3(0.0, 0.0, 0.0), 	// focus point
	// 			   							Vector3(0.0, 1.0, 0.0) ),	// up direction
	// 				projection = Matrix4().Perspective(45.0f, 1.0f, 0.1f, 50.0f);

	// int time_ = glutGet(GLUT_ELAPSED_TIME);
	// float angle = time_ / 1000.0 * 10;

	// primitive_wall.updateElement();

	// Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
	// 				Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
	// 				Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));	// Z axis


	// Matrix4 m_transform = projection * view * model * anim;
	// //m_transform.Scale(Vector3(1.4,1.4,1.4));

	// glUseProgram(primitive_wall.getProgram());

	// glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());

	glutPostRedisplay();
}

void Game::printMaze(){
	if(!isPlaying()) return;

	static Matrix4 view = Matrix4().lookAt( Vector3(-10.5, 60.0, -10.0), 		// eye position
				   							Vector3(-10.0, 0.0, -10.0), 	// focus point
				   							Vector3(0.0, 0.0, 1.0) ),	// up direction
					projection = Matrix4().Perspective(45.0f, // fov
														1.0f, // aspect ratio
														1.0f, // z-near
														200.0f//z-far
													);

	// Add all walls that will be rendered
	for(int i=printing_maze.Get_height(); i > 0; i--)
		for(int j=printing_maze.Get_width(); j > 0; j--)
			if(player.isNear(j-1,i-1))
				printing_maze[i-1][j-1] = maze[i-1][j-1];
			// else
			//     printing_maze[i-1][j-1] = Maze::PATH;
	
	
	// sets obstacles position
	Maze m = printing_maze;
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		if(player.isNear((*it)->getX(), (*it)->getY()))
			m[(*it)->getY()][(*it)->getX()] = (*it)->getDisp();
	
	// // sets the player position
	// m[player.getY()][player.getX()] = Character::TECKEN;

	// TODO: Print maze:
	glUseProgram(primitive_wall.getProgram());
	for(int i=printing_maze.Get_height(); i > 0; i--)
		for(int j=printing_maze.Get_width(); j > 0; j--)
				if(printing_maze[i-1][j-1] == Maze::WALL){
					Matrix4 model = Matrix4().Translate(Vector3(-(j-1)*3, 0, -(i-1)*3)).Scale(Vector3(0.39,0.39,0.39));
					if(i-1 > 0 && i < maze.Get_height()){
						if( j-1 > 0 && j < maze.Get_height() && 
							  (maze[i][j-1] == Maze::WALL || maze[i-2][j-1] == Maze::WALL) && 
							  (maze[i-1][j] == Maze::WALL || maze[i-1][j-2] == Maze::WALL)){
							Matrix4 m_transform = projection * view * model;
							glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
							primitive_wall.render();
							model.RotateA(90.0f, Vector3(0,1,0));
						}
						else if(maze[i][j-1] == Maze::WALL || maze[i-2][j-1] == Maze::WALL)
							model.RotateA(90.0f, Vector3(0,1,0));
					}
					else if((i-1 > 0 && maze[i-2][j-1] == Maze::WALL) || (i < maze.Get_height() && maze[i][j-1] == Maze::WALL))
						model.RotateA(90.0f, Vector3(0,1,0));
					Matrix4 m_transform = projection * view * model;
					glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
					primitive_wall.render();
				}

		Matrix4 model = Matrix4().Translate(Vector3(-player.getX()*3, 0, -player.getY()*3)).Scale(Vector3(0.1,0.1,0.1));
		
		Matrix4 m_transform = projection * view * model;
		glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
		primitive_wall.render();
	glUseProgram(0);
	glPushMatrix();

		// glBegin(GL_TRIANGLES);
		// 	glVertex3f( 0, 3, 0 );
		// 	glVertex3f( 0, 0, 0 );
		// 	glVertex3f( 1, 0, 0 );
		// glEnd();
	glPopMatrix();
}

