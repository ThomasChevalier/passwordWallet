/*
 * Parser for pin definition xml file.
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

struct Pin
{
	enum Dir
	{
		OUTPUT,
		INPUT
	};
	enum State
	{
		HIGH,
		LOW
	};

	std::string modName;
	std::string name;
	bool pinChange;
	char reg;
	int num;
	Dir direction;
	State defaultState;
};

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
	std::cout << "pindef_parser by Thomas Chevalier.\n"
				 "Copyright 2017 Thomas Chevalier - All rights reserved.\n"
				 "thomasom.chevalier@gmail.com\n"
				 "Compiled on " __DATE__ " at " __TIME__ "\n"
				 "Usage : pindef_parser [options]\n"
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
					oss << "invalid argument " << i+1<< ", expected file name but no command provided.";
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

	std::map<std::string, std::vector<Pin> > moduleAndPin;

	while(mod != nullptr)
	{
		if(std::string(mod->Value()) != "module")
		{
			std::cout << "Error : bad document (module node expected and get " << mod->Value() << " instead).\n";
			return 1;
		}
		std::string modName = mod->ToElement()->Attribute("name");
		std::vector<Pin>& modPins = moduleAndPin[modName];
		
		tinyxml2::XMLNode* pinNode = mod->FirstChild();
		while(pinNode != nullptr)
		{
			if(std::string(pinNode->Value()) != "pin")
			{
				std::cout << "Error : bad document (pin node expected and get " << mod->Value() << " instead).\n";
				return 1;
			}
			tinyxml2::XMLElement* pinElem = pinNode->ToElement();
			if(pinElem == nullptr)
			{
				std::cout << "Error : invalid pointer pinElem\n";
				return 1;
			}

			Pin pin;
			pin.modName = modName;

			const char* pinName_p = pinElem->Attribute("name");
			if(pinName_p == nullptr)
			{
				std::cout << "Error : attribute \"name\" does not exist.\n";
				return 1;
			}
			pin.name = pinName_p;

			const char* reg_p = pinElem->Attribute("register");
			if(reg_p == nullptr)
			{
				std::cout << "Error : attribute \"register\" does not exist.\n";
				return 1;
			}
			std::string reg = reg_p;
			if(reg.size() != 1)
			{
				std::cout << "Error : bad register description \"" << reg_p << "\".\n";
				return 1;
			}
			pin.reg = toupper(reg[0]);

			int pinN = 0;
			if(pinElem->QueryIntAttribute("num", &pinN) != 0)
			{
				std::cout << "Error : bad attribute \"num\"\n";
				return 1;
			}
			pin.num = pinN;

			const char* dir_p = pinElem->Attribute("dir");
			if(dir_p == nullptr)
			{
				std::cout << "Error : attribute \"dir\" does not exist.\n";
				return 1;
			}
			std::string dir = dir_p;
			if(dir == "OUTPUT")
			{
				pin.direction = Pin::OUTPUT;
			}
			else if(dir == "INPUT")
			{
				pin.direction = Pin::INPUT;
			}
			else
			{
				std::cout << "Error : wrong value for attribute \"dir\" : \"" << dir << "\".\n";
				return 1;
			}

			bool pinChange = false;
			if(pinElem->QueryBoolAttribute("pinChangeInt", &pinChange) == 0)
			{
				pin.pinChange = pinChange;
			}
			else
			{
				pin.pinChange = false;
			}

			const char* default_p = pinElem->Attribute("default");
			std::string def;
			if(default_p == nullptr)
			{
				def = "LOW";
			}
			else
			{
				def = default_p;
			}
			if(def == "HIGH")
			{
				pin.defaultState = Pin::HIGH;
			}
			else if(def == "LOW")
			{
				pin.defaultState = Pin::LOW;
			}
			else
			{
				std::cout << "Error : wrong value for attribute \"default\" : \"" << def << "\".\n";
				return 1;
			}

			std::cout << "Pin " << std::setw(6) << std::right << pin.modName << "/" << std::setw(6) << std::left << pin.name << " [Reg = " << pin.reg << "; Num = " << pin.num << "; Dir = " << (pin.direction == Pin::OUTPUT ? "O" : "I") << "; Default = " << (pin.defaultState == Pin::HIGH ? "H" : "L") <<  "]\n";
			modPins.push_back(pin);

			pinNode = pinNode->NextSibling();
		}

		std::cout << std::endl;

		mod = mod->NextSibling();
	}

	// Check verif
	std::map<char, std::vector<int> > regNumMap;
	bool warn = false;
	for (auto it= moduleAndPin.begin(); it != moduleAndPin.end(); ++it)
	{
		std::vector<Pin> allRegPins = it->second;
		for(Pin p : allRegPins)
		{
			std::vector<int>& pins = regNumMap[p.reg];
			if(std::find(pins.begin(), pins.end(), p.num) != pins.end())
			{
				warn = true;
				std::cout << "[WARNING] - Pin " << p.num << " in register " << p.reg << " is used several times.\n";
			}
			pins.push_back(p.num);
		}
	}
	if(!warn)
	{
		std::cout << "All test passed. Everything is OK.\n";
	}

	if(noOut)
	{
		return 0;
	}

	// Construct file
	std::ofstream outFile("PinDefinition.h");
	if(!outFile)
	{
		std::cout << "Error : can't create file PinDefinition.h.\n";
		return 1;
	}

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	// Copyright notice
	outFile << "/*\n"
				  " * File generated by pindef_parser.\n"
				  " * Copyright - Thomas Chevalier - 2017.\n"
				  " * thomasom.chevalier@gmail.com\n"
				  " * File created on " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "\n"
				  "*/\n\n";


	// Header
	outFile << "#ifndef PINDEFINITION_HEADER_THOMAS_CHEVALIER\n"
	           "#define PINDEFINITION_HEADER_THOMAS_CHEVALIER\n\n";

	// Start writing pin def
	std::map<char, std::vector<Pin> > regPinMap;
	
	for (auto it= moduleAndPin.begin(); it != moduleAndPin.end(); ++it)
	{
		outFile << "//" << it->first << "\n";
		std::vector<Pin> allRegPins = it->second;
		for(Pin p : allRegPins)
		{
			std::vector<Pin>& allPinsInReg = regPinMap[p.reg];
			allPinsInReg.push_back(p);

			std::string completeName = p.modName;
			if(!p.name.empty())
			{
				completeName += "_" + p.name;
			}
			outFile << "#define " << completeName + "_PIN_NUM "  << p.num << "\n";
			outFile << "#define " << completeName + "_DDR DDR"   << p.reg << "\n";
			outFile << "#define " << completeName + "_PORT PORT" << p.reg << "\n";
			outFile << "#define " << completeName + "_PIN PIN"   << p.reg << "\n";
			if(p.pinChange)
			{
				outFile << "#define " << completeName + "_PIN_CHANGE_NUM PCINT" << p.num << "\n";
			}
			outFile<<"\n";
		}
		outFile<<"\n";
	}
	outFile << "// INIT VALUE\n";
	for (auto it = regPinMap.begin(); it != regPinMap.end(); ++it)
	{
		outFile << "#define DDR" << it->first << "_INIT_VALUE (";
		for(Pin p : it->second)
		{
			std::string completeName = p.modName;
			if(!p.name.empty())
			{
				completeName += "_" + p.name;
			}
			if(p.direction == Pin::OUTPUT)
			{
				outFile << "(1 << " << completeName + "_PIN_NUM) | ";
			}
		}
		outFile << "0)\n";

		outFile << "#define PORT" << it->first << "_INIT_VALUE (";
		for(Pin p : it->second)
		{
			std::string completeName = p.modName;
			if(!p.name.empty())
			{
				completeName += "_" + p.name;
			}
			if(p.defaultState == Pin::HIGH)
			{
				outFile << "(1 << " << completeName + "_PIN_NUM) | ";
			}
		}
		outFile << "0)\n\n";
	}


	outFile << "#define INIT_ALL_DDR ";

	for (auto it = regPinMap.begin(); it != regPinMap.end(); ++it)
	{
		outFile << "DDR" << it->first << " = DDR" << it->first << "_INIT_VALUE; \\\n";
	}
	outFile << "\n#define INIT_ALL_PORT ";
	for (auto it = regPinMap.begin(); it != regPinMap.end(); ++it)
	{
		outFile << "PORT" << it->first << " = PORT" << it->first << "_INIT_VALUE; \\\n";
	}
	outFile << "\n\n";
	// Close header
	outFile << "#endif // PINDEFINITION_HEADER_THOMAS_CHEVALIER\n";
}