/*

Copyright 2017 Thomas Chevalier
All rights reserved

*/

#include "../Globals.h"
#include "Fram.h"
#include "Spi.h"
#include "PinDefinition.h"

void fram_setup_hardware()
{
	FRAM_CS_DDR |= (1<<FRAM_CS_PIN_NUM);
	fram_deselect();
}

void fram_setup_spi()
{
	// SPIE=0 SPE=1 DORD=0 MSTR=1 CPOL=0 CPHA=0 SPR1=0 SPR0=0
	SPCR = SPI_ENABLE | SPI_MASTER | SPI_CLOCK_DIV4;
	SPSR |= (1<<SPI2X); // Active 2x speed mode
}

uint8_t fram_read_byte(uint16_t addr)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_READ);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	uint8_t result = spi_read_8();
	fram_deselect();
	return result;
}

void fram_read_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_READ);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);

	spi_read(buffer, size);

	fram_deselect();
}

void fram_write_byte(uint16_t addr, uint8_t byte)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();
	
	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_send_8(byte);
	fram_deselect();
}

void fram_write_bytes(uint16_t addr, uint8_t* buffer, uint8_t size)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();

	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_send(buffer, size);
	fram_deselect();
}

void fram_set (uint16_t addr, uint8_t val, uint8_t num)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_WREN); // Enable write to memory
	fram_deselect();

	fram_select();
	spi_send_8(F_WRITE);
	spi_send_8(addr >> 8);
	spi_send_8(addr & 0xFF);
	spi_set(val, num);
	fram_deselect();
}

Fram_id fram_read_id()
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_RDID);
	Fram_id id;
	id.manufacturer_id = spi_read_8();
	id.continuation_code = spi_read_8();
	id.product_idL = spi_read_8();
	id.product_idH = spi_read_8();
	fram_deselect();
	return id;
}

uint8_t fram_read_register()
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_RDSR);
	uint8_t result = spi_read_8();
	fram_deselect();
	return result;
}

void fram_write_register(uint8_t reg)
{
	fram_setup_spi();
	fram_select();
	spi_send_8(F_WREN);
	fram_deselect();

	fram_select();
	spi_send_8(F_WRSR);
	spi_send_8(reg);
	fram_deselect();
}

void fram_select()
{
	FRAM_CS_PORT &= ~(1<<FRAM_CS_PIN_NUM);
}

void fram_deselect()
{
	FRAM_CS_PORT |= (1<<FRAM_CS_PIN_NUM);
}

uint8_t fram_test (void)
{
	Fram_id id = fram_read_id();
	if(id.manufacturer_id != 0x04 || id.continuation_code != 0x7F)
	{
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}
