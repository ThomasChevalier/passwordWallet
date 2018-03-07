#include <iostream>
#include <fstream>

#include "Command.h"

#include "dependency/Globals.h"
#include "Fram.h"

void intro()
{
	std::cout << "pindef_parser by Thomas Chevalier.\n"
			 "Copyright 2017 Thomas Chevalier - All rights reserved.\n"
			 "thomasom.chevalier@gmail.com\n"
			 "Compiled on " __DATE__ " at " __TIME__ << std::endl;
}

void set_key(const std::string& ascii85_key)
{
	const char *input= ascii85_key.c_str();

	uint8_t i = 0;
	uint32_t powers[5] = {52200625, 614125, 7225, 85, 1};
	uint8_t pos = 0;
	for(; i < 4; ++i)
	{
		uint32_t sum = 0;
		uint8_t j = 0;
		for(; j < 5; ++j)
		{
			sum += (input[j]-33) * powers[j];
		}
		KEY[0+pos] = sum >> 24;
		KEY[1+pos] = (sum & 0xFF0000) >> 16;
		KEY[2+pos] = (sum & 0xFF00) >> 8;
		KEY[3+pos] = sum & 0xFF;

		input += 5;
		pos += 4;
	}
}

int main()
{
	intro();

	std::cout << "Memory file : ";
	std::string memory_file;
	std::cin >> memory_file;
	std::ifstream file(memory_file, std::ios_base::binary);
	if(!file){
		file.close();
		std::ofstream new_file(memory_file, std::ios_base::binary);
		std::cout << "The file \"" << memory_file << "\" has been created\n";
		for(unsigned i=0; i < FRAM_BYTE_SIZE; ++i){
			char dummy = 0;
			new_file.write(&dummy, 1);
		}
		new_file.close();
		file.open(memory_file, std::ios_base::binary);
	};

	file.seekg (0, file.end);
	int size = file.tellg();
	file.seekg (0, file.beg);
	if(size != FRAM_BYTE_SIZE){
		std::cout << "Error : file is " << size << " bytes long and " << FRAM_BYTE_SIZE << " was expected." << std::endl;
		return 1;
	}
	std::cout << "Creating virtual memory ";
	uint8_t *memory = new uint8_t[size];
	std::cout << "OK" << std::endl;

	std::cout << "Reading file ";
	int read  = 0;
	while(read < size){
		file.read(reinterpret_cast<char*>(memory), size - read);
		read = file.gcount();
	}
	file.close();
	std::cout << "OK" << std::endl;

	fram_init(memory);

	update_opt_flags((1<<OPTIONS_FLAG_OFFSET_INITIALIZED));
	std::cout << "Successfuly read " << size << " bytes" << std::endl;


	std::string ascii85_key;
	while(ascii85_key.size() != 20){
		std::cout << "Enter the key in ascii85 format : ";
		std::cin >> ascii85_key;
	}

	set_key(ascii85_key);

	Command com;
	while(com.type() != Command::Exit)
	{
		com.read();
		com.exec();
	}

	std::cout << "Saving file ";
	std::ofstream save_file(memory_file);
	for(int i(0); i < FRAM_BYTE_SIZE; ++i){
		const char data = memory[i];
		save_file.write(&data, 1);
	}
	std::cout << "OK" << std::endl;

	delete[] memory;
}