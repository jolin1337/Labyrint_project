
#include "Game.h"
#include <iostream>
#include "setup.h"

#include "Environment.h"


const char 	*Game::vertexShaderFileName = "shaders/wall.v-shader", 
			*Game::fragmentShaderFileName = "shaders/wall.f-shader";
	// Möjlig nyckel: fungerar u, 
	//                fungerar F
	// Möjlig dörr:   fungerar E, 
	//                fungerar n,
	//                   fungerar o

Game::Game(int w, int h):playing(false) {
	if( w < 3) return;      // if width is too small abort
	start(w,h);             // start the game if right configs where made
}
void Game::start(int w, int h){
	if( w < 3) throw GameException("Small width @24"); // Throw exception if width is too small
	if(h < 3)           // if height is too small height will become the value of width
		h = w;			// set height to width instead

    maze.Create_maze(w,h);  // generate maze
	printing_maze = maze;   // the visible maze

	  // clear the entire maze
	for(int i=printing_maze.Get_height()-1; i > 1; i--)
		for(int j=printing_maze.Get_width()-1; j > 1; j--)
			printing_maze[i-1][j-1] = ' ';	// clear to space
	init();                 // initialize components to render
}

Game::~Game(){
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		delete *it;         // delete all obstacles
	Obstacle *ob;           // for all items in player
	while((ob = player.destroyItem("key")) != 0)
		delete ob;          // delete ob key
}

int Game::init() {
	// sets to playing state
	playing = true;

    bool status = 0;
	try{
        // load object files to memory
		primitiv_wall.loadObj("models/wall.obj");
		primitiv_key.loadObj("models/key.obj");
		// primitiv_wall.setScale(Vector3(0.5,0.5,0.5));

        // sets the opengl buffers for rendering the primitives
		setupBuffers(primitiv_wall);
		setupBuffers(primitiv_key);
        
        // uniform that will be used in the shader 
		Uniform u("m_transform");   
		Attribute a1("position"),   // position that will represent all the vercies
				  // a2("texcoord"), // textures uppcomming TODOS
				  a3("color"),      // color of the object that was loaded
				  a4("normal");     // normal attribute
                  
        // Add all the uniforms and attributes for this wall-object 
		primitiv_wall.addUniform(u);
		primitiv_wall.addAttribute(a1);
		primitiv_wall.addAttribute(a3);
		primitiv_wall.addAttribute(a4);

        // Add all the uniforms and attributes for this key-object 
		primitiv_key.addUniform(u);
		primitiv_key.addAttribute(a1);
		primitiv_key.addAttribute(a3);
		primitiv_key.addAttribute(a4);
        
        // load vertex-shader and fragment-shader to a program and bind to our properties
		status = primitiv_wall.loadShaders(vertexShaderFileName, fragmentShaderFileName) && 
				 primitiv_key.loadShaders(vertexShaderFileName, fragmentShaderFileName);
        if(status != 1) // if loadshaders failed to load
            return status; // abort

	    // generate obstacles
		createObstacles();
	}catch(int i){
        // this will happend when some files can't be found
		std::cout << "Failed to start the game" << std::endl;
		playing = false;
		return 0;
	}
	return status;
}
void Game::createObstacles() {
	if(obst.size())     // if already created obstacles abort!
		return;
        
	// instantiate obstacles here:
	Maze solved = maze;         // the solved maze of the generated one
	Obstacle *door = new Door(-1,-1, DOOR), // a door obstacle
             *key = new Key(-1,-1, KEY);    // a key item
	Obstacle::generateObstacle(solved, door, key);
	
    // if we sucessfully generated a key and a door
	if(door->getX() > -1 && door->getY() > -1 && key->getX() > -1 && key->getY() > -1){
		((Door*)door)->id = ((Key*)key)->id = obst.size()+1; // set a id to both so that this key will lead to this door
		obst.push_back(door);   // add door
		obst.push_back(key);    // add key
        
        // set the door and key to maze
		maze[door->getY()][door->getX()] = DOOR;
		maze[key->getY()][key->getX()] = KEY;
		std::cout << "Created obstacles...\n";
	}
}

bool Game::isPlaying() const {
	return playing; // are we still playing?
}
 
void Game::checkEvents(unsigned char key, bool oneChar){
	unsigned char &c = key;
    
	try{
		char pc;                        // position character (pc)
		if(oneChar && c == 27)          // escape key was pressed
			playing = false;
		else if((!oneChar && c == GLUT_KEY_UP) || (oneChar && (c == WALK_UP || c == WALK_UP - ('a'-'A')))){ // up key was pressed
			pc = maze[(int)(player.getY()-Character::stepSize)][(int)player.getX()];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is a walkable char
				player.walkUp();			                    // y--
		}
		else if((!oneChar && c == GLUT_KEY_DOWN) || (oneChar && (c == WALK_DOWN || c == WALK_DOWN - ('a'-'A')))){// down key was pressed
			pc = maze[(int)(player.getY()+Character::stepSize)][(int)player.getX()];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is walkable char
				player.walkDown();	                        	// y++
		}
		else if((!oneChar && c == GLUT_KEY_RIGHT) || (oneChar && (c == WALK_RIGHT || c == WALK_RIGHT - ('a'-'A')))) {// right key was pressed
			pc = maze[(int)player.getY()][(int)(player.getX()+Character::stepSize)];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is walkable char
				player.walkRight();		                        // x++
		}
		else if((!oneChar && c == GLUT_KEY_LEFT) || (oneChar && (c == WALK_LEFT || c == WALK_LEFT - ('a'-'A')))){// left key was pressed
			pc = maze[(int)player.getY()][(int)(player.getX()-Character::stepSize)];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is walkable char
				player.walkLeft();			                    // x--
		}
            // Door opener
		else if(oneChar && c == ACTION_BTN){                    // action button (ie space)
            // search for near obstacles
    		for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
                if(player.isNear((*it)->getX(), (*it)->getY(), 1) && player.findItem("key", static_cast<Key*>(*it)->id) != 0){
                    // door-obstacle detected with same id as some of the keys in player-bag
                    obst.erase(it); // make the player open the door
					maze[(*it)->getY()][(*it)->getX()] = Maze::PATH; // clean the key position form maze
                    break;
                }
		}

			// ....
		if(maze[(int)player.getY()][(int)player.getX()] == Maze::END){ // If goal reached
			std::cout << "You've reached the exit!" << std::endl << "Congratulations!" << std::endl << std::endl; // send message!
            // TODO: send opengl gui message here:
			playing = false;    // set playing state to false
		}
		if(maze[(int)player.getY()][(int)player.getX()] == Game::KEY){ // If key found
            // pickup key now
			for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
				if((int)player.getY() == (*it)->getY() && (int)player.getX() == (*it)->getX()){ // If the key is on players location
                     Obstacle *k = *it;
    				 obst.erase(it);
					 player.addItem("key", k); // add the key to players item(s)
					 maze[(int)player.getY()][(int)player.getX()] = Maze::PATH; // clean key position from maze
					 break;
				}
		}
	}
	catch(std::exception &e){}
}
void Game::idle(){

	// TODO: update key movements
	// TODO: update animations
	glutPostRedisplay();
}

void Game::printMaze(){
	if(!isPlaying()) return;

	Matrix4 view = Matrix4().lookAt( Vector3(-player.getX(), 10.0f, -player.getY()), 				// eye position
				   							Vector3(-player.getX(), 0.0f, -player.getY()), 			// focus point
				   							Vector3(0.0, 0.0, 1.0) ),								// up direction
					projection = Matrix4().Perspective(45.0f, 										// fov
														(float)Environment::screen_width / (float)Environment::screen_height, // aspect ratio
														1.0f, 										// z-near
														200.0f										//z-far
													);

	// Add all walls that will be rendered
	for(int i=printing_maze.Get_height(); i > 0; i--)
		for(int j=printing_maze.Get_width(); j > 0; j--)
			if(player.isNear(j-1,i-1))		// if character is near this position
				printing_maze[i-1][j-1] = maze[i-1][j-1];// show this position
			// else
			//     printing_maze[i-1][j-1] = ' ';	// hide position
	
	
	// sets obstacles position
	Maze m = printing_maze;
	for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();it++)
		if(player.isNear((*it)->getX(), (*it)->getY()))
			m[(*it)->getY()][(*it)->getX()] = (*it)->getDisp();	// render obstacle if character is near

	for(int i=printing_maze.Get_height(); i > 0; i--)
		for(int j=printing_maze.Get_width(); j > 0; j--)
				if(printing_maze[i-1][j-1] == Maze::WALL){
					glUseProgram(primitiv_wall.getProgram());				// use program of the wall
					
					Matrix4 model = Matrix4().Translate(Vector3(-(j-1)-0.5f, 0, -(i-1)-0.5f)).Scale(Vector3(0.2,0.2,0.2));
					bool up =  (i-1 > 0 && maze[i-2][j-1] == Maze::WALL),
						 left= (j-1 > 0 && maze[i-1][j-2] == Maze::WALL),
						 down= (i < maze.Get_height() && maze[i][j-1] == Maze::WALL),
						 right=(j < maze.Get_width() && maze[i-1][j] == Maze::WALL);

					if((up && (left || right)) || (down && (left || right))){
						Matrix4 m_transform = projection * view * model;
						glUniformMatrix4fv(primitiv_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
						primitiv_wall.render();
						model.RotateA(90.0f, Vector3(0,1,0));
					}
					else if(down || up)
						model.RotateA(90.0f, Vector3(0,1,0));

					Matrix4 m_transform = projection * view * model;
					glUniformMatrix4fv(primitiv_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
					primitiv_wall.render();
				}
				else if(printing_maze[i-1][j-1] == KEY){
					glUseProgram(primitiv_key.getProgram());				// use program of the key
					Matrix4 model = Matrix4().Translate(Vector3(-(j-1)-0.5f, 0, -(i-1)-0.5f)).Scale(Vector3(0.2,0.2,0.2));

					Matrix4 m_transform = projection * view * model;
					glUniformMatrix4fv(primitiv_key.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
					primitiv_key.render();
				}

		Matrix4 model = Matrix4().Translate(Vector3(-player.getX(), 0, -player.getY())).Scale(Vector3(0.1,0.1,0.1));
		
		Matrix4 m_transform = projection * view * model;
		glUniformMatrix4fv(primitiv_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
		primitiv_wall.render();

		model.RotateA(90.0f, Vector3(0,1,0));
		m_transform = projection * view * model;
		glUniformMatrix4fv(primitiv_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
		primitiv_wall.render();
	glUseProgram(0);
	glPushMatrix();

		// glBegin(GL_TRIANGLES);
		// 	glVertex3f( 0, 3, 0 );
		// 	glVertex3f( 0, 0, 0 );
		// 	glVertex3f( 1, 0, 0 );
		// glEnd();
	glPopMatrix();
}

