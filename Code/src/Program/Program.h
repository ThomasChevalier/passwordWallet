#ifndef PROGRAM_HEADER_THOMAS_CHEVALIER
#define PROGRAM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void program_init(void);
void program_update(void);
void program_wait(void);
void program_pause_until_event(uint8_t event); // Pause the program until event 'event' occurs

#endif // PROGRAM_HEADER_THOMAS_CHEVALIER
