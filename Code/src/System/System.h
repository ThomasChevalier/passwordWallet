#ifndef SYSTEM_HEADER_THOMAS_CHEVALIER
#define SYSTEM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Init the microcontroller. Should be called once at the beginning of the main function.
 * @details Init the timers and the ADC converter. Code taken from Arduino sources.
 */
void system_init(void);

/**
 * @brief Find why the microcontroller has been reset and display it
 */
void system_read_reset_source(uint8_t mcusr);

#endif // SYSTEM_HEADER_THOMAS_CHEVALIER
