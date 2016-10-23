#ifndef DRAWING_HEADER_THOMAS_CHEVALIER
#define DRAWING_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void draw_main_menu();
void draw_browse_dock(char letter, uint8_t highlight);
void draw_option_menu(uint8_t currentChoice);
void draw_confirmation_screen(uint8_t choice, uint8_t yesNo);
void draw_typing_column(uint8_t column);

#endif // DRAWING_HEADER_THOMAS_CHEVALIER
