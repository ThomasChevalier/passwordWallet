#ifndef STATE_WAIT_CARD_HEADER_THOMAS_CHEVALIER
#define STATE_WAIT_CARD_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_wait_card_begin(void);
uint8_t state_wait_card_transition (uint8_t event);
void state_wait_card_end(void);

#endif // STATE_WAIT_CARD_HEADER_THOMAS_CHEVALIER
