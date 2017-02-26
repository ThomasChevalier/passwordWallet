#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <iomanip>

#include "../../Code/src/Globals.h"

struct Info
{
	uint8_t options_flag;
	uint8_t first_pwd_util;
	uint8_t first_pwd_alpha;
	uint8_t num_pwd;
	uint16_t pool_size;
	uint8_t pool[256];
	uint8_t memory_map[6];
};

uint16_t readWord(std::ifstream& file)
{
	uint16_t word;
	uint8_t *byte = reinterpret_cast<uint8_t*>(&word);
	byte[0] = file.get();
	byte[1] = file.get();
	return word;
}

Info readInfo(std::ifstream& file)
{
	file.seekg(0);
	Info info;
	info.options_flag = file.get();
	info.first_pwd_util = file.get();
	info.first_pwd_alpha = file.get();
	info.num_pwd = file.get();
	info.pool_size = readWord(file);
	file.read(reinterpret_cast<char*>(info.pool), 256);
	file.read(reinterpret_cast<char*>(info.memory_map),6);
	return info;
}

void printInfo(const Info& info)
{
	std::ostringstream oss;
	oss << "Flags : ";

	if(info.options_flag & (1<<0))
	   oss << "No encrypt | ";

	if(info.options_flag & (1<<1))
	{
		if(info.options_flag & (1<<2))
			oss << "INVALID SORTING | ";
		else
			oss << "Sort usage | ";
	}
	else
	{
		if(info.options_flag & (1<<2))
			oss << "Sort alpha | ";
		else
			oss << "No sorting | ";
	}

	if(info.options_flag & (1<<3))
		oss << "Orientation reversed | ";
	if(info.options_flag & (1<<4))
		oss << "Initialized";

	oss << "\nFirst Pwd Util is " << static_cast<int>(info.first_pwd_util) << ".";
	oss << "\nFirst Pwd Alpha is " << static_cast<int>(info.first_pwd_alpha) << ".";
	oss << "\nThere is " << static_cast<int>(info.num_pwd) << " password";
	if(info.num_pwd > 1)
		oss << "s.";
	else
		oss << ".";

	oss << "\nEntropy pool size is " << static_cast<int>(info.pool_size) << ".";

	oss << "\nMemory map : ";
	for(uint8_t i = 0; i < 6; ++i)
	{
		uint8_t byte = info.memory_map[i];
		if(byte <= 0x0F)
			oss << "0x0" << std::hex << static_cast<int>(info.memory_map[i]) << std::dec;
		else
			oss << "0x" << std::hex << static_cast<int>(info.memory_map[i]) << std::dec;
		oss << " ";
	}
	std::cout << oss.str();
}

struct Password
{
	uint8_t prev_pwd_util;
	uint8_t next_pwd_util;
	uint8_t prev_pwd_alpha;
	uint8_t next_pwd_alpha;
	uint16_t count;
	char name [32];
	uint8_t iv_pwd[16];
	uint8_t pwd[32];
	uint8_t iv_usr[16];
	uint8_t usr[64];

	uint8_t id;
};

Password readPassword(std::ifstream& file, int pwd_id)
{
	if((pwd_id+1)*SIZE_OF_PWD_BLOCK + OFFSET_FIRST_PWD-1 > FRAM_BYTE_SIZE)
	{
		std::cerr << "Error, too large pwd_id.";
	}
	if(pwd_id < 0)
	{
		std::cerr << "Error, invalid pwd index";
	}

	uint16_t pwd_offset = pwd_id * SIZE_OF_PWD_BLOCK + OFFSET_FIRST_PWD;
	file.seekg(pwd_offset);
	Password pwd;
	pwd.prev_pwd_util = file.get();
	pwd.next_pwd_util = file.get();
	pwd.prev_pwd_alpha = file.get();
	pwd.next_pwd_alpha = file.get();
	pwd.count = readWord(file);
	file.read(pwd.name, 32);
	file.read(reinterpret_cast<char*>(pwd.iv_pwd), 16);
	file.read(reinterpret_cast<char*>(pwd.pwd), 32);
	file.read(reinterpret_cast<char*>(pwd.iv_usr), 16);
	file.read(reinterpret_cast<char*>(pwd.usr), 64);

	pwd.id = pwd_id;

	return pwd;
}

void printPassword(const Password& pwd, std::ifstream& file)
{
	std::cout << "Password " << static_cast<int>(pwd.id);
	std::cout << "\nName : " << pwd.name;
	std::cout << "\n\tCounter = " << static_cast<int>(pwd.count);

	std::cout << "\n\tPrevious password [usg] : " << static_cast<int>(pwd.prev_pwd_util);
	if(pwd.prev_pwd_util < MAXIMUM_NUMBER_OF_PWD)
	{
		Password previous = readPassword(file, pwd.prev_pwd_util);
		std::cout << " (" << previous.name << ")";
	}

	std::cout << "\n\tNext password [usg] : " << static_cast<int>(pwd.next_pwd_util);
	if(pwd.next_pwd_util < MAXIMUM_NUMBER_OF_PWD)
	{
		Password next = readPassword(file, pwd.next_pwd_util);
		std::cout << " (" << next.name << ")";
	}

	std::cout << "\n\tPrevious password [alpha] : " << static_cast<int>(pwd.prev_pwd_alpha);
	if(pwd.prev_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
	{
		Password previous = readPassword(file, pwd.prev_pwd_alpha);
		std::cout << " (" << previous.name << ")";
	}

	std::cout << "\n\tNext password [alpha] : " << static_cast<int>(pwd.next_pwd_alpha);
	if(pwd.next_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
	{
		Password next = readPassword(file, pwd.next_pwd_alpha);
		std::cout << " (" << next.name << ")";
	}
}

void parseCommand(std::string command, std::ifstream& file)
{
	static int pwd_id = 0;

	if(command == "quit")
		return;

	std::istringstream iss(command);
	std::string word;
	iss >> word;
	if(word == "read")
	{
		iss >> word;
		if(word == "info")
		{
			Info info = readInfo(file);
			printInfo(info);
		}
		else if(word == "password" || word == "pwd")
		{
			iss >> pwd_id;
			if(!iss)
			{
				std::cerr << "Invalid command";
			}
			else
			{
                Password pwd = readPassword(file, pwd_id);
                printPassword(pwd, file);
			}
		}
		else if(word == "next")
		{
            iss >> word;
            if(word == "alpha")
            {
                Password current = readPassword(file, pwd_id);
                pwd_id = current.next_pwd_alpha;
            }
            else if(word == "util" || word == "usage" || word == "usg")
            {
                Password current = readPassword(file, pwd_id);
                pwd_id = current.next_pwd_util;
            }
            else
            {
                ++pwd_id;
            }
			Password pwd = readPassword(file, pwd_id);
			printPassword(pwd, file);
		}
		else if(word == "previous" || word == "prev")
		{
            iss >> word;
			if(word == "alpha")
            {
                Password current = readPassword(file, pwd_id);
                pwd_id = current.prev_pwd_alpha;
            }
            else if(word == "util" || word == "usage" || word == "usg")
            {
                Password current = readPassword(file, pwd_id);
                pwd_id = current.prev_pwd_util;
            }
            else
            {
                --pwd_id;
            }
			Password pwd = readPassword(file, pwd_id);
			printPassword(pwd, file);
		}
		else if(word == "current")
		{
			Password pwd = readPassword(file, pwd_id);
			printPassword(pwd, file);
		}
		else if(word == "all")
		{
            Info info = readInfo(file);
			printInfo(info);
			pwd_id = 0;
			for(; pwd_id < MAXIMUM_NUMBER_OF_PWD; ++pwd_id)
			{
                Password pwd = readPassword(file, pwd_id);
                printPassword(pwd, file);
                std::cout << std::endl;
			}
		}
		else
		{
            std::cerr << "Unknow argument for command read \"" << word << "\"";
		}
	}
	else
	{
		std::cerr << "Unknow command \"" << command << "\"";
	}
}

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
	//std::string fileName("/home/thomas/Documents/Git/dump");
	std::ifstream file(fileName, std::ifstream::in | std::ifstream::binary);
	if(!file)
	{
		std::cerr << "Cannot open \"" << fileName << "\".\n";
		return 1;
	}

	std::string command;
	while(command != "quit")
	{
		std::cout << "\n> ";
		getline(std::cin, command);
		parseCommand(command, file);
	}


	return 0;
}
