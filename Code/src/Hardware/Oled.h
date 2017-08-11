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


// Software
/**
 * @brief Setup the oled software, send command to the screen.
 * @details This function take about 11 ms.
 */
void oled_init				(void);

/**
 * @brief Draw a pixel in the pixel buffer.
 * @details This DOES NOT display a pixel on the screen, oled_display should be called for that.
 * 
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param color The color of the pixel (BLACK, WHITE or INVERSE)
 */
void oled_draw_pixel		(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief Invert all the color of the display.
 * @details Black become white and white become black.
 * 
 * @param i If i is no zero then switch to invert mode, else if i equals 0 then switch to normal mode.
 */
void oled_invert_display	(uint8_t i);

/**
 * @brief Dim the display.
 * @details When the display is dimmed it has less contrast and use less power.
 * 
 * @param dim If 'dim' is no zero then dim the display, else go to normal contrast.
 */
void oled_dim				(uint8_t dim); // dim(1) = less contrast = less power

/**
 * @brief Send the internal pixel buffer to the oled screen.
 */
void oled_display			(void);

/**
 * @brief Set the internal pixel buffer to zero.
 */
void oled_clear_display		(void);

/**
 * @brief Active or not the display
 * 
 * @param active Display is active if true, switch off if false
 */
void oled_active_display(uint8_t active);


#endif // OLED_HEADER_THOMAS_CHEVALIER
