#ifndef SECURITY_HEADER_THOMAS_CHEVALIER
#define SECURITY_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Init all the security modules.
 * @details Init the random generator
 */
void security_init(void);

/**
 * @brief Fill array with 0, even if buffer is not used after
 * @details This is the only safe way to destroy a sensitive information.
 * 
 * @param buffer The memory to erase.
 * @param lenght The lenght of the memory chunk.
 */
//void security_erase_data(void *buffer, uint8_t lenght);

/**
 * @brief Compare in a secure way two blocks of memory
 * @details The function does the comparaison in a constant time,
 * preventing timing attack
 *  
 * @param data1 A pointer to one of the block to compare
 * @param data2 A pointer to the other block
 * @param sz The number of byte to be compared
 * @return Returns RETURN_SUCCESS if the two block match, RETURN_ERROR otherwise
 */
uint8_t security_memcmp(const void *data1, const void* data2, uint8_t sz) __attribute__((naked)) ;

#endif // SECURITY_HEADER_THOMAS_CHEVALIER
