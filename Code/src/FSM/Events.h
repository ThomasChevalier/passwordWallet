#ifndef EVENTS_HEADER_THOMAS_CHEVALIER
#define EVENTS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Globals.h"

#define EVENT_BUTTON_1          (1<<0)
#define EVENT_BUTTON_2          (1<<1)
#define EVENT_BUTTON_3          (1<<2)
#define EVENT_BUTTON_4          (1<<3)
#define EVENT_ALL_BUTTONS       (EVENT_BUTTON_1 | EVENT_BUTTON_2 | EVENT_BUTTON_3 | EVENT_BUTTON_4)

#define EVENT_COMMUNICATING     (1<<4)
#define EVENT_STOP              (1<<5)
#define EVENT_SLEEP_SHUTDOWN    (1<<6)
#define EVENT_NO_SLEEP          (1<<7)

#define EVENT_WHENEVER (0xFF)

/**
 * @brief Add the event to the global event variable.
 * 
 * @param event One of EVENT_* define.
 */
void events_happen(uint8_t event);

/**
 * @brief Get the events that occur since the last call of this function.
 * @details Reset some event of the GLOBALS_EVENTS variable (Buttons and communication).
 * @return The events.
 */
uint8_t events_get(void);


#endif // EVENTS_HEADER_THOMAS_CHEVALIER
