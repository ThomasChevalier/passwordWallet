#include <iostream>
#include <string>

#include "MemoryRepresentation.h"
#include "MemoryAccess.h"

int main()
{
	MemoryRepresentation rep;
	if(!rep.loadFromFile("../../Code/memorg.xml"))
	{
		std::cerr << "Error while loading :\"" << rep.errorString() << "\"\n";
	}
	// std::cout << "File : ";
	// std::string fileName;
	// std::cin >> fileName;
	// std::cout << "Key :";
	// std::string key;
	// std::cin >> key;

	MemoryAccess mem(rep);
	mem.loadFromFile("../../../../fram.hex");
	mem.setKey("aaaaaaaaaaaaaaaaaaaa");

	for(Password &p : mem.allPasswords())
	{
		std::cout << p.name << " | " << p.password << " | " << p.userName << "|" <<  p.counter << std::endl;
	}
}
