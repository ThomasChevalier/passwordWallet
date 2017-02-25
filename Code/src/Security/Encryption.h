#ifndef ENCRYPTION_HEADER_THOMAS_CHEVALIER
#define ENCRYPTION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Only use it when the device is not encrypted, copy the content of the key stored in eeprom in ram
void encryption_copy_key_from_eeprom(void);

// Disable encryption of the device
void encryption_disable(void);

// Enable encryption of the device
// Don't generate a new key, change_master_key should be called first.
// This function erase the key from eeprom and update flags
void encryption_enable(void);

// Check the KEY buffer against a hash stored in eeprom, should return 1 if key is correct.
uint8_t encryption_check_key(void);

// Change the encryption key for all the passwords (decrypt with KEY and encrypt with new_key)
void encryption_update_key(uint8_t *new_key);

#endif // ENCRYPTION_HEADER_THOMAS_CHEVALIER
