#ifndef ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER
#define ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>

/**
 * @brief Convert a ascii character to a keyboard keycode.
 * @details This function use azerty or qwerty layout according to the value stored in memory.
 * 
 * @param c The character to convert, should be in the range [' '; '~'].
 * @return An HID keycode, with modifier.
 */
uint8_t ascii_to_keycode(char c);


#endif // ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER
