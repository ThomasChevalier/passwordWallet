#ifndef STATE_COMMUNICATION_HEADER_THOMAS_CHEVALIER
#define STATE_COMMUNICATION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void state_communication_process_data(uint8_t* buffer, uint8_t lenght);

void state_communication_begin (void);
uint8_t state_communication_transition (uint8_t event);
void state_communication_end (void);

#endif // STATE_COMMUNICATION_HEADER_THOMAS_CHEVALIER
