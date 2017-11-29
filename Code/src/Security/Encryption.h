#ifndef ENCRYPTION_HEADER_THOMAS_CHEVALIER
#define ENCRYPTION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include "../Memory/MemoryLayout.h"

/**
 * @brief Check if KEY is valid.
 * @details Check the KEY buffer against a hash stored in eeprom.
 * @return Return RETURN_SUCCESS if the key is the good one, RETURN_ERROR otherwise.
 */
uint8_t encryption_check_key(void);

/**
 * @brief Update the eeprom validation of the key.
 * This function add 20 point of progress (16 for random values, 2 for aes, 2 for eeprom)
 */
void encryption_update_validation(void);

/**
 * @brief Change the encryption key for all the passwords.
 * @details Execution time of this function is proportionnal to the number of passwords stored in memory.
 * KEY must be valid. Copy new_key inside KEY.
 * Add 1 point of progress for each password updated.
 * 
 * @param new_key The new key used to encrypt the passwords.
 * @parem start_from The password id to begin the update. In normal use of this function start_from = 0, but backup system need this arg.
 */
void encryption_update_key(uint8_t *new_key, p_addr start_from);

#endif // ENCRYPTION_HEADER_THOMAS_CHEVALIER
