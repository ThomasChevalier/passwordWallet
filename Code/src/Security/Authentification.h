#ifndef AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
#define AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Wait until a new RFID tag is present. This is a blockant function.
 */
void user_wait_card(void);

/**
 * @brief Try to authenticate on the rfid tag.
 * @details It use the key 0xFFFFFFFFFFFF to authenticate.
 * This function may draw on screen (str_error_card or str_error_auth).
 * 
 * @return Return RETURN_SUCCESS if success, RETURN_ERROR otherwise.
 */
uint8_t	user_authenticate_card	(void);

/**
 * @brief Display a message and a progress bar and update encryption validation
 * backup_free() must be called after this function.
 */
void user_update_validation (void);

/**
 * @brief Change the key used to encrypt the fram memory.
 * @details Execution time of this function is proportionnal to the number of passwords stored in memory.
 * This function always draw on screen.
 */
void	user_change_key		(void);

/**
 * @brief Read a key from the card.
 * @details This function assumes that the card is authenticate.
 * 
 * @param keyOut The output of the read. Must be 18 bytes long.
 * @param keyBlock Where to read the key.
 * 
 * @return RETURN_SUCCESS if success, RETURN_ERROR otherwise.
 */
uint8_t user_read_key_from_card(uint8_t* keyOut, uint8_t keyBlock);

/**
 * @brief Write a key to the card.
 * @details This function assumes that the card is authenticate.
 * 
 * @param keyOut The key to write. Must be 16 bytes long.
 * @param keyBlock Where to write the key.
 * 
 * @return RETURN_SUCCESS if success, RETURN_ERROR otherwise.
 */
uint8_t user_write_key_to_card(uint8_t* keyIn, uint8_t keyBlock);

/**
 * @brief Display the master key on the screen.
 * @details Displayed key is encoded with Ascii85.
 */
void user_display_key(void);

#endif // AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
