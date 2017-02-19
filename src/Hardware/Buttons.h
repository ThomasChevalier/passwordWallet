#ifndef BUTTONS_HEADER_THOMAS_CHEVALIER
#define BUTTONS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void 	buttons_setup_hardware	(void);
void 	buttons_update_event	(void); // Update event flag for buttons, should be call regularly
uint8_t buttons_pressed			(void); // Bit 0 for button 1, bit 2 for button 1, etc ... 1 <=> pressed / 0 <=> released


#endif
