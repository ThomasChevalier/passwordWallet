#include "Security.h"

#include "Random.h"

void security_init(void)
{
	random_init();
}

void security_erase_data(uint8_t *buffer, uint8_t lenght)
{
	volatile uint8_t *buf;
   
	for (buf = (volatile uint8_t *)buffer;  lenght;  buf[--lenght] = 0);
}