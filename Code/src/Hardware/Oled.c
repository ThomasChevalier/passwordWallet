#include "Oled.h"

#include <util/delay.h>
#include <string.h>

#include "Spi.h"

#include "../Globals.h"
#include "PinDefinition.h"
#include "Fram.h"

/*
	MOSI : 	11 	PB3
	MISO :	There is no need of Miso for the oled
	CLOCK :	13 	PB5
	SS :	10 	PB2
	RESET:	9	PB1
	DC : 	8	PB0
*/

#define WIDTH 128
#define HEIGHT 64

#ifndef STORE_SCREEN_BUFFER_IN_FRAM
static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF0, 0xF0, 0xC0, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF0,
	0x78, 0x1E, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x70, 0x38, 0x18, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x0F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x38, 0x70, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1E, 0x7C, 0xF0,
	0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFE, 0x1F, 0x03, 0x00,
	0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0x3E, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xE0,
	0x70, 0x38, 0x38, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F,
	0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
	0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x38, 0x38, 0x70, 0xE0,
	0xC0, 0x80, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x3E, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x1F, 0xFE, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFE, 0xFF, 0x03, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0xFF, 0xFE, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0F, 0x7F, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1F, 0x7F, 0xF0, 0xE0,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x1F, 0x1F,
	0x1F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0,
	0xF0, 0x7F, 0x1F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xC0, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x7E, 0xF0, 0xC0,
	0x80, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x01,
	0x03, 0x07, 0x07, 0x0E, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x04, 0x0E, 0x07, 0x07, 0x03, 0x01,
	0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x3C, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x80, 0xC0,
	0xF0, 0x7E, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
	0x0F, 0x1E, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x01, 0x03, 0x03, 0x07, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xC0, 0xC0,
	0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x07, 0x03, 0x03, 0x01, 0x00, 0x80, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x3C, 0x1E, 0x07, 0x03,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

// Hardware
void oled_setup_hardware()
{
	OLED_CS_DDR |= (1<<OLED_CS_PIN_NUM);
	OLED_DC_DDR |= (1<<OLED_DC_PIN_NUM);
	OLED_RES_DDR |= (1<<OLED_RES_PIN_NUM);

	oled_reset_low();
	oled_deselect();
	oled_dc_low();

}

void oled_reset_high()
{
	OLED_RES_PORT |= (1<<OLED_RES_PIN_NUM);
}
void oled_reset_low()
{
	OLED_RES_PORT &= ~(1<<OLED_RES_PIN_NUM);
}

void oled_deselect()
{
	OLED_CS_PORT |= (1<<OLED_CS_PIN_NUM);
}
void oled_select()
{
	OLED_CS_PORT &= ~(1<<OLED_CS_PIN_NUM);
}

void oled_dc_high()
{
	OLED_DC_PORT |= (1<<OLED_DC_PIN_NUM);
}
void oled_dc_low()
{
	OLED_DC_PORT &= ~(1<<OLED_DC_PIN_NUM);
}

void oled_setup_spi()
{
	// SPIE=0 SPE=1 DORD=0 MSTR=1 CPOL=1 CPHA=0 SPR1=0 SPR0=0
	SPCR = SPI_MASTER | SPI_ENABLE | (1<<3);
	SPSR &= ~(1<<SPI2X); // DeActive 2x speed mode
	//SPSR |= (1<<SPI2X); // Active 2x speed mode
}

// Software
void oled_init()
{
	oled_reset_high();
	_delay_ms(1);
	// bring reset low
	oled_reset_low();

	// wait 10ms
	_delay_ms(10);
	// bring out of reset
	oled_reset_high();

	// Init sequence

	oled_command(SSD1306_DISPLAYOFF);                    // 0xAE
	oled_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	oled_command(0x80);                                  // the suggested ratio 0x80

	oled_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	oled_command(SSD1306_LCDHEIGHT - 1);

	oled_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	oled_command(0x0);                                   // no offset
	oled_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
	oled_command(SSD1306_CHARGEPUMP);                    // 0x8D

	oled_command(0x14);

	oled_command(SSD1306_MEMORYMODE);                    // 0x20
	oled_command(0x00);                                  // 0x0 act like ks0108
	oled_command(SSD1306_SEGREMAP | 0x1);
	oled_command(SSD1306_COMSCANDEC);

	oled_command(SSD1306_SETCOMPINS);                    // 0xDA
	oled_command(0x12);
	oled_command(SSD1306_SETCONTRAST);                   // 0x81
	oled_command(0xCF);

	oled_command(SSD1306_SETPRECHARGE);                  // 0xd9

	oled_command(0xF1);

	oled_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	oled_command(0x40);
	oled_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	oled_command(SSD1306_NORMALDISPLAY);                 // 0xA6

	oled_command(SSD1306_DEACTIVATE_SCROLL);

	oled_command(SSD1306_DISPLAYON);//--turn on oled panel
}

// the most basic function, set a single pixel
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
	if( x & 0x80 || y & 0x40)
		return;
	// check rotation, move pixel around if necessary
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION))
	{
		x = 127 - x;
		y = 63 - y;
	}

	// x is which column
	#ifdef STORE_SCREEN_BUFFER_IN_FRAM

	uint8_t buf = fram_read_byte(OFFSET_OLED_BUFFER + (x+ (y/8)*WIDTH));
	uint8_t val = (1 << (y&7));
	switch (color)
	{
	case WHITE:
		buf |= val;
		break;
	case BLACK:
		buf &= ~val;
		break;
	case INVERSE:
		buf ^= val;
		break;
	}
	fram_write_byte(OFFSET_OLED_BUFFER + (x+ (y/8)*WIDTH), buf);

	#else

	uint8_t* buf = &buffer[x+ (y/8)*WIDTH];
	uint8_t val = (1 << (y&7));
	switch (color)
	{
	case WHITE:
		(*buf) |=  val;
		break;
	case BLACK:
		(*buf) &= ~val;
		break;
	case INVERSE:
		(*buf) ^=  val;
		break;
	}

	#endif
}

void oled_command(uint8_t c)
{
	// SPI
	oled_setup_spi();
	oled_deselect();
	oled_dc_low();
	oled_select();
	spi_send_8(c);
	oled_deselect();
}

void oled_invert_display(uint8_t i)
{
	oled_command( i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY );
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void oled_dim(uint8_t dim)
{
	// the range of contrast to too small to be really useful
	// it is useful to dim the display
	oled_command(SSD1306_SETCONTRAST);
	oled_command(dim ? 0 : 0xCF);
}

void oled_display(void)
{
	oled_command(SSD1306_COLUMNADDR);
	oled_command(0);   // Column start address (0 = reset)
	oled_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

	oled_command(SSD1306_PAGEADDR);
	oled_command(0); // Page start address (0 = reset)

	oled_command(7); // Page end address

	oled_deselect();
	oled_dc_high();
	oled_select();

	#ifdef STORE_SCREEN_BUFFER_IN_FRAM
	uint8_t pixBuff[64];
	
	uint8_t i = 0;
	uint8_t j = 0;
	for(; i < ((HEIGHT * WIDTH / 8) / 64); ++i)
	{
		oled_deselect();
		fram_read_bytes(OFFSET_OLED_BUFFER + i*64, pixBuff, 64);
		oled_setup_spi();
		oled_select();
		for(j = 0; j < 64; ++j)
		{
			spi_send_8(pixBuff[j]);
		}

	}
	#else

	uint16_t i=0;
	for (; i<(HEIGHT * WIDTH / 8); ++i)
	{
		spi_send_8(buffer[i]);
	}
	#endif

	oled_deselect();
}

// clear everything
void oled_clear_display(void)
{
	uint16_t i=0;
	#ifdef STORE_SCREEN_BUFFER_IN_FRAM
	for(; i < ((HEIGHT * WIDTH / 8) / 128); ++i)
	{
		fram_set(i*128, 0, 128);
	}
	#else
	for (; i<(HEIGHT * WIDTH / 8); ++i)
	{
		buffer[i] = 0;
	}
	#endif
}
