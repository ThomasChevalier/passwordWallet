#ifndef SELF_TEST_HEADER_THOMAS_CHEVALIER
#define SELF_TEST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
/**
 * @brief Check if the device is ok.
 * @return Return RETURN_SUCCESS if everything is fine, return RETURN_ERROR if self test must be performed.
 */
uint8_t	self_test_check		(void);

/**
 * @brief  Execute tests for the whole device
 * @details Read "/Doc/Note/Self test procedure.txt" for furher informations.
 */
void	self_test_execute	(void);

#endif // SELF_TEST_HEADER_THOMAS_CHEVALIER
