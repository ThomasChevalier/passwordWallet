#ifndef OLED_HEADER_THOMAS_CHEVALIER
#define OLED_HEADER_THOMAS_CHEVALIER

#include <avr/io.h>
#include <util/delay.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_LCDWIDTH             128
#define SSD1306_LCDHEIGHT            64

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

// Hardware
void oled_setup_hardware();

void oled_reset_high();
void oled_reset_low();

void oled_deselect();
void oled_select();

void oled_dc_high();
void oled_dc_low();

void oled_setup_spi();


// Software
void oled_init();
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void oled_command(uint8_t c);
void oled_invert_display(uint8_t i); // Invert all the colors
void oled_start_scroll_right(uint8_t start, uint8_t stop);
void oled_start_scroll_left(uint8_t start, uint8_t stop);
void oled_start_scroll_diagright(uint8_t start, uint8_t stop);
void oled_start_scroll_diagleft(uint8_t start, uint8_t stop);
void oled_stop_scroll();
void oled_dim(uint8_t dim);
void oled_display();
void oled_clear_display();
void oled_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
//void oled_internal_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
void oled_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color);
//void oled_internal_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color);
uint8_t oled_draw_char(uint8_t x, uint8_t y, uint8_t c);
void oled_draw_text(uint8_t x, uint8_t y, char *str, uint8_t str_len);
void oled_draw_hex(uint8_t x, uint8_t y, uint8_t* buffer, uint8_t bufferSize);


#endif // OLED_HEADER_THOMAS_CHEVALIER
