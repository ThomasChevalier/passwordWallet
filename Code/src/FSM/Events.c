#include "Events.h"

#include <avr/interrupt.h>

void events_happen(uint8_t event)
{
	GLOBALS_EVENTS |= event;
	ACTIVITY_TIMER = 0;
}

uint8_t events_get(void)
{
    uint8_t temp = GLOBALS_EVENTS;
    GLOBALS_EVENTS &= ~(EVENT_ALL_BUTTONS | EVENT_INIT_COMMUNICATION); // Delete all events but STOP and SLEEP_*
    return temp;
}
