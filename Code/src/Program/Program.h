#ifndef PROGRAM_HEADER_THOMAS_CHEVALIER
#define PROGRAM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Init the program.
 * @details Initialize some variables and read relevant content from memory.
 */
void program_init(void);

/**
 * @brief Update the program. Should be called regularly.
 */
void program_update(void);

/**
 * @brief Do a pause in the program, in order to slow down the interface.
 * @details Wait for WAIT_DELAY ms.
 * If the user press a button while the device is in idle mode, the device
 * wake up (with the pin change interrupt), and therefore the device wait
 * less WAIT_DELAY ms.
 */
void program_wait(void);


/**
 * @brief Pause the program until event 'event' occurs
 * @details Call program_update
 * @param event The event to wait for.
 */
void program_pause_until_event(uint8_t event); 

#endif // PROGRAM_HEADER_THOMAS_CHEVALIER
