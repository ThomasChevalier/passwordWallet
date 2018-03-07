#include "Fram.h"

static uint8_t* f_mem = nullptr;

void fram_init(uint8_t* dest)
{
	f_mem = dest;
}

uint8_t	fram_read_byte		(f_addr_t addr)
{
	return f_mem[addr];
}

void	fram_read_bytes		(f_addr_t addr, uint8_t* buffer, uint8_t size)
{
	for(unsigned i(0); i < size; ++i){
		buffer[i] = f_mem[addr + i];
	}
}

void	fram_write_byte		(f_addr_t addr, uint8_t byte)
{
	f_mem[addr] = byte;
}

void	fram_write_bytes	(f_addr_t addr, uint8_t* buffer, uint8_t size)
{
	for(unsigned i(0); i < size; ++i){
		f_mem[addr+i] = buffer[i];
	}
}

void	fram_set			(f_addr_t addr, uint8_t val, uint8_t num)
{
	for(unsigned i(0); i < num; ++i){
		f_mem[addr+i] = val;
	}
}