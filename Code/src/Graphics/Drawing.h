#ifndef DRAWING_HEADER_THOMAS_CHEVALIER
#define DRAWING_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

/**
 * @brief Clear the screen buffer.
 */
void draw_clear(void);

/**
 * @brief Send the frame to the screen.
 * @details Whatever modifications might have been done on the pixel buffer, this function should be called before anything change on the physic screen.
 */
void draw_update(void);

/**
 * @brief Draw a character on the screen using the system font.
 * The null character must be converted in INVALID_CHARACTER
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

void draw_num(uint8_t x, uint8_t y, uint16_t num);

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

/**
 * @brief Get a string from user input.
 * @details This is a blocant function, it returns when the user have finished
 * to type the string.
 * 
 * @param string_typed The initial string to type
 * @param maxLen The maximum length of the string
 * 
 * @return Returns TRUE if the string has been modified, FALSE otherwise.
 * The string is modified if the user changes one letter, even if he replaces
 * the letter after. So a modified string may not have changed, but the 
 * user have pressed buttons.
 */
uint8_t type_string(char* string_typed, uint8_t maxLen);

/**
 * @brief Helper function for the type_string function
 * 
 * @param str The string to draw
 * @param column The current column
 * @param max The maximum size of the string
 */
void draw_typing_screen(char* str, uint8_t column, uint8_t max);

/**
 * @brief Draw a column of character
 * @details There is no more than 10 column so the lower
 * nibble of column_and_flags is for the column number and
 * the upper nibble is for the flags
 * bit 4 = 1 ? highlight : no_highlight
 * bit 5 = 1 ? dock_mode : no_dock_mode
 * @param column_and_flags The column number and the flags, as described above
 * @param letter the letter to display
 */
void draw_char_column(uint8_t column_and_flags, char letter);


#endif // DRAWING_HEADER_THOMAS_CHEVALIER
