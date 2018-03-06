#ifndef BUTTONS_HEADER_THOMAS_CHEVALIER
#define BUTTONS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Debouncing algorithm parameters
#define RELEASE_TIME_MS (100)
#define PRESS_TIME_MS (50)
#define LONG_PRESS_TIME_MS (500)

/**
 * @brief Update event flag for buttons, should be call regularly (to detect long press).
 */
void	buttons_update_event    (void);

/**
 * @brief Return the buttons pressed.
 * @details Bit 0 is for button 1, bit 1 for button 2, bit 2 for button 3 and bit 3 for button 4.
 * A bit set to 1 means that the button is pressed, a bit set to 0 means that the button is released.
 * @return Return the status of the buttons.
 */
uint8_t buttons_pressed         (void);

/**
 * @brief Routine to debounce the buttons and update the events
 * @details To call whenever a button change (for example in the pcint)
 */
void    buttons_debounce        (void);
#endif
