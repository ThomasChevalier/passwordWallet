#ifndef DRAWING_HEADER_THOMAS_CHEVALIER
#define DRAWING_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

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
