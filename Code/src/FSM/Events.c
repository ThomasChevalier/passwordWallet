#include "Events.h"

#include <avr/interrupt.h>

uint8_t events_get(void)
{
    uint8_t temp = GLOBALS_EVENTS;
    GLOBALS_EVENTS &= ~(EVENT_ALL_BUTTONS | EVENT_INIT_COMMUNICATION); // Delete all events but STOP
    return temp;
}
