#ifndef SECURITY_HEADER_THOMAS_CHEVALIER
#define SECURITY_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Init the random generator
void security_init(void);

// Fill buffer with 0, even if buffer is not used after
void security_erase_data(void *buffer, uint8_t lenght);

#endif // SECURITY_HEADER_THOMAS_CHEVALIER
