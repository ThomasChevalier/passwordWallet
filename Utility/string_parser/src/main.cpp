/*
 * Parser for string xml file.
 * Written by Thomas Chevalier.
 * Copyright - Thomas Chevalier - 2017.
 * thomasom.chevalier@gmail.com
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cctype>

#include "tinyxml2.h"

#define PROGMEM
#include "../font.h"

#define SCREEN_WIDTH (128)

struct String
{
	std::string modName;
	std::string name;
	std::string content;
};

unsigned get_width_char(char c)
{
	if(c == ' ')
	{
		return 2;
	}
	unsigned size = 0;
	bool searchSpace = true;
	for(int i = FONT_WIDTH; i > 0; --i)
	{
		uint8_t font_byte = font[(c-' ')*FONT_WIDTH+(i-1)];

		// Skip space in the end of the character
		if(font_byte != 0 || !searchSpace)
		{
			searchSpace = false;
			++size;
		}
	}
	return size;
}

unsigned get_width(std::string str)
{
	unsigned size = 0;
	for(char c : str)
	{
		size += get_width_char(c) + 1;
	}
	return size;
}

struct Command
{
	enum Type
	{
		HELP,
		NO_OUT,
		FILE,
		ERROR
	};

	Type type;
	std::string arg;

	Command(Type t, const std::string& str):
		type(t), arg(str)
	{}
};

void show_help()
{
	std::cout << "string_parser by Thomas Chevalier.\n"
				 "Copyright 2017 Thomas Chevalier - All rights reserved.\n"
				 "thomasom.chevalier@gmail.com\n"
				 "Compiled on " __DATE__ " at " __TIME__ "\n"
				 "Usage : string_parser [options]\n"
				 "\toptions :\n"
				 "\t  --help = Display this notice.\n"
				 "\t  --noOut = Don't create a file after the parsing of the xml.\n"
				 "\t  --file [fileName] = Give the file name of the file to parse."
				 << std::endl;
	return;
}

std::vector<Command> parse_command(int argc, char* argv[])
{
	std::vector<Command> commands;
	if(argc < 2)
	{
		commands.push_back({Command::HELP, std::string()});
	}
	for(int i (1); i < argc; ++i)
	{
		std::string cmd = argv[i];
		if(cmd == "--help")
		{
			commands.push_back({Command::HELP, std::string()});
		}
		else if(cmd == "--noOut")
		{
			commands.push_back({Command::NO_OUT, std::string()});
		}
		else if(cmd == "--file")
		{
			if(i+1 < argc)
			{
				if(argv[i+1][0] != '-')
				{
					commands.push_back({Command::FILE, argv[i+1]});
					++i; // Skip file.
				}
				else
				{
					std::ostringstream oss;
					oss << "invalid argument " << i+1<< ", expected file name but command provided.";
					commands.push_back({Command::ERROR, oss.str()});
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "invalid argument " << i+1<< ", expected file name but no argument provided.";
				commands.push_back({Command::ERROR, oss.str()});
			}
		}
		else
		{
			commands.push_back({Command::ERROR, "command \"" + cmd + "\" is unknown."});
		}
	}
	return commands;
}

int main(int argc, char* argv[])
{
	std::vector<Command> com = parse_command(argc, argv);

	// First check for help request
	for(Command c : com)
	{
		if(c.type == Command::HELP)
		{
			show_help();
			return 0;
		}
	}

	bool noOut = false;
	std::string file;

	bool error = false;

	for(Command c : com)
	{
		if(c.type == Command::NO_OUT)
		{
			noOut = true;
		}
		else if(c.type == Command::FILE)
		{
			file = c.arg;
		}
		else if(c.type == Command::ERROR)
		{
			error = true;
			std::cerr << "Error : " << c.arg << '\n';
		}
	}
	if(error){
		return 1;
	}
	if(file.empty()){
		std::cerr << "Error : no file provided.\n";
		return 1;
	}
	std::cout << "Loading file " << file << std::endl;
	tinyxml2::XMLDocument doc;
	if(doc.LoadFile(file.c_str()) != 0)
	{
		std::cout << "Error while loading file.\n";
		doc.PrintError();
		return 1;
	}
	tinyxml2::XMLNode* root = doc.FirstChildElement("root");
	if(doc.Error())
	{
		std::cout << "Error while parsing element.\n";
		doc.PrintError();
		return 1;
	}
	else if(root == nullptr)
	{
		std::cout << "Error : bad document (no root node).\n";
		return 1;
	}

	tinyxml2::XMLNode* mod = root->FirstChild();
	if(doc.Error())
	{
		std::cout << "Error while parsing element.\n";
		doc.PrintError();
		return 1;
	}

	std::map<std::string, std::vector<String> > moduleAndStr;

	while(mod != nullptr)
	{
		if(std::string(mod->Value()) != "module")
		{
			std::cout << "Error : bad document (module node expected and get " << mod->Value() << " instead).\n";
			return 1;
		}
		std::string modName = mod->ToElement()->Attribute("name");
		std::vector<String>& modStr = moduleAndStr[modName];
		std::cout << modName << " : " << std::endl;

		tinyxml2::XMLNode* strNode = mod->FirstChild();
		while(strNode != nullptr)
		{
			if(std::string(strNode->Value()) != "str")
			{
				std::cout << "Error : bad document (str node expected and get " << mod->Value() << " instead).\n";
				return 1;
			}
			tinyxml2::XMLElement* strElem = strNode->ToElement();
			if(strElem == nullptr)
			{
				std::cout << "Error : invalid pointer strElem\n";
				return 1;
			}

			String str;
			str.modName = modName;

			const char* strName_p = strElem->Attribute("name");
			if(strName_p == nullptr)
			{
				std::cout << "Error : attribute \"name\" does not exist.\n";
				return 1;
			}
			str.name = strName_p;
			for(char c : str.name)
			{
				if( (c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c != '_'))
				{
					std::cout << "[ERROR] Invalid character \'" << c << "\' in name \"" << str.name << "\".\n"; 
				}
			}

			str.content = strElem->GetText();

			std::cout << "\t" << std::setw(17) << std::left << str.name << "= \"" << str.content << "\"\n";
			modStr.push_back(str);

			strNode = strNode->NextSibling();
		}

		std::cout << std::endl;

		mod = mod->NextSibling();
	}

	// Check verif
	std::vector<std::string> allNames;
	std::vector<std::string> allContents;
	for(auto it = moduleAndStr.begin(); it != moduleAndStr.end(); ++it)
	{
		std::string modName = it->first;
		auto vec = it->second;
		for(String s : vec)
		{
			if(s.name.empty())
			{
				allNames.push_back("str_" + modName);
			}
			else
			{
				allNames.push_back("str_" + modName + "_" + s.name);
			}
			allContents.push_back(s.content);
		}
	}
	std::vector<std::string> warnStrings;
	for(auto str : allNames)
	{
		if(std::find(warnStrings.begin(), warnStrings.end(), str) != warnStrings.end())
		{
			continue;
		}
		auto num = std::count(allNames.begin(), allNames.end(), str);
		if( num !=  1)
		{
			std::cout << "[WARNING] " << num << " strings have the name \"" << str << "\"" << std::endl;
			warnStrings.push_back(str);
		}
	}
	warnStrings.clear();

	for(auto str : allContents)
	{
		if(std::find(warnStrings.begin(), warnStrings.end(), str) != warnStrings.end())
		{
			continue;
		}
		auto num = std::count(allContents.begin(), allContents.end(), str);
		if( num !=  1)
		{
			std::cout << "[WARNING] " << num << " strings have the same content \"" << str << "\"" << std::endl;
			warnStrings.push_back(str);
		}
	}

	if(noOut)
	{
		std::cout << "No output option specified.\nExit program.\n";
		return 0;
	}

	std::ofstream outFile("String.h");
	if(!outFile)
	{
		std::cout << "Error : can't create file String.h.\n";
		return 1;
	}
	// Copyright notice
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	outFile << "/*\n"
				  " * File auto-generated by string_parser.\n"
				  " * Copyright - Thomas Chevalier - 2017.\n"
				  " * thomasom.chevalier@gmail.com\n"
				  " * File created on " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "\n"
				  "*/\n\n";
	outFile << "#ifndef STRING_HEADER_THOMAS_CHEVALIER\n"
	           "#define STRING_HEADER_THOMAS_CHEVALIER\n\n"
	           "#include <avr/pgmspace.h>\n\n";

	int index = 0;
	std::vector<unsigned char> allData;

	for(auto it = moduleAndStr.begin(); it != moduleAndStr.end(); ++it)
	{
		std::string modName = it->first;
		auto vec = it->second;
		for(String s : vec)
		{
			std::string strName;
			if(s.name.empty())
			{
				strName = "str_" + modName;
			}
			else
			{
				strName = "str_" + modName + "_" + s.name;
			}
			outFile << "#define " << strName << "_index (" << index << ")\n";
			index+=s.content.size()+1;
			outFile << "#define " << strName << "_len (" << s.content.size() << ")\n";

			unsigned centerX = SCREEN_WIDTH;
			const unsigned widthStr = get_width(s.content);

			outFile << "#define " << strName << "_pixLen (" << widthStr << ")\n";
			
			if(widthStr > centerX){
				centerX = 0;
			}
			else{
				centerX -= widthStr;
				centerX /= 2;
			}

			outFile << "#define " << strName << "_centerX (" << centerX << ")\n";
			for(char c : s.content)
			{
				allData.push_back(c);
			}
			allData.push_back(0);
		}
	}

	outFile << "\nstatic const uint8_t stringData[" << allData.size() << "] PROGMEM =\n{\n";
	outFile << std::hex;
	int count = 0;
	for(char c : allData)
	{
		if(c==0)
		{
			outFile << "\'\\0\', ";
		}
		else
		{		
			outFile << "\'";
			if(c == '\'' || c == '\\' || c == '\"')
			{
				outFile << "\\";
			}
			outFile << c << "\', ";
		}

		count++;
		if(count == 16)
		{
			count = 0;
			outFile << "\n";
		}
	}
	outFile << "};\n\n";

	outFile << "#endif // STRING_HEADER_THOMAS_CHEVALIER\n";

	std::cout << "string_parser done. Thank you."<< std::endl;
}