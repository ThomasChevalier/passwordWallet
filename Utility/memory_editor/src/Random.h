#ifndef RANDOM_HEADER_THOMAS_CHEVALIER
#define RANDOM_HEADER_THOMAS_CHEVALIER

#include <cstdint>

/**
 * @brief Get a byte of pure random.
 * @details Return a value in [0; 255] range, make the best use of entropy pool
 * The function may be blockant.
 * @return A true random number.
 */
uint8_t	random_request_byte			(void);

/**
 * @brief Return an ascii printable byte.
 * @details This function use random_request_byte so may be blockant.
 * @return Return a character in [' '; '~'[ range
 */
uint8_t	random_request_printable	(void);



#endif // RANDOM_HEADER_THOMAS_CHEVALIER