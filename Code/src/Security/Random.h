#ifndef RANDOM_HEADER_THOMAS_CHEVALIER
#define RANDOM_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

extern volatile uint32_t random_dword; 		// The variable that contain the 32 bit of random
extern volatile uint8_t random_transfer; 	// 0 => random_dword has already been written to fram | 1 => random_dword should be written to fram

void	random_init					(void);							// Should be called once before any use of random_ functions
void	random_save_entropy			(void);							// Save entropy to the fram
uint8_t	random_request_byte			(void);							// Return a value in [0; 255] range, make the best use of entropy pool
uint8_t	random_request_printable	(void);							// Return a value in [' '; '~'] range

#endif // RANDOM_HEADER_THOMAS_CHEVALIER
