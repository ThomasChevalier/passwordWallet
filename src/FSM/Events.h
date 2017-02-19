#ifndef EVENTS_HEADER_THOMAS_CHEVALIER
#define EVENTS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Globals.h"

#define EVENT_BUTTON_1 			(1<<0)
#define EVENT_BUTTON_2			(1<<1)
#define EVENT_BUTTON_3			(1<<2)
#define EVENT_BUTTON_4			(1<<3)
#define EVENT_ALL_BUTTONS		(EVENT_BUTTON_1 | EVENT_BUTTON_2 | EVENT_BUTTON_3 | EVENT_BUTTON_4)

#define EVENT_WHENEVER (0xFF)

void events_happen(uint8_t event);
uint8_t events_get(void);

#endif // EVENTS_HEADER_THOMAS_CHEVALIER
