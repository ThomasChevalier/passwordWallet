#ifndef DRAWING_HEADER_THOMAS_CHEVALIER
#define DRAWING_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

void draw_clear(void);
void draw_update(void);
uint8_t draw_char(uint8_t x, uint8_t y, uint8_t c);
void draw_text(uint8_t x, uint8_t y, char *str, uint8_t str_len);
void draw_hex(uint8_t x, uint8_t y, uint8_t* hexBuffer, uint8_t hexBufferSize);
void draw_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
void draw_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color);


void draw_main_menu(void);
void draw_browse_dock(char letter, uint8_t highlight);
void draw_option_menu(uint8_t currentChoice);
void draw_confirmation_screen(uint8_t choice, uint8_t yesNo);

void type_string(char* string_typed, uint8_t maxLen);

void draw_typing_screen(char* str, uint8_t column);

// There is no more than 10 column
// So lower nibble of column_and_flags is for the column
// Upper nibble is for flag :
// bit 4 = 1 ? highlight : no_highlight
// bit 5 = 1 ? dock_mode : no_dock_mode
void draw_char_column(uint8_t column_and_flags, char letter);


#endif // DRAWING_HEADER_THOMAS_CHEVALIER
