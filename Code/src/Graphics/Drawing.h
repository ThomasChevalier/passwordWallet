#ifndef DRAWING_HEADER_THOMAS_CHEVALIER
#define DRAWING_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

/**
 * @brief Clear the screen.
 */
void draw_clear(void);

/**
 * @brief Send the frame to the screen.
 * @details Whatever modifications might have been done on the pixel buffer, this function should be called before anything change on the physic screen.
 */
void draw_update(void);

/**
 * @brief Draw a character on the screen using the system font.
 * 
 * @param x The x top left corner of the character.
 * @param y The y top left corner of the character.
 * @param c The character to draw.
 * @return Return FONT_WIDTH - [character size], in pixel
 */
uint8_t draw_char(uint8_t x, uint8_t y, uint8_t c);

/**
 * @brief Same as draw_text but load the str_index in the str_buffer before.
 */
void draw_flash_string(uint8_t x, uint8_t y, uint16_t str_index);
#define draw_flash_str_cx(y, str) draw_flash_string(str##_centerX, y, str##_index)
#define draw_flash_str(x, y, str) draw_flash_string(x, y, str##_index)

//void draw_text_index(uint8_t x, uint8_t y, uint8_t str_index);

//#define draw_text_index(x,y,s) str_to_buffer(s); draw_text(x, y, str_buffer, 0);

/**
 * @brief Draw a string on the screen at a given position.
 * @details New lines are added if the string is too long.
 * 
 * @param x The x top left corner of the first character of the string.
 * @param y The y top left corner of the first character of the string.
 * @param str The string of character.
 * @param max The maximum number of character to display. The function stops displaying char as soon as it see a '\0'
 */
void draw_text(uint8_t x, uint8_t y, char *str, uint8_t max);

/**
 * @brief Retrieive a table of byte on screen.
 * @details WARNING : debug fonction only.
 * 
 * @param x The x top left corner of the character.
 * @param y The y top left corner of the character.
 * @param hexBuffer The array of byte to display.
 * @param hexBufferSize The lenght of the array.
 */
void draw_hex(uint8_t x, uint8_t y, uint8_t* hexBuffer, uint8_t hexBufferSize);

/**
 * @brief Draw an horizontal line on the screen.
 * 
 * @param x The x coordinate of the first point.
 * @param y The y coordinate of the first point.
 * @param w The width of the line.
 * @param color The color of the line.
 */
void draw_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color);

/**
 * @brief Draw a vertical line on the screen.
 * 
 * @param x The x coordinate of the first point.
 * @param y The y coordinate of the first point.
 * @param w The height of the line.
 * @param color The color of the line.
 */
void draw_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color);


void draw_main_menu(void);
void draw_browse_dock(char letter, uint8_t highlight);

// Return 1 if the string has been modified and 0 if nothing changed
uint8_t type_string(char* string_typed, uint8_t maxLen);

void draw_typing_screen(char* str, uint8_t column, uint8_t max);

// There is no more than 10 column
// So lower nibble of column_and_flags is for the column
// Upper nibble is for flag :
// bit 4 = 1 ? highlight : no_highlight
// bit 5 = 1 ? dock_mode : no_dock_mode
void draw_char_column(uint8_t column_and_flags, char letter);


#endif // DRAWING_HEADER_THOMAS_CHEVALIER
