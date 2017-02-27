#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>


int main(int argc, char *argv[])
{
	if(argc != 3 || (argc == 2  && argv[1][0] == '-'
								&& argv[1][1] == '-'
								&& argv[1][2] == 'h'
								&& argv[1][3] == 'e'
								&& argv[1][4] == 'l'
								&& argv[1][5] == 'p'))
	{
		std::cout << "key_helper by Thomas Chevalier.\n(c) 2017 Thomas Chevalier - All rights reserved.\nthomasom [dot] chevalier [at] gmail [dot] com\nUsage : key_helper [key] [fileName]\n\tkey = the key in hex format, like this : 0011aaff\n\tfileName = The file name where the key should be save." << std::endl;
		return 0;
	}
	std::string key(argv[1]);
	std::string fileName(argv[2]);

	if(key.size() % 2 != 0)
	{
		std::cerr << "Error : key size is not a even.\n";
		return 1;
	}

	std::vector<char> data(key.size()/2, 0);

	for(unsigned i(0); i < key.size()/2; ++i)
	{
		char high = tolower(key[i*2]);
		if(high >= '0' && high <= '9')
		{
			high -= '0';
		}
		else if(high >= 'a' && high <= 'f')
		{
			high -= ('a'-10);
		}
		else
		{
			std::cerr << "Error : invalid character \'" << high << "\'. Key must be in hex format.\n";
			return 1;
		}
		char low = tolower(key[i*2+1]);
		if(low >= '0' && low <= '9')
		{
			low -= '0';
		}
		else if(low >= 'a' && low <= 'f')
		{
			low -= ('a'-10);
		}
		else
		{
			std::cerr << "Error : invalid character \'" << low << "\'. Key must be in hex format.\n";
			return 1;
		}
		data[i] = (high << 4) + low;
	}

	std::ofstream file(fileName);
	if(!file)
	{
		std::cerr << "Error : can't open " << fileName << " in write only mode.\n";
		return 1;
	}
	file.write(data.data(), data.size());
}