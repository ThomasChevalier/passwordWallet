#ifndef PASSWORD_HEADER_THOMAS_CHEVALIER
#define PASSWORD_HEADER_THOMAS_CHEVALIER

#include <cstdint>
#include <string>


struct Password
{
	typedef uint8_t id_t;

	Password();

	id_t id;
	uint16_t counter;
	std::string name;
	std::string password;
	std::string userName;
};

#endif // PASSWORD_HEADER_THOMAS_CHEVALIER