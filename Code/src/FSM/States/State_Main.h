#ifndef STATE_MAIN_HEADER_THOMAS_CHEVALIER
#define STATE_MAIN_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_main_begin (void);
uint8_t state_main_transition (uint8_t event);
void state_main_end (void);

#endif // STATE_MAIN_HEADER_THOMAS_CHEVALIER
