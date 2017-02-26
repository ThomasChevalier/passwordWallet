#include "FileHelper.h"

uint16_t readWord(std::ifstream& file)
{
	uint16_t word;
	uint8_t *byte = reinterpret_cast<uint8_t*>(&word);
	byte[0] = file.get();
	byte[1] = file.get();
	return word;
}