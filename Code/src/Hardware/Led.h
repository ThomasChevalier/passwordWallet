#ifndef LED_HEADER_THOMAS_CHEVALIER
#define LED_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Blink the the led for a defined number of times.
 * @details Delay duration is managed by LED_BLINK_DELAY global define.
 * 
 * @param times The number of times that the led has to be turned on and turned off.
 */
void led_blink(uint8_t times);

#endif // LED_HEADER_THOMAS_CHEVALIER
