#include "Application.h"

#include <cstdlib>
#include <iostream>

#include "Command.h"
#include "Info.h"
#include "Password.h"

#include "../../Code/src/Globals.h"

Application::Application(const std::string& fileName):
	m_currentId(0)
{
	openFile(fileName);
}

void Application::run()
{
	Command cmd("");
	do
	{
		std::cout << "\n> " << std::flush;
		std::string str;
		getline(std::cin, str);
		cmd.setString(str);
		execute(cmd);
	}while(cmd.getType() != Command::Quit);
}

void Application::openFile(const std::string& fileName)
{
	m_file.open(fileName, std::ifstream::in | std::ifstream::binary);
	if(!m_file)
	{
		std::cerr << "Cannot open \"" << fileName << "\".\n";
		quit();
	}
}

void Application::quit()
{
	std::cerr << "Error made application quit.\n";
	exit(1);
}

void Application::execute(const Command& cmd)
{
	Command::Command_Type type = cmd.getType();
	if(type == Command::ReadInfo)
	{
		Info info(m_file);
		std::cout << info;
	}
	else if(type == Command::ReadNextUtil)
	{
		Password pwd(m_currentId, m_file);
		if(pwd.next_pwd_util < MAXIMUM_NUMBER_OF_PWD)
		{
			m_currentId = pwd.next_pwd_util;
			Password next(m_currentId, m_file);
			std::cout << next;
		}
		else
		{
			std::cout << "Next util is invalid." << std::endl;
		}

	}
	else if(type == Command::ReadPrevUtil)
	{
		Password pwd(m_currentId, m_file);
		if(pwd.prev_pwd_util < MAXIMUM_NUMBER_OF_PWD)
		{
			m_currentId = pwd.prev_pwd_util;
			Password prev(m_currentId, m_file);
			std::cout << prev;
		}
		else
		{
			std::cout << "Prev util is invalid." << std::endl;
		}

	}
	else if(type == Command::ReadNextAlpha)
	{
		Password pwd(m_currentId, m_file);
		if(pwd.next_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
		{
			m_currentId = pwd.next_pwd_alpha;
			Password next(m_currentId, m_file);
			std::cout << next;
		}
		else
		{
			std::cout << "Next alpha is invalid." << std::endl;
		}

	}
	else if(type == Command::ReadPrevAlpha)
	{
		Password pwd(m_currentId, m_file);
		if(pwd.prev_pwd_alpha < MAXIMUM_NUMBER_OF_PWD)
		{
			m_currentId = pwd.prev_pwd_alpha;
			Password prev(m_currentId, m_file);
			std::cout << prev;
		}
		else
		{
			std::cout << "Prev alpha is invalid." << std::endl;
		}

	}
	else if(type == Command::ReadNext)
	{
		if(m_currentId >= MAXIMUM_NUMBER_OF_PWD-1)
		{
			std::cout << "This is the last password available (id = " << m_currentId << ")." << std::endl;
		}
		else
		{
			++m_currentId;
			Password next(m_currentId, m_file);
			std::cout << next;
		}
	}
	else if(type == Command::ReadPrev)
	{
		if(m_currentId == 0)
		{
			std::cout << "This is the first password available (id = " << m_currentId << ")." << std::endl;
		}
		else
		{
			--m_currentId;
			Password prev(m_currentId, m_file);
			std::cout << prev;
		}
	}
	else if(type == Command::ReadCurrent)
	{
		Password current(m_currentId, m_file);
		std::cout << current;
	}
	else if(type == Command::ReadIndex)
	{
		m_currentId = cmd.getArgument();
		Password current(m_currentId, m_file);
		std::cout << current;
	}
	else if(type == Command::ReadAll)
	{
		Info info(m_file);
		std::cout << info << std::endl;
		int id = 0;
		for(; id < MAXIMUM_NUMBER_OF_PWD; ++id)
		{
			Password pwd(id, m_file);
			std::cout << pwd << std::endl;
		}
	}
}
