#ifndef TRANSITIONS_HEADER_THOMAS_CHEVALIER
#define TRANSITIONS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define TRANSITION(state_type) state_ ##state_type ## _transition
#define DECLARE_TRANSITION(state_type) uint8_t TRANSITION(state_type) (uint8_t event)

DECLARE_TRANSITION(STATE_INIT);
DECLARE_TRANSITION(STATE_MAIN);
DECLARE_TRANSITION(STATE_BROWSE);
DECLARE_TRANSITION(STATE_OPTION);
DECLARE_TRANSITION(STATE_SAVE);

// Drawing functions
void draw_main_menu();
void draw_browse_dock(char letter, uint8_t highlight);
void draw_option_menu(uint8_t currentChoice);
void draw_confirmation_screen(uint8_t choice, uint8_t yesNo);
#endif // TRANSITIONS_HEADER_THOMAS_CHEVALIER
