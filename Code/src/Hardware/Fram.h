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

void	fram_setup_hardware	(void);												// Setup pin direction for fram
void	fram_setup_spi		(void);												// Setup spi mode and clock
uint8_t	fram_read_byte		(uint16_t addr);									// Read one byte of memory at addr
void	fram_read_bytes		(uint16_t addr, uint8_t* buffer, uint8_t size);		// Read bytes of memory from addr to addr + size
void	fram_write_byte		(uint16_t addr, uint8_t byte);						// Write one byte in memory at addr
void	fram_write_bytes	(uint16_t addr, uint8_t* buffer, uint8_t size);		// Write bytes in memory from addr to addr + size
void	fram_set			(uint16_t addr, uint8_t val, uint8_t num);			// Write num times the value val in memory beginning at addr
Fram_id	fram_read_id		(void);												// Read product id, see Fram_id struct declaration
uint8_t	fram_read_register	(void);												// Read the status register
void	fram_write_register	(uint8_t reg);										// Write the status register, bit 1 and 0 are ignored
void	fram_select			(void);												// Pull cs down
void	fram_deselect		(void);												// Pull cs up
uint8_t fram_test			(void);												// Test if the fram is installed. Return 1 if OK, return 0 if fram is not installed

#endif // FRAM_HEADER_THOMAS_CHEVALIER