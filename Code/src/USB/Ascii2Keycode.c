#include "Ascii2Keycode.h"

#include "../Globals.h"
#include "../Memory/eeprom.h"

uint8_t ascii_to_keycode(char c)
{
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_QWERTY))
	{
		return eeprom_read_byte(&(EEPROM_VARS.qwerty_to_keycode_map[c-' ']));
	}
	else
	{
		return eeprom_read_byte(&(EEPROM_VARS.azerty_to_keycode_map[c-' ']));
	}
}