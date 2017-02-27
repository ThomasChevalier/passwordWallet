#ifndef INFO_HEADER_THOMAS_CHEVALIER
#define INFO_HEADER_THOMAS_CHEVALIER

#include <cstdint>
#include <ostream>
#include <fstream>

struct Info
{
	Info();
	Info(std::ifstream& file);

	uint8_t options_flag;
	uint8_t first_pwd_util;
	uint8_t first_pwd_alpha;
	uint8_t num_pwd;
	uint16_t pool_size;
	uint8_t pool[256];
	uint8_t memory_map[6];

	void readFromFile(std::ifstream& file);

	void print(std::ostream& stream) const;

};

std::ostream& operator<<( std::ostream &stream, const Info & info );

#endif // INFO_HEADER_THOMAS_CHEVALIER
