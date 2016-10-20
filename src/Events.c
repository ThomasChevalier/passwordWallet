#include "Events.h"

#include <avr/interrupt.h>

void eventHappen(uint8_t event)
{
    cli();
    GLOBALS_EVENTS |= event;
    sei();
}

uint8_t getEvents()
{
    cli();
    uint8_t temp = GLOBALS_EVENTS;
    GLOBALS_EVENTS = 0;
    sei();
    return temp;
}
