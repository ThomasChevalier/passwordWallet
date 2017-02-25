#ifndef STATE_OPTIONS_HEADER_THOMAS_CHEVALIER
#define STATE_OPTIONS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_options_begin (void);
uint8_t state_options_transition (uint8_t event);
void state_options_end (void);

#endif // STATE_OPTIONS_HEADER_THOMAS_CHEVALIER
