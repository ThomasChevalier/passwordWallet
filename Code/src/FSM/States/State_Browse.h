#ifndef STATE_BROWSE_HEADER_THOMAS_CHEVALIER
#define STATE_BROWSE_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_browse_begin (void);
uint8_t state_browse_transition (uint8_t event);
void state_browse_end (void);

#endif // STATE_BROWSE_HEADER_THOMAS_CHEVALIER
