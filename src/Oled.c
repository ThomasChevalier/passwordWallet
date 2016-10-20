#include "Oled.h"

#include <string.h>

#include "Spi.h"
#include "font.h"

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

uint8_t _oled_rotation = 0;

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
    SPCR = 0x58;
    SPSR = 0; // Active 2x speed mode
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
    if(_oled_rotation)
    {
        x = ~(x|128); // x = 127 - x
        y = ~(y|64); // y = 64 - y
    }

    // x is which column
    uint8_t* buf = &buffer[x+ (y/8)*128];
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

void oled_rotate_180()
{
    _oled_rotation ^= 1;
}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void oled_start_scroll_right(uint8_t start, uint8_t stop)
{
    oled_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
    oled_command(0X00);
    oled_command(start);
    oled_command(0X00);
    oled_command(stop);
    oled_command(0X00);
    oled_command(0XFF);
    oled_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void oled_start_scroll_left(uint8_t start, uint8_t stop)
{
    oled_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
    oled_command(0X00);
    oled_command(start);
    oled_command(0X00);
    oled_command(stop);
    oled_command(0X00);
    oled_command(0XFF);
    oled_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void oled_start_scroll_diagright(uint8_t start, uint8_t stop)
{
    oled_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
    oled_command(0X00);
    oled_command(SSD1306_LCDHEIGHT);
    oled_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    oled_command(0X00);
    oled_command(start);
    oled_command(0X00);
    oled_command(stop);
    oled_command(0X01);
    oled_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void oled_start_scroll_diagleft(uint8_t start, uint8_t stop)
{
    oled_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
    oled_command(0X00);
    oled_command(SSD1306_LCDHEIGHT);
    oled_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
    oled_command(0X00);
    oled_command(start);
    oled_command(0X00);
    oled_command(stop);
    oled_command(0X01);
    oled_command(SSD1306_ACTIVATE_SCROLL);
}

void oled_stop_scroll(void)
{
    oled_command(SSD1306_DEACTIVATE_SCROLL);
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

    // SPI
    oled_deselect();
    oled_dc_high();
    oled_select();

    uint16_t i=0;
    for (; i<1024; i++)
    {
        spi_send_8(buffer[i]);
    }

    oled_deselect();
}

// clear everything
void oled_clear_display(void)
{

    uint16_t i=0;
    for (; i<1024; ++i)
    {
        buffer[i] = 0;
    }
}



void oled_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
    if(_oled_rotation) // 180 rotation
    {
        x = ~(x|128); // x = WIDTH - x - 1;
        y = ~(y|64); // y =HEIGHT - y - 1;
        x -= (w-1);
    }

    //oled_internal_h_line(x, y, w, color);
    uint8_t i = 0;
    for(; i < w; ++i)
    {
        oled_draw_pixel(x+w, y, color);
    }
}

/*void oled_internal_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
    // Do bounds/limit checks
    if(x & 0x80 || y & 0x40)
    {
        return;
    }

    // make sure we don't go off the edge of the display
    if( (x + w) > WIDTH)
    {
        w = (WIDTH - x);
    }

    // set up the pointer for  movement through the buffer
    register uint8_t *pBuf = buffer;
    // adjust the buffer pointer for the current row
    pBuf += (y/8) * 128;
    // and offset x columns in
    pBuf += x;

    register uint8_t mask = 1 << (y&7);

    switch (color)
    {
    case WHITE:
        while(w--)
        {
            *pBuf++ |= mask;
        };
        break;
    case BLACK:
        mask = ~mask;
        while(w--)
        {
            *pBuf++ &= mask;
        };
        break;
    case INVERSE:
        while(w--)
        {
            *pBuf++ ^= mask;
        };
        break;
    }
}*/

void oled_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{

    if(_oled_rotation) // 180 rotation
    {
        x = ~(x|128); // x = WIDTH - x - 1;
        y = ~(y|64); // y =HEIGHT - y - 1;
        y -= (h-1);
    }

    //oled_internal_v_line(x, y, h, color);
    uint8_t i = 0;
    for(; i < h; ++i)
    {
        oled_draw_pixel(x, y+i, color);
    }
}

/*
void oled_internal_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{

    // do nothing if we're off the left or right side of the screen
    if( x & 0x80 || y & 0x40)
    {
        return;
    }


    // make sure we don't go past the height of the display
    if( (y + h) > HEIGHT)
    {
        h = (HEIGHT - y);
    }

    // set up the pointer for fast movement through the buffer
    register uint8_t *pBuf = buffer;
    // adjust the buffer pointer for the current row
    pBuf += (y/8)*128;
    // and offset x columns in
    pBuf += x;

    // do the first partial byte, if necessary - this requires some masking
    register uint8_t mod = (y&7);
    if(mod)
    {
        // mask off the high n bits we want to set
        mod = 8-mod;

        // note - lookup table results in a nearly 10% performance improvement in fill* functions
        // register uint8_t mask = ~(0xFF >> (mod));
        static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
        register uint8_t mask = premask[mod];

        // adjust the mask if we're not going to reach the end of this byte
        if( h < mod)
        {
            mask &= (0XFF >> (mod-h));
        }

        switch (color)
        {
        case WHITE:
            *pBuf |=  mask;
            break;
        case BLACK:
            *pBuf &= ~mask;
            break;
        case INVERSE:
            *pBuf ^=  mask;
            break;
        }

        // fast exit if we're done here!
        if(h<mod)
        {
            return;
        }

        h -= mod;

        pBuf += SSD1306_LCDWIDTH;
    }


    // write solid bytes while we can - effectively doing 8 rows at a time
    if(h >= 8)
    {
        if (color == INVERSE)             // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
        {
            do
            {
                *pBuf=~(*pBuf);

                // adjust the buffer forward 8 rows worth of data
                pBuf += SSD1306_LCDWIDTH;

                // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
                h -= 8;
            }
            while(h >= 8);
        }
        else
        {
            // store a local value to work with
            register uint8_t val = (color == WHITE) ? 255 : 0;

            do
            {
                // write our value in
                *pBuf = val;

                // adjust the buffer forward 8 rows worth of data
                pBuf += SSD1306_LCDWIDTH;

                // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
                h -= 8;
            }
            while(h >= 8);
        }
    }

    // now do the final partial byte, if necessary
    if(h)
    {
        mod = h & 7;
        // this time we want to mask the low bits of the byte, vs the high bits we did above
        // register uint8_t mask = (1 << mod) - 1;
        // note - lookup table results in a nearly 10% performance improvement in fill* functions
        static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
        register uint8_t mask = postmask[mod];
        switch (color)
        {
        case WHITE:
            *pBuf |=  mask;
            break;
        case BLACK:
            *pBuf &= ~mask;
            break;
        case INVERSE:
            *pBuf ^=  mask;
            break;
        }
    }
}
*/

// Retourne la longueur non écrite
uint8_t oled_draw_char(uint8_t x, uint8_t y, uint8_t c)
{
    if(c < ' ' || c > '~' || x + 4 >= 128 || y + 5 >= 64)
        return 4;

    //const uint8_t *font_ptr = &font[(c-' ') * 4];

    uint8_t i;
    uint8_t* buf = &buffer[x+ (y/8)*128];
    for(i = 0; i < 4; ++i)
    {
        uint8_t font_byte = pgm_read_byte_near(font + ((c-' ') * 4) + i);
        //const uint8_t b = *font_ptr;
        if(/*b*/font_byte == 0)
        {
            return 4 - i;
        }
        //++font_ptr;


        (*buf) |= (font_byte << (y&7) );
        if( (y&7) > 4)
        {
            buffer[x+ (y/8)*128+128+i] |= (font_byte >> (8 - (y&7)) );
        }
        ++buf;

    }
    return 0;
}

void oled_draw_text(uint8_t x, uint8_t y, char *str, uint8_t str_len)
{
    if( y % 8 != 0)
        return;
    if(str_len == 0)
        str_len = strlen(str);

    uint8_t i = 0;
    for(i = 0; i < str_len; ++i)
    {
        uint8_t s = oled_draw_char(x, y, str[i]);
        x += 5 - s;
        if(x > 123)
        {
            x = 0;
            y += 8;
        }
    }
}


void oled_draw_hex(uint8_t x, uint8_t y, uint8_t* buffer, uint8_t bufferSize)
{
    uint8_t index = 0;
    char str[bufferSize * 2];
    for(index = 0; index < bufferSize; ++index)
    {
        uint8_t nibble = buffer[index] >> 4;// High
        str[index * 2] = (nibble < 10) ? nibble + '0' : (nibble-10) + 'A';
        nibble = buffer[index] & 0x0F; // Low
        str[index * 2 + 1] = (nibble < 10) ? nibble + '0' : (nibble-10) + 'A';
    }
    oled_draw_text(x, y, str, bufferSize * 2);
}

