#include "Password.h"

#include <cstring>
#include <iostream>

#include "FileHelper.h"

#include "../../Code/src/Globals.h"

Password::Password(uint8_t id_, std::ifstream& file):
	prev_pwd_util(0), next_pwd_util(0), prev_pwd_alpha(0), next_pwd_alpha(0), count(0), id(id_),
	m_file(file)
{
	memset(name, 0, 32);
	memset(iv_pwd, 0, 16);
	memset(pwd, 0, 32);
	memset(iv_usr, 0, 16);
	memset(usr, 0, 64);

	readFromFile();
}

void Password::readFromFile()
{
	if((id+1)*SIZE_OF_PWD_BLOCK + OFFSET_FIRST_PWD-1 > FRAM_BYTE_SIZE)
	{
		std::cerr << "Error, too large pwd_id.\n";
		return;
	}
	if(id < 0)
	{
		std::cerr << "Error, invalid pwd index\n";
		return;
	}

	uint16_t pwd_offset = id * SIZE_OF_PWD_BLOCK + OFFSET_FIRST_PWD;
	m_file.seekg(pwd_offset);
	prev_pwd_util = m_file.get();
	next_pwd_util = m_file.get();
	prev_pwd_alpha = m_file.get();
	next_pwd_alpha = m_file.get();
	count = readWord(m_file);
	m_file.read(name, 32);
	m_file.read(reinterpret_cast<char*>(iv_pwd), 16);
	m_file.read(reinterpret_cast<char*>(pwd), 32);
	m_file.read(reinterpret_cast<char*>(iv_usr), 16);
	m_file.read(reinterpret_cast<char*>(usr), 64);
}

void Password::print(std::ostream& stream) const
{
	stream << "Password " << static_cast<int>(id);
	stream << "\nName : " << name;
	stream << "\n\tCounter = " << static_cast<int>(count);

	stream << "\n\tPrevious password [usg] : " << static_cast<int>(prev_pwd_util);
	if(prev_pwd_util < MAXIMUM_NUMBER_OF_PWD)
	{
		Password previous(prev_pwd_util, m_file);
		stream << " (" << previous.name << ")";
	}

	stream << "\n\tNext password [usg] : " << static_cast<int>(next_pwd_util);
	if(next_pwd_util < MAXIMUM_NUMBER_OF_PWD)
	{
		Password next(next_pwd_util, m_file);
		stream << " (" << next.name << ")";
	}

	stream << "\n\tPrevious password [alpha] : " << static_cast<int>(prev_pwd_alpha);
	if(prev_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
	{
		Password previous(prev_pwd_alpha, m_file);
		stream << " (" << previous.name << ")";
	}

	stream << "\n\tNext password [alpha] : " << static_cast<int>(next_pwd_alpha);
	if(next_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
	{
		Password next(next_pwd_alpha, m_file);
		stream << " (" << next.name << ")";
	}
}


std::ostream& operator<<( std::ostream &stream, const Password& pwd )
{
	pwd.print(stream);
	return stream;
}
