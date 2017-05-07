#ifndef AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
#define AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Try to authenticate on the rfid tag.
 * @details It use the key 0xFFFFFFFFFFFF to authenticate.
 * This function may draw on screen (str_error_card or str_error_auth).
 * 
 * @return Return RETURN_SUCCESS if success, RETURN_ERROR otherwise.
 */
uint8_t	authenticate_on_card	(void);

/**
 * @brief Change the key used to encrypt the fram memory.
 * @details Execution time of this function is proportionnal to the number of passwords stored in memory.
 * This function always draw on screen.
 */
void	change_master_key		(void);

#endif // AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
