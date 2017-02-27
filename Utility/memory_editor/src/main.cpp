#include <string>
#include <iostream>

#include "Application.h"

int main(int argc, char *argv[])
{

	if(argc != 2 || (argc == 2  && argv[1][0] == '-'
								&& argv[1][1] == '-'
								&& argv[1][2] == 'h'
								&& argv[1][3] == 'e'
								&& argv[1][4] == 'l'
								&& argv[1][5] == 'p'))
	{
		std::cout << "memory_viewer by Thomas Chevalier.\n(c) 2017 Thomas Chevalier - All rights reserved.\nthomasom [dot] chevalier [at] gmail [dot] com\nUsage : memory_viewer [fileName]\n\tfileName = The file name of the fram memory dump." << std::endl;
		return 0;
	}
	std::string fileName(argv[1]);

	Application app(fileName);
	app.run();


	return 0;
}
