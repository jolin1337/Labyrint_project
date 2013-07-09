CPPS = main.cpp Character.cpp Game.cpp Maze/Maze.cpp Inputs.cpp Linux/Linux.cpp Obstacle.cpp
HEADERS =       Maze/Maze.h   Inputs.h   Character.h   Game.h   Linux/Linux.h   Obstacle.h

all: maze_game

maze_game: $(CPPS) $(HEADERS) 
	g++ -o maze_game $(CPPS)

run: 
	./maze_game
LIBS = Maze/Maze.cpp Inputs.cpp Linux/Linux.cpp
# TODO: make libs
