#ifndef ENCRYPTION_HEADER_THOMAS_CHEVALIER
#define ENCRYPTION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

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
