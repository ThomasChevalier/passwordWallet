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
void security_erase_data(void *buffer, uint8_t lenght);

#endif // SECURITY_HEADER_THOMAS_CHEVALIER
