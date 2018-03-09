#include "Random.h"

#include <random>
#include <stdexcept>

uint8_t random_request_byte (void)
{
	std::random_device rd("/dev/urandom");
	std::uniform_int_distribution<int> distribution(0,255);
	return distribution(rd);
}

uint8_t random_request_printable (void)
{
	std::random_device rd("/dev/urandom");
	std::uniform_int_distribution<int> distribution(' ','~'-1);
	return distribution(rd);
}