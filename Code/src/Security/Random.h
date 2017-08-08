#ifndef RANDOM_HEADER_THOMAS_CHEVALIER
#define RANDOM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>


/**
 * @brief Should be called once at the beggining of the program.
 * @details This function initializes the global variables needed to implement the circular entropy pool and
 * the buffer that holds the raw Timer 1 values that are used to create the entropy pool.  It then
 * Initializes the Watch Dog Timer (WDT) to perform an interrupt every 2048 clock cycles, (about 
 * 16 ms) which is as fast as it can be set.
 */
void	random_init					(void);

/**
 * @brief Save entropy to memory.
 * @details If the entropy pool is full then the random double word is not saved.
 */
void	random_save_entropy			(void);

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
 * @return Return a character in [' '; '~'] range
 */
uint8_t	random_request_printable	(void);

/**
 * @brief Reset the random generator
 */
void random_reset(void);

#endif // RANDOM_HEADER_THOMAS_CHEVALIER
