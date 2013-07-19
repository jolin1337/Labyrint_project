HEADERS = Maze/Maze.h Inputs.h Character.h Game.h Linux/Linux.h Obstacle.h render.h

all: maze_game

maze_game: libgame.a $(HEADERS) 
	g++ -o maze_game main2.cpp -L./libs -lgame -lglut -lGLEW -lGL -lGLU

run: 
	./maze_game
LIBS = Maze/Maze.cpp Inputs.cpp Linux/Linux.cpp
LIBS_o = Maze.o Inputs.o Linux.o

LIBS_GAME = Character.cpp Game.cpp Obstacle.cpp render.cpp
LIBS_GAME_o = Character.o Game.o Obstacle.o render.o

libmaze.a: $(LIBS) $(HEADERS)
	g++ -c $(LIBS)
	ar -rvs $(LIBS_o)
	make clean_os
	mv libmaze.a libs/

libgame.a: $(LIBS) $(LIBS_GAME) $(HEADERS)
	g++ -c $(LIBS_GAME) $(LIBS)
	ar -rvs libgame.a $(LIBS_GAME_o) $(LIBS_o)
	make clean_os
	mv libgame.a libs/

clean_os:
	rm -f $(LIBS_o)
	rm -f $(LIBS_GAME_o)

clean: clean_os
	rm -f libs/libmaze.a libs/libgame.a maze_game