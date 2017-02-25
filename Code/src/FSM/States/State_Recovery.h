#ifndef STATE_RECOVERY_HEADER_THOMAS_CHEVALIER
#define STATE_RECOVERY_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_recovery_do_full_reset(void);

void state_recovery_begin (void);
uint8_t state_recovery_transition (uint8_t event);
void state_recovery_end (void);

#endif // STATE_RECOVERY_HEADER_THOMAS_CHEVALIER
