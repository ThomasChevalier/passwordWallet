#include "Info.h"

#include <cstring>
#include <iomanip>

#include "FileHelper.h"

Info::Info():
	options_flag(0), first_pwd_util(0), first_pwd_alpha(0), num_pwd(0), pool_size(0)
{
	memset(pool, 0, 256);
	memset(memory_map, 0, 6);
}

Info::Info(std::ifstream& file):
	options_flag(0), first_pwd_util(0), first_pwd_alpha(0), num_pwd(0), pool_size(0)
{
	memset(pool, 0, 256);
	memset(memory_map, 0, 6);
	readFromFile(file);
}

void Info::readFromFile(std::ifstream& file)
{
	file.seekg(0);
	options_flag = file.get();
	first_pwd_util = file.get();
	first_pwd_alpha = file.get();
	num_pwd = file.get();
	pool_size = readWord(file);
	file.read(reinterpret_cast<char*>(pool), 256);
	file.read(reinterpret_cast<char*>(memory_map),6);
}

void Info::print(std::ostream& stream) const
{
	stream << "Flags : ";

	if(options_flag & (1<<0))
	   stream << "No encrypt | ";

	if(options_flag & (1<<1))
	{
		if(options_flag & (1<<2))
			stream << "INVALID SORTING | ";
		else
			stream << "Sort usage | ";
	}
	else
	{
		if(options_flag & (1<<2))
			stream << "Sort alpha | ";
		else
			stream << "No sorting | ";
	}

	if(options_flag & (1<<3))
		stream << "Orientation reversed | ";
	if(options_flag & (1<<4))
		stream << "Initialized";

	stream << "\nFirst Pwd Util is " << static_cast<int>(first_pwd_util) << ".";
	stream << "\nFirst Pwd Alpha is " << static_cast<int>(first_pwd_alpha) << ".";
	stream << "\nThere is " << static_cast<int>(num_pwd) << " password";
	if(num_pwd > 1)
		stream << "s.";
	else
		stream << ".";

	stream << "\nEntropy pool size is " << static_cast<int>(pool_size) << ".";

	stream << "\nMemory map : ";
	for(uint8_t i = 0; i < 6; ++i)
	{
		uint8_t byte = memory_map[i];
		if(byte <= 0x0F)
			stream << "0x0" << std::hex << static_cast<int>(memory_map[i]) << std::dec;
		else
			stream << "0x" << std::hex << static_cast<int>(memory_map[i]) << std::dec;
		stream << " ";
	}
}


std::ostream& operator<<( std::ostream &stream, const Info & info )
{
	info.print(stream);
	return stream;
}
