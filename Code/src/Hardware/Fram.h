/*

Copyright 2017 Thomas Chevalier
All rights reserved

*/

#ifndef FRAM_HEADER_THOMAS_CHEVALIER
#define FRAM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/io.h>


// See the fram datasheet : http://edevice.fujitsu.com/fj/DATASHEET/e-ds/MB85RS256B-DS501-00021-4v0-E.pdf

/*
	   FRAM PINOUT
		   ______
	 CS  -|      |- VDD
	 SO  -| Fram |- HOLD = HIGH
HIGH=WP  -|      |- SCK
	 GND -|______|- SI


	CS : Child Select -> Low level to select the device, high to deselect
	SO : Slave Out
	WP : Write Protect -> Control writing to status register, not used, should not be NC, put it HIGH if you want unprotected writing in all cases, otherwise put it LOW
	GND: Ground pin
	VDD: Supply voltage pin
	HOLD: Used to interrupt operation, not used, should be HIGH
	SCK: Serial Clock Pin
	SI : Slave In
*/

// WARNING : it is to the user to disable interrupt before any spi operation

typedef enum
{
	F_WREN = 0x06, // Set write enable latch
	F_WRDI = 0x04, // Reset write enable latch
	F_RDSR = 0x05, // Read status register
	F_WRSR = 0x01, // Write status register
	F_READ = 0x03, // Read memory code
	F_WRITE = 0x02,// Write memory code
	F_RDID = 0x9F, // Read device id
	F_FSTRD = 0x0B // Fast read memory code (not used)
} FRAM_OPCODE;

typedef struct Fram_id_
{
	uint8_t manufacturer_id;	// Manufacturer ID : should be 0x04 for Fujistu
	uint8_t continuation_code;	// Continuation code : should be 0x7F
	uint8_t product_idL;		// First byte of product ID : the bit 0-4 are the density code, bit 5-7 are for proprietary use
	// Density code : 0x01 is 16 kbit / 0x03 is 64 kbit / 0x04 is 128 kbit / 0x05 is 256kbit / 0x06 is 512kbit / 0x07 is 1Mbit / 0x08 is 2Mbit / 0x09 is 4 Mbit
	uint8_t product_idH;		// Second byte of product ID : proprietary use only
} Fram_id;

/**
 * @brief Setup pin direction for fram.
 */
void	fram_setup_hardware	(void);

/**
 * @brief Setup spi mode and clock speed.
 */
void	fram_setup_spi		(void);

/**
 * @brief Read one byte of memory.
 * 
 * @param addr The address to read the byte from.
 * @return The byte that has been read.
 */
uint8_t	fram_read_byte		(uint16_t addr);

/**
 * @brief Read an array of bytes from memory.
 * 
 * @param addr The address to read the bytes from.
 * @param buffer The array of bytes that will contains the bytes read.
 * @param size The size of the array.
 */
void	fram_read_bytes		(uint16_t addr, uint8_t* buffer, uint8_t size);

/**
 * @brief Write one byte in memory.
 * 
 * @param addr Where to write the byte.
 * @param byte The byte to write.
 */
void	fram_write_byte		(uint16_t addr, uint8_t byte);

/**
 * @brief Write an array of bytes in memory.
 * 
 * @param addr Where to write the bytes.
 * @param buffer The array of bytes to write.
 * @param size The size of the array.
 */
void	fram_write_bytes	(uint16_t addr, uint8_t* buffer, uint8_t size);

/**
 * @brief Write an array of bytes with a fixed value in memory.
 * 
 * @param addr Where to write the bytes.
 * @param val The value of the bytes that will be written.
 * @param num The number of bytes to write.
 */
void	fram_set			(uint16_t addr, uint8_t val, uint8_t num);

/**
 * @brief Read the product identifier.
 * 
 * @return Return a structure with all the memory info. See Fram_id declaration.
 */
Fram_id	fram_read_id		(void);

/**
 * @brief Read the status register of the memory.
 * 
 * @return Return the value of the register.
 */
uint8_t	fram_read_register	(void);

/**
 * @brief Write the status register.
 * @details Bit 0 and 1 are not taken into account.
 * 
 * @param reg The value to write in the register.
 */
void	fram_write_register	(uint8_t reg);

/**
 * @brief Select the memory for spi operation.
 * @details Pull cs low.
 */
void	fram_select			(void);

/**
 * @brief Deselect the memory.
 * @details Pull cs high.
 */
void	fram_deselect		(void);

/**
 * @brief Test if the fram is installed.
 * @details Check the fram identifier against known value.
 * @return Return RETURN_SUCCESS if OK, RETURN_ERROR otherwise.
 */
uint8_t fram_test			(void);

#endif // FRAM_HEADER_THOMAS_CHEVALIER
