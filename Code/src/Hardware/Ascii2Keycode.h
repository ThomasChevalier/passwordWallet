#ifndef ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER
#define ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>

#define KEYCODE_MODIFIER_SHIFT	(1<<7)
#define KEYCODE_MODIFIER_ALTGR	(1<<6)

uint8_t ascii_to_keycode(char c);


#endif // ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER
