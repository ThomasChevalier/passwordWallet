/*

Copyright 2017 Thomas Chevalier
All rights reserved

*/

#include <fstream>
#include <iostream>

#include "Globals.h"
#include "Fram.h"

static unsigned int bytes_read = 0;
static unsigned int bytes_written = 0;

unsigned char FRAM_DATA[FRAM_BYTE_SIZE];

void fram_setup_hardware()
{
	std::ifstream file("fram.dat", std::ifstream::binary);
	file.read(reinterpret_cast<char*>(FRAM_DATA), FRAM_BYTE_SIZE);
}

void fram_setup_spi()
{
}

uint8_t fram_read_byte(uint16_t addr)
{
	++bytes_read;
	return FRAM_DATA[addr];
}

void fram_read_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	bytes_read += size;
	for(int i = 0; i < size ;++i)
	{
		buffer[i] = FRAM_DATA[addr+i];
	}
}

void fram_write_byte(uint16_t addr, uint8_t byte)
{
	++bytes_written;
	FRAM_DATA[addr] = byte;
}

void fram_write_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	bytes_written += size;
	for(int i = 0; i < size; ++i)
	{
		FRAM_DATA[addr+i] = buffer[i];
	}
}

void fram_set (uint16_t addr, uint8_t val, uint8_t num)
{
	bytes_written += num;
	for(int i = 0; i < num; ++i)
	{
		FRAM_DATA[addr+i] = val;
	}
}


uint8_t fram_test (void)
{
	return RETURN_SUCCESS;
}

void fram_end()
{
	std::ofstream file("fram.dat", std::ofstream::binary);
	file.write(reinterpret_cast<char*>(FRAM_DATA), FRAM_BYTE_SIZE);
	std::cout << bytes_read << " read | " << bytes_written << " written" << std::endl;
}