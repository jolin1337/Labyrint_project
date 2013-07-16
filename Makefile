# Makefile
utils = utils/GLobject.cpp utils/Matrix4.cpp utils/config.cpp utils/Data.cpp
LIBS = -lglut -lGLEW -lGL -lGLU

CPPS = \
			Events.cpp Environment.cpp \
			Character.cpp Game.cpp Obstacle.cpp \
			Maze/Maze.cpp

all: a.out

a.out: main.cpp $(utils) $(CPPS)
	g++ -o a.out main.cpp $(utils) $(CPPS) $(LIBS) 
