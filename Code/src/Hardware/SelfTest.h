#ifndef SELF_TEST_HEADER_THOMAS_CHEVALIER
#define SELF_TEST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

uint8_t	self_test_check		(void); // Check if the device is ok. Return 1 if everything is fine, return 0 if self test must be performed
void	self_test_execute	(void); // Execute test for the whole device

#endif // SELF_TEST_HEADER_THOMAS_CHEVALIER
