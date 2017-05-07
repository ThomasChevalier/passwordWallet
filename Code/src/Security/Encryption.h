#ifndef ENCRYPTION_HEADER_THOMAS_CHEVALIER
#define ENCRYPTION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Retrieve the key stored in eeprom in ram.
 * @details Only use it when the device is not encrypted.
 */
void encryption_copy_key_from_eeprom(void);

/**
 * @brief Disable encryption of the device.
 * @details Copy the current key (stored in KEY) in eeprom and update the OPTIONS_FLAG.
 */
void encryption_disable(void);

/**
 * @brief Enable encryption of the device.
 * @details Don't generate a new key, change_master_key should be called first.
 * Erase the key from eeprom and update OPTIONS_FLAG.
 */
void encryption_enable(void);

/**
 * @brief Check if KEY is valid.
 * @details Check the KEY buffer against a hash stored in eeprom.
 * @return Return RETURN_SUCCESS if the key is the good one, RETURN_ERROR otherwise.
 */
uint8_t encryption_check_key(void);

/**
 * @brief Change the encryption key for all the passwords.
 * @details Execution time of this function is proportionnal to the number of passwords stored in memory.
 * KEY must be valid. Copy new_key inside KEY.
 * Add 1 point of progress for each password updated.
 * 
 * @param new_key The new key used to encrypt the passwords.
 */
void encryption_update_key(uint8_t *new_key);

#endif // ENCRYPTION_HEADER_THOMAS_CHEVALIER
