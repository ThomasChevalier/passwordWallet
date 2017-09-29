#include "Spi.h"
#include "PinDefinition.h"

//Initialize SPI hardware

void spi_init (void)
{
	// Setup spi configuration
	// This config is the same for all the devices at 8 MHz
	SPCR = SPI_ENABLE | SPI_MASTER;
	SPSR |= (1<<SPI2X); // Active 2x speed mode
}

uint8_t spi_transfer_8(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & _BV(SPIF))) ;
	return SPDR;
}

void spi_send(uint8_t * dataout, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; ++i)
	{
		SPDR = dataout[i];
		while (!(SPSR & _BV(SPIF)));
	}
}

void spi_send_8(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
}

void spi_read(uint8_t* buffer, uint8_t size)
{
	uint8_t i = 0;
	for(; i < size; ++i)
	{
		SPDR = 0x00;
		while (!(SPSR & _BV(SPIF)));
		buffer[i] = SPDR;
	}
}

uint8_t spi_read_8(void)
{
	return spi_transfer_8(0x00);
}

void spi_set (uint8_t val, uint8_t num)
{
	uint8_t i;
	for (i = 0; i < num; ++i)
	{
		SPDR = val;
		while (!(SPSR & _BV(SPIF)));
	}
}
