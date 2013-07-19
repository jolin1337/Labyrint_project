#include <iostream>
#include "Inputs.h"
#include "Game.h"

int main(int argc, char const *argv[])
{
	std::string choice;
	int col, row;
	std::cout << "\t -:: Labyrinth game ::-" << std::endl;
	while (1){ // Simple user interface
		std::cout 
			<< "Choose option" << std::endl
			<< "[1] -\t Play" << std::endl
			<< "[2] -\t Instructions" << std::endl
			<< "[3] -\t About" << std::endl
			<< "[4] -\t Exit program" << std::endl;
		getline(std::cin, choice);
		std::cout << std::endl;
		if (choice == "1"){ // play
			std::cout 
				<< "What difficulty would you play" << std::endl
				<< "[1] -\t Easy" << std::endl
				<< "[2] -\t Normal" << std::endl
				<< "[3] -\t Hard" << std::endl;
			std::getline(std::cin, choice);
			if(choice == "1")
				col = row = 10;
			else if(choice == "2")
				col = row = 30;
			else if(choice == "3")
				col = row = 100;

			if (Check_valid_size(col) && Check_valid_size(row)){	
					// Generate a maze
				Game::start(col, row);
			}
			continue;
		}
		else if(choice == "2"){ // Print about message
			std::cout << "How to" << std::endl;
		}
		else if(choice == "3"){ // Print about message
			std::cout << "Awesome game!" << std::endl;
			std::cout << "Made by Johannes Lindén and Marcus Ekström!" << std::endl;
		}
		else if(choice == "4") // exit
			return 0;
		else
			std::cout << "Please choose between [1], [2], [3] and [4]." << std::endl;
	}




	return 0;
}








