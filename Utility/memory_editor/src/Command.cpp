#include "Command.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "dependency/Security/Password_List.h"

Command::Command():
	m_type(Type::None)
{

}

void Command::read()
{
	std::vector<std::string> valid_str(Type::None);
	valid_str[Type::New] = "new";
	valid_str[Type::Delete] = "del";
	valid_str[Type::Change] = "change";
	valid_str[Type::Show] = "show";
	valid_str[Type::Exit] = "quit";

	std::string user_input;
	auto it = std::find(valid_str.begin(), valid_str.end(), user_input);
	while(it == valid_str.end())
	{
		std::cout << "> ";
		std::cin >> user_input;
		it = std::find(valid_str.begin(), valid_str.end(), user_input);
		if(it == valid_str.end()){
			std::cout << "Invalid command\n";
		}
	}

	m_type = static_cast<Type>(std::distance(valid_str.begin(), it));
	switch(m_type)
	{
		case Type::New:
			ask_new();
			break;
		case Type::Delete:
			ask_which();
			m_currentPwd.op = PasswordInfo::Del;
			break;
		case Type::Change:
			ask_which();
			ask_what();
			break;
		default:
			break;
	}
}

void Command::exec()
{
	if(m_type == Type::New){
		uint8_t name[32];
		uint8_t pwd[32];
		uint8_t usr[64];


		for(int i(0); i < 64; ++i){
			if(i < m_currentPwd.name.size()){
				name[i] = m_currentPwd.name[i];
			}
			if(i < m_currentPwd.pwd.size()){
				pwd[i] = m_currentPwd.pwd[i];
			}
			if(i < m_currentPwd.usr.size()){
				usr[i] = m_currentPwd.usr[i];
			}
		}
		pwd_list_add_pwd(name, pwd, usr);
	}
}

void Command::ask_new()
{
	std::cout << "Name : ";
	std::cin >> m_currentPwd.name;
	std::cout << "Password : ";
	std::cin >> m_currentPwd.pwd;
	std::cout << "User name : ";
	std::cin >> m_currentPwd.usr;
	m_currentPwd.op = PasswordInfo::Create;
}

void Command::ask_which()
{
	std::cout << "Password id : ";
	std::cin >> m_currentPwd.id;
}

void Command::ask_what()
{
	std::cout << "Change : \n"
				 "\t[1] Name\n"
				 "\t[2] Password\n"
				 "\t[3] Usr name\n";
	int choice;
	std::cin >> choice;
	m_currentPwd.op = static_cast<PasswordInfo::Operation>(choice);
}