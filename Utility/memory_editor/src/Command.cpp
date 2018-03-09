#include "Command.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <iomanip>
#include <limits>

#include "dependency/Globals.h"
#include "dependency/Security/Password_List.h"
#include "dependency/Security/Password.h"

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
	valid_str[Type::Reencrypt] = "reencrypt";

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

static void ascii85_decode(const std::string& str, uint8_t *decoded)
{
	const char *input= str.c_str();

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
		decoded[0+pos] = sum >> 24;
		decoded[1+pos] = (sum & 0xFF0000) >> 16;
		decoded[2+pos] = (sum & 0xFF00) >> 8;
		decoded[3+pos] = sum & 0xFF;

		input += 5;
		pos += 4;
	}
}

void Command::exec()
{
	if(m_type == Type::New){
		uint8_t name[32] = {0};
		uint8_t pwd[32]  = {0};
		uint8_t usr[64]  = {0};


		for(int i(0); i < 64; ++i){
			if(i < m_currentPwd.name.size() && i < 32){
				name[i] = m_currentPwd.name[i];
			}
			if(i < m_currentPwd.pwd.size() && i < 32){
				pwd[i] = m_currentPwd.pwd[i];
			}
			if(i < m_currentPwd.usr.size() && i < 64){
				usr[i] = m_currentPwd.usr[i];
			}
		}
		pwd_list_add_pwd(name, pwd, usr);
	}
	else if(m_type == Type::Show)
	{
		if(NUM_PWD == 0){
			std::cout << "No password." << std::endl;
		}else{
			std::cout << NUM_PWD << " passwords" << std::endl;
			const p_addr first = pwd_list_get_first_pwd_id_sort_none();
			p_addr current = first;
			do
			{
				uint8_t name_raw[32];
				password_read_name(current, name_raw);
				std::string name(reinterpret_cast<char*>(name_raw), strlen_bound(reinterpret_cast<char*>(name_raw), 32));

				uint8_t pwd_raw[32];
				password_read_data(current, pwd_raw, KEY);
				std::string pwd(reinterpret_cast<char*>(pwd_raw), strlen_bound(reinterpret_cast<char*>(pwd_raw), 32));

				uint8_t usr_raw[64];
				password_read_usr_name(current, usr_raw, KEY);
				std::string usr(reinterpret_cast<char*>(usr_raw), strlen_bound(reinterpret_cast<char*>(usr_raw), 64));

				std::cout << "\t[" << std::setw(3) << current << "] " << name << " | " << pwd << " | " << usr << std::endl;
				current = pwd_list_get_next_pwd_id_sort_none(current);
			}while(current != first);
		}
	}
	else if(m_type == Type::Delete)
	{
		pwd_list_delete_pwd(m_currentPwd.id);
	}
	else if(m_type == Type::Change)
	{

		std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
		if(m_currentPwd.op == PasswordInfo::ChangeName)
		{
			uint8_t name_raw[32] = {0};
			std::cout << "Name : ";
			std::string name;
			std::getline(std::cin, name);
			for(int i(0); i < name.size() && i < 32; ++i){
				name_raw[i] = name[i];
			}
			password_set_name(m_currentPwd.id, name_raw);
		}
		else if(m_currentPwd.op == PasswordInfo::ChangePwd)
		{
			uint8_t pwd_raw[32] = {0};
			std::cout << "Password : ";
			std::string pwd;
			std::getline(std::cin, pwd);
			for(int i(0); i < pwd.size() && i < 32; ++i){
				pwd_raw[i] = pwd[i];
			}
			password_set_data(m_currentPwd.id, pwd_raw, KEY);
		}
		else if(m_currentPwd.op == PasswordInfo::ChangeUsr)
		{
			uint8_t usr_raw[64] = {0};
			std::cout << "User name : ";
			std::string usr;
			std::getline(std::cin, usr);
			for(int i(0); i < usr.size() && i < 64; ++i){
				usr_raw[i] = usr[i];
			}
			password_set_usr_name(m_currentPwd.id, usr_raw, KEY);
		}
	}
	else if(m_type == Type::Reencrypt)
	{
		if(NUM_PWD == 0){
			std::cout << "No password." << std::endl;
			return;
		}
		std::string newKey_85;
		std::cout << "Enter new key : ";
		std::cin >> newKey_85;
		if(newKey_85.size()!=20){
			std::cout << "Wrong size." << std::endl;
			return;
		}
		uint8_t newKey[16];
		ascii85_decode(newKey_85, newKey);

		const p_addr first = pwd_list_get_first_pwd_id_sort_none();
		p_addr current = first;
		do
		{

			uint8_t buffer[64] = {0};
			// ... Change the key
			password_read_data(current, buffer, KEY);
			password_set_data (current, buffer, newKey);

			password_read_usr_name(current, buffer, KEY);
			password_set_usr_name (current, buffer, newKey);

			current = pwd_list_get_next_pwd_id_sort_none(current);
		}while(current != first);
		for(int i(0); i < 16; ++i){
			KEY[i] = newKey[i];
		}
	}
}

void Command::ask_new()
{
	std::cout << "Name : ";

	std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
	std::getline(std::cin, m_currentPwd.name);
	std::cout << "Password : ";
	std::getline(std::cin, m_currentPwd.pwd);
	std::cout << "User name : ";
	std::getline(std::cin, m_currentPwd.usr);
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