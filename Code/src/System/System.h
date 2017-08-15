#ifndef SYSTEM_HEADER_THOMAS_CHEVALIER
#define SYSTEM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Init the microcontroller. Should be called once at the beginning of the main function.
 * @details Init the timers and active interrupts.
 */
void system_init(void);

/**
 * @brief Find why the microcontroller has been reset and display it
 */
void system_read_reset_source(uint8_t mcusr);

/**
 * @brief Read the current vcc value, against 1.1V voltage.
 * @details Take at least 4 ms.
 * @return The vcc value in millivolts
 */
uint16_t system_read_vcc(void);

/**
 * @brief Calculate the amount of free ram.
 * @return The amount in byte of free ram.
 */
uint16_t system_free_ram (void);

#endif // SYSTEM_HEADER_THOMAS_CHEVALIER
