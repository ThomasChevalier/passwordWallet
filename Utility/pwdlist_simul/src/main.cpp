#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "Globals.h"
#include "Password_List.h"
#include "Password.h"
#include "Fram.h"
#include "Random.h"

class CommandReader
{
public:
	enum Command
	{
		None,
		AddPwd,
		DelPwd,
		Reset,
		Exit,
		Save,
		ShowList
	};
	static Command getCommand()
	{
		std::map<std::string, Command> map =
		{ {"add", AddPwd},
		{"del", DelPwd},
		{"reset", Reset},
		{"exit", Exit},
		{"save", Save},
		{"showList", ShowList}
		};

		std::string cmd;
		std::cout << "> ";
		std::cin >> cmd;

		auto mapIt = map.find(cmd);
		if(mapIt!= map.end())
		{
			return mapIt->second;
		}
		else
		{
			std::cout << "Unknow command \"" << cmd << "\"" << std::endl;
			return Command::None;
		}
	}
};

void do_add_pwd()
{
	unsigned char name[32];
	unsigned char data[32] = "abcdefghijklmnopqrstuvwxyzabcde";
	unsigned char userName[64] = "abcdefghijklmnopqrstuvwxyzabcdeabcdefghijklmnopqrstuvwxyzabcded";
	std::cout << "Name : ";
	std::cin >> name;

	pwd_list_add_pwd(name, data, userName);
}

void do_del_pwd()
{
	int id;
	std::cout << "pwd id : ";
	std::cin >> id;
	pwd_list_delete_pwd(id);
}

void do_reset()
{
	for(int i(0); i < FRAM_BYTE_SIZE; ++i)
	{
		fram_write_byte(i,0);
	}
	NUM_PWD = 0;
}

void do_show()
{
	int id = pwd_list_get_first_pwd_id_sort_alpha();
	int i = 0;
	while(id!=MAXIMUM_NUMBER_OF_PWD)
	{
		++i;
		if(i > 30) {return;}

		unsigned char name[32];
		password_read_name(id, name);
		std::cout << "[" << id << "]" << reinterpret_cast<char*>(name) << std::endl;
		id = password_read_next_pwd_alpha(id);
	}
}

int main()
{
	std::cout << "Welcome in the simulation tool for the password list of PASSWORD WALLET.\n";

	fram_setup_hardware();
	random_init();

	CommandReader::Command cmd;
	while(cmd != CommandReader::Exit)
	{
		cmd = CommandReader::getCommand();
		switch(cmd)
		{
			case CommandReader::AddPwd:
			do_add_pwd();
			break;

			case CommandReader::DelPwd:
			do_del_pwd();
			break;

			case CommandReader::Reset:
			do_reset();
			break;

			case CommandReader::Save:
			fram_end();
			break;

			case CommandReader::ShowList:
			do_show();
			break;

			case CommandReader::None:
			case CommandReader::Exit:
			break;

			default:
			break;
		}
	}

	fram_end();
	return 0;
}