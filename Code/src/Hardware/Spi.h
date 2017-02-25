#ifndef SPI_HEADER_THOMAS_CHEVALIER
#define SPI_HEADER_THOMAS_CHEVALIER

#include <avr/io.h>

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

void	spi_setup_hardware	(void);													//Initialize SPI Master Device
uint8_t spi_transfer_8		(uint8_t data);											// Write 8 bit on bus and receive 8
void	spi_send			(uint8_t * dataout, uint8_t len);						// Shift full array to target device without receiving any byte
void	spi_send_8			(uint8_t data);											// Write 8 bit on bus
void	spi_read			(uint8_t* buffer, uint8_t size);						// Read size bytes from spi bus
uint8_t spi_read_8			(void);													// Read one byte
void	spi_set				(uint8_t val, uint8_t num);								// Send num times the value val to the device

#endif // SPI_HEADER_THOMAS_CHEVALIER
