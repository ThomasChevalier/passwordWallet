#ifndef SLEEP_HEADER_THOMAS_CHEVALIER
#define SLEEP_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/sleep.h>

#include "../FSM/Events.h"

void sleep_device(uint8_t sleepMode);


#define DISABLE_SLEEP() (GLOBALS_EVENTS |=  EVENT_NO_SLEEP)
#define ENABLE_SLEEP()  (GLOBALS_EVENTS &= EVENT_NO_SLEEP)
#endif // SLEEP_HEADER_THOMAS_CHEVALIER
