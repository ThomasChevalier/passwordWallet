#include "Command.h"

#include <sstream>
#include <iostream>

Command::Command():
	m_type(None), m_command(), m_argument(0)
{
}

Command::Command(const std::string& command):
	m_type(None), m_command(command), m_argument(0)
{
	parseCommand();
}

int Command::getArgument() const
{
	return m_argument;
}

Command::Command_Type Command::getType() const
{
	return m_type;
}

void Command::setString(const std::string& command)
{
	m_command = command;
	parseCommand();
}

void Command::parseCommand()
{
    if(m_command.empty())
        return;

	std::istringstream iss(m_command);
	std::string word;
	iss >> word;

	m_type = Unknow;

	if(word == "quit")
	{
		m_type = Quit;
	}
	else if(word == "read")
	{
		iss >> word;
		if(word == "info")
		{
			m_type = ReadInfo;
		}
		else if(word == "password" || word == "pwd")
		{
			int id = 0;
			iss >> id;
			if(!iss || id > 255)
			{
				std::cerr << "Invalid index\n";
			}
			else
			{
				m_type = ReadIndex;
				m_argument = id;
			}
		}
		else if(word == "next")
		{
			iss >> word;
			if(word == "alpha")
			{
				m_type = ReadNextAlpha;
			}
			else if(word == "util" || word == "usage" || word == "usg")
			{
				m_type = ReadNextUtil;
			}
			else
			{
				m_type = ReadNext;
			}
		}
		else if(word == "previous" || word == "prev")
		{
			iss >> word;
			if(word == "alpha")
			{
				m_type = ReadPrevAlpha;
			}
			else if(word == "util" || word == "usage" || word == "usg")
			{
				m_type = ReadPrevUtil;
			}
			else
			{
				m_type = ReadPrev;
			}
		}
		else if(word == "current")
		{
			m_type = ReadCurrent;
		}
		else if(word == "all")
		{
			m_type = ReadAll;
		}
		else
		{
			std::cerr << "Unknow argument for command read \"" << word << "\"\n";
		}
	}
	else if(word == "set")
	{
		iss >> word;
		if(word == "key")
		{
			m_type = SetKey;
		}
		else
		{
			std::cerr << "Unknow argument for command set \"" << word << "\"\n";
		}
	}
	else
	{
		std::cerr << "Unknow command \"" << m_command << "\"\n";
	}
}
