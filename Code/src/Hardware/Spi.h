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

#define SPI_MASTER 0x10
#define SPI_ENABLE 0x40

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

/**
 * @brief Setup register for SPI.
 */
void	spi_init	(void);

/**
 * @brief Send a byte via the spi port and read one.
 * 
 * @param data The byte to send.
 * @return The byte that the slave send.
 */
uint8_t spi_transfer_8		(uint8_t data);

/**
 * @brief Shift full array to target device without receiving any byte.
 * 
 * @param dataout The array of bytes to send.
 * @param len The lenght of the array.
 */
void	spi_send			(uint8_t * dataout, uint8_t len);

/**
 * @brief Write one byte on the bus without receiving anything.
 * 
 * @param data The byte to send.
 */
void	spi_send_8			(uint8_t data);

/**
 * @brief Read an array of byte from spi bus.
 * 
 * @param buffer The array that contains the content read.
 * @param size How many bytes to read.
 */
void	spi_read			(uint8_t* buffer, uint8_t size);

/**
 * @brief Read one byte from the bus.
 * @details Send a dummy byte (0x00). Same as spi_transfer_8(0x00).
 * @return The byte that the slave send.
 */
uint8_t spi_read_8			(void);

/**
 * @brief Send several times a byte on the spi bus.
 * 
 * @param val The byte to send several times.
 * @param num How many times the byte must be sent.
 */
void	spi_set				(uint8_t val, uint8_t num);

#endif // SPI_HEADER_THOMAS_CHEVALIER
