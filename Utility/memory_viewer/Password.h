#ifndef PASSWORD_HEADER_THOMAS_CHEVALIER
#define PASSWORD_HEADER_THOMAS_CHEVALIER

#include <cstdint>
#include <ostream>
#include <fstream>
#include <cstring>

struct Password
{

	Password(uint8_t id_, std::ifstream& file);

	uint8_t prev_pwd_util;
	uint8_t next_pwd_util;
	uint8_t prev_pwd_alpha;
	uint8_t next_pwd_alpha;
	uint16_t count;
	char name [32];
	uint8_t iv_pwd[16];
	uint8_t pwd[32];
	uint8_t iv_usr[16];
	uint8_t usr[64];

	uint8_t id;

	void readFromFile();

	void print(std::ostream& stream) const;

private:
	std::ifstream& m_file;
};

std::ostream& operator<<( std::ostream &stream, const Password & pwd );

#endif // PASSWORD_HEADER_THOMAS_CHEVALIER
