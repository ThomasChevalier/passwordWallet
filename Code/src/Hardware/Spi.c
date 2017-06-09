#include "Spi.h"
#include "PinDefinition.h"

//Initialize SPI hardware

void spi_setup_hardware (void)
{
	// Set up SCK as output
	SPI_SCK_DDR |= (1<<SPI_SCK_PIN_NUM);
	// Set up MOSI as output
	SPI_MOSI_DDR |= (1<<SPI_MOSI_PIN_NUM);

	// Set up SS as output
	// Because spi is master mode, PB0 (aka Slave Select pin) should be an output, or, if it is an input, should be in high state
	SPI_SS_DDR |= (1<<SPI_SS_PIN_NUM);

	// Set up MISO as input
	SPI_MISO_DDR &= ~(1<<SPI_MISO_PIN_NUM);
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