#ifndef SLEEP_HEADER_THOMAS_CHEVALIER
#define SLEEP_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/sleep.h>

#include "../FSM/Events.h"

/**
 * @brief Sleep device until a button is pressed.
 */
void sleep_device(void);

/**
 * @brief Sleep device until an interrupt happen
 */
void sleep_idle(void);

/**
 * @brief Put the device in noise reduction mode, by putting to sleep various component.
 * This function exists because the RFID reader need a quit environment. Thus I hardcoded the
 * desired delay of 32ms in this function.
 */
void sleep_noise_reduction(void);

#define DISABLE_SLEEP() (GLOBALS_EVENTS |=  EVENT_NO_SLEEP)
#define ENABLE_SLEEP()  (GLOBALS_EVENTS &= ~EVENT_NO_SLEEP)
#endif // SLEEP_HEADER_THOMAS_CHEVALIER
