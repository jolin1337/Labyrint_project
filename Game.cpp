
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
	if( w < 3) return;      // if width is too small abort
	start(w,h);             // start the game if right configs where made
}
void Game::start(int w, int h){
	if( w < 3) throw GameException("Small width"); // Throw exception if width is too small
	if(h < 3)           // if height is too small height will become the value of width
		h = w;

    maze.Create_maze(w,h);  // generate maze
	printing_maze = maze;   // the visible maze
	for(int i=printing_maze.Get_height()-1; i > 1; i--)
		for(int j=printing_maze.Get_width()-1; j > 1; j--)
			printing_maze[i-1][j-1] = ' ';  // clear maze
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
        // load object file to memory
		primitive_wall.loadObj("models/wall.obj");
		// primitive_wall.setScale(Vector3(0.5,0.5,0.5));

        // sets the opengl buffers for rendering for a signle wall
		setupBuffers(primitive_wall);
        
        // uniform that will be used in the shader 
		Uniform u("m_transform");   
		Attribute a1("position"),   // position that will represent all the vercies
				  // a2("texcoord"), // textures uppcomming TODOS
				  a3("color"),      // color of the object that was loaded
				  a4("normal");     // normal attribute
                  
        // Adds all the uniforms and attributes for this wall-object 
		primitive_wall.addUniform(u);
		primitive_wall.addAttribute(a1);
		// primitive_wall.addAttribute(a2);
		primitive_wall.addAttribute(a3);
		primitive_wall.addAttribute(a4);
        
        // load v-shader and f-shader to a program and bind to our properties
		status = primitive_wall.loadShaders(vertexShaderFileName, fragmentShaderFileName);
        if(status != 1)
            return status;
	}catch(int i){
        // this will happend when some files can't be found
		std::cout << "Failed to start the game" << std::endl;
		playing = false;
		return 0;
	}
    // generate obstacles
	createObstacles();
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
	}
}

bool Game::isPlaying() const {
	return playing; // are we still playing?
}

void Game::checkEvents(unsigned char key){
	unsigned char &c = key;
    
	try{
		bool oneChar = true;            // this will indicate if it is a special key or not
		char pc;                        // position character (pc)
		if(oneChar && c == 27)          // escape key was pressed
			playing = false;
		else if(oneChar && (c == WALK_UP || c == WALK_UP - ('a'-'A'))){ // up key was pressed
			pc = maze[player.getY()-1][player.getX()];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is a walkable char
				player.walkUp();			                    // y--
		}
		else if(oneChar && (c == WALK_DOWN || c == WALK_DOWN - ('a'-'A'))){// down key was pressed
			pc = maze[player.getY()+1][player.getX()];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is walkable char
				player.walkDown();	                        	// y++
		}
		else if(oneChar && (c == WALK_RIGHT || c == WALK_RIGHT - ('a'-'A'))) {// right key was pressed
			pc = maze[player.getY()][player.getX()+1];          // get position char
			if(pc != Maze::WALL && pc != DOOR)                  // if pc is walkable char
				player.walkRight();		                        // x++
		}
		else if(oneChar && (c == WALK_LEFT || c == WALK_LEFT - ('a'-'A'))){// left key was pressed
			pc = maze[player.getY()][player.getX()-1];          // get position char
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
		if(maze[player.getY()][player.getX()] == Maze::END){ // If goal reached
			std::cout << "You've reached the exit!" << std::endl << "Congratulations!" << std::endl << std::endl; // send message!
            // TODO: send opengl gui message here:
			playing = false;    // set playing state to false
		}
		if(maze[player.getY()][player.getX()] == Game::KEY){ // If key found
            // pickup key now
			for(std::vector<Obstacle *>::iterator it=obst.begin(); it != obst.end();++it)
				if(player.getY() == (*it)->getY() && player.getX() == (*it)->getX()){ // If the key is on players location
                     Obstacle *k = *it;
    				 obst.erase(it);
					 player.addItem("key", k); // add the key to players item(s)
					 maze[player.getY()][player.getX()] = Maze::PATH; // clean key position from maze
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

	Matrix4 view = Matrix4().lookAt( Vector3(-player.getX(), 10.0f, -player.getY()), 		// eye position
				   							Vector3(-player.getX(), 0.0f, -player.getY()), 			// focus point
				   							Vector3(0.0, 0.0, 1.0) ),								// up direction
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
					Matrix4 model = Matrix4().Translate(Vector3(-(j-1), 0, -(i-1))).Scale(Vector3(0.2,0.2,0.2));
					bool up =  (i-1 > 0 && maze[i-2][j-1] == Maze::WALL),
						 left= (j-1 > 0 && maze[i-1][j-2] == Maze::WALL),
						 down= (i < maze.Get_height() && maze[i][j-1] == Maze::WALL),
						 right=(j < maze.Get_width() && maze[i-1][j] == Maze::WALL);

					if((up && (left || right)) || (down && (left || right))){
						Matrix4 m_transform = projection * view * model;
						glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
						primitive_wall.render();
						model.RotateA(90.0f, Vector3(0,1,0));
					}
					else if(down || up)
						model.RotateA(90.0f, Vector3(0,1,0));

					Matrix4 m_transform = projection * view * model;
					glUniformMatrix4fv(primitive_wall.getUniform("m_transform"), 1, GL_FALSE, m_transform.getMatrix4());
					primitive_wall.render();
				}

		Matrix4 model = Matrix4().Translate(Vector3(-player.getX(), 0, -player.getY())).Scale(Vector3(0.1,0.1,0.1));
		
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

