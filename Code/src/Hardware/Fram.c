/*

Copyright 2017 Thomas Chevalier
All rights reserved

*/

#include "../Globals.h"
#include "Fram.h"

#if defined(SPI_FRAM)
#include "Spi.h"
#elif defined(I2C_FRAM)
#include "I2C.h"
#define FRAM_I2C_ADDR (0x50)
#endif

#include "PinDefinition.h"

#if defined(SPI_FRAM)

/**
 * @brief Select the memory for spi operation.
 * @details Pull cs low.
 */
static void fram_select	(void)
{
	FRAM_CS_PORT &= ~(1<<FRAM_CS_PIN_NUM);
}
/**
 * @brief Deselect the memory.
 * @details Pull cs high.
 */
static void fram_deselect (void)
{
	FRAM_CS_PORT |= (1<<FRAM_CS_PIN_NUM);
}

/**
 * @brief Read the status register of the memory.
 * 
 * @return Return the value of the register.
 */
// static uint8_t fram_read_register(void)
// {
//
// 	fram_select();
// 	spi_send_8(F_RDSR);
// 	uint8_t result = spi_read_8();
// 	fram_deselect();
// 	return result;
// }

/**
 * @brief Write the status register.
 * @details Bit 0 and 1 are not taken into account.
 * 
 * @param reg The value to write in the register.
 */
// static void fram_write_register(uint8_t reg)
// {
//
// 	fram_select();
// 	spi_send_8(F_WREN);
// 	fram_deselect();

// 	fram_select();
// 	spi_send_8(F_WRSR);
// 	spi_send_8(reg);
// 	fram_deselect();
// }

#endif // SPI_FRAM

void fram_setup_hardware()
{
	#if defined(I2C_FRAM)
	i2c_init();
	#endif
}
uint8_t fram_read_byte(uint16_t addr)
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_READ);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	uint8_t result = spi_read_8();
	fram_deselect();
	return result;

	#elif defined(I2C_FRAM)

	uint8_t cmd[2];
	cmd[0] = addr >> 8;
	cmd[1] = addr & 0xFF;
	i2c_transmit(FRAM_I2C_ADDR, cmd, 2);
	uint8_t result = 0;
	i2c_receive(FRAM_I2C_ADDR, &result, 1);
	return result;
	#endif // SPI_FRAM
}

void fram_read_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_READ);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);

	spi_read(buffer, size);

	fram_deselect();
	#elif defined(I2C_FRAM)
	for(uint8_t i = 0; i < size; ++i)
	{
		buffer[i] = fram_read_byte(addr+i);
	}
	#endif // SPI_FRAM
}

void fram_write_byte(uint16_t addr, uint8_t byte)
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();
	
	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_send_8(byte);
	fram_deselect();

	#elif defined(I2C_FRAM)
	uint8_t cmd[3];
	cmd[0] = addr >> 8;
	cmd[1] = addr & 0xFF;
	cmd[2] = byte;
	i2c_transmit(FRAM_I2C_ADDR, cmd, 3);
	#endif
}


void fram_write_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();

	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_send(buffer, size);
	fram_deselect();
	#elif defined(I2C_FRAM)
	// Must be improved
	for(int i = 0; i < size; ++i)
	{
		fram_write_byte(addr+i, buffer[i]);
	}
	#endif // SPI_FRAM
}

void fram_set (uint16_t addr, uint8_t val, uint8_t num)
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();

	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_set(val, num);
	fram_deselect();
	#elif defined(I2C_FRAM)
	for(int i = 0; i < num; ++i)
	{
		fram_write_byte(addr+i, val);
	}
	#endif // SPI_FRAM
}

Fram_id fram_read_id()
{
	#if defined(SPI_FRAM)
	fram_select();
	spi_send_8(F_RDID);
	Fram_id id;
	id.manufacturer_id = spi_read_8();
	id.continuation_code = spi_read_8();
	id.product_idL = spi_read_8();
	id.product_idH = spi_read_8();
	fram_deselect();
	return id;

	#elif defined(I2C_FRAM)

	uint8_t a[3] = { 0 };
	i2c_start(0xF8 | I2C_WRITE);
	i2c_write(FRAM_I2C_ADDR<<1);
	i2c_receive(0xF8>>1, a, 3);

	Fram_id id;

	/* Shift values to separate manuf and prod IDs */
	/* See p.10 of http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf */
	id.manufacturer_id  = (a[0] << 4) + (a[1]  >> 4);
	id.product_id = ((a[1] & 0x0F) << 8) + a[2];
	
	return id;
	#endif // SPI_FRAM
}

uint8_t fram_test (void)
{
	Fram_id id = fram_read_id();
	#if defined(SPI_FRAM)
	if(id.manufacturer_id != 0x04 || id.continuation_code != 0x7F)
	{
		return RETURN_ERROR;
	}
	#elif defined(I2C_FRAM)
	if(id.manufacturer_id != 0x00A || id.product_id != 0x510)
	{
		return RETURN_ERROR;
	}
	#endif // SPI_FRAM
	return RETURN_SUCCESS;
}
