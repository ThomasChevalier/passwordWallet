#ifndef EEPROM_HEADER_THOMAS_CHEVALIER
#define EEPROM_HEADER_THOMAS_CHEVALIER

#include <avr/eeprom.h>

// Program eeprom with "make dfu-ee"

typedef struct Eeprom_init_
{
	uint8_t rand_seq       [16];
	uint8_t enc_validation [16];
	uint8_t rfid_firmware  [64];

	uint8_t qwerty_to_keycode_map[96];
	uint8_t azerty_to_keycode_map[96];
} Eeprom_init;

extern Eeprom_init EEPROM_VARS EEMEM;

#endif // EEPROM_HEADER_THOMAS_CHEVALIER
