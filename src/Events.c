#include "Events.h"

#include <avr/interrupt.h>

void eventHappen(uint8_t event)
{
    GLOBALS_EVENTS |= event;
}

uint8_t getEvents()
{
    uint8_t temp = GLOBALS_EVENTS;
    GLOBALS_EVENTS = 0;
    return temp;
}
