#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>

#include "Bitmap.h"
#include "Pixel.h"
#include "FileHeader.h"
#include "ImageHeader.h"

int main(int argc, char *argv[])
{
	if((argc != 2 && argc != 3) || (argc == 2  && argv[1][0] == '-'
								&& argv[1][1] == '-'
								&& argv[1][2] == 'h'
								&& argv[1][3] == 'e'
								&& argv[1][4] == 'l'
								&& argv[1][5] == 'p')
		|| (argc == 3 && (argv[2][0] != '-' ||
						  argv[2][1] != 'c' ||
						  argv[2][2] != 'o' ||
						  argv[2][3] != 'l')
						  ))
	{
		std::cout << "random_to_bitmap by Thomas Chevalier.\n(c) 2017 Thomas Chevalier - All rights reserved.\nthomasom [dot] chevalier [at] gmail [dot] com\nUsage : random_to_bitmap [fileName] [-col]\n\tfileName = The file name of the raw random data file.\n\t-col = [Optional] The output image has color" << std::endl;
		return 0;
	}

	bool col = argc == 3;

	std::ifstream file(argv[1]);
	if(!file){
		std::cerr << "Unable to open \"" << argv[1] << "\".\n";
		return 1;
	}
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0);

	int dim = col ? std::sqrt(fileSize/3.0) : std::sqrt(fileSize);
	std::cout << "Creating a " << dim << "*" << dim << " bitmap." << std::endl;

	Bitmap bmp(dim, dim);
	for(int i = 0; i < dim; ++i)
	{
		for(int j = 0; j < dim; ++j)
		{
			if(col)
			{
				char buffer[3];
				file.read(buffer, 3);
				bmp.setPixel(j, i, Pixel(buffer[0], buffer[1], buffer[2]));
			}
			else
			{
				char byte;
				file.read(&byte, 1);
				bmp.setPixel(j, i, Pixel(byte, byte, byte));
			}
		}
	}

	bmp.setFileName(std::string(argv[1]) + ".bmp");
	bmp.save();

	return 0;
}
