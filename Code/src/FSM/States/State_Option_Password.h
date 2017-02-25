#ifndef STATE_OPTION_PASSWORD_HEADER_THOMAS_CHEVALIER
#define STATE_OPTION_PASSWORD_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_option_password_begin (void);
uint8_t state_option_password_transition (uint8_t event);
void state_option_password_end (void);

#endif // STATE_OPTION_PASSWORD_HEADER_THOMAS_CHEVALIER
