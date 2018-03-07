#ifndef FRAM_HEADER_THOMAS_CHEVALIER
#define FRAM_HEADER_THOMAS_CHEVALIER

#include <cstdint>

#define f_addr_t uint32_t
#define f_addr_s 3

void fram_init(uint8_t* dest);

uint8_t	fram_read_byte		(f_addr_t addr);

void	fram_read_bytes		(f_addr_t addr, uint8_t* buffer, uint8_t size);

void	fram_write_byte		(f_addr_t addr, uint8_t byte);

void	fram_write_bytes	(f_addr_t addr, uint8_t* buffer, uint8_t size);

void	fram_set			(f_addr_t addr, uint8_t val, uint8_t num);


#endif // FRAM_HEADER_THOMAS_CHEVALIER