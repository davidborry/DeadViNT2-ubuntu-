#include <iostream>
#include "src/headers/Application.hpp"

int main(){

	//printf("Hello world!\n");

	try{
		//Game game;
		//game.run();

		Application app;
		app.run();
	}

	catch (std::exception& e){
		std::cout << "\nEXCEPTION " << e.what() << std::endl;
	}

	return 0;
	}
