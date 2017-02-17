#ifndef USERSETUP_HEADER_THOMAS_CHEVALIER
#define USERSETUP_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Check if the device has been initialized. Return 1 if initialized, 0 otherwise
// Return can be 0 if the user request a reset
uint8_t usr_setup_is_initialized(void);

// Initialize the device. WARNING : all the memory will be erased.
// Return 1 if the device has been initialized, 0 otherwise
uint8_t usr_setup_do_initialization(void);

// Check if the user request to type his key because he lost his card
// Return 1 if he want to type his key, 0 otherwise
uint8_t usr_setup_check_card_lost(void);

// Ask the user for his key
// Return 1 if he typed the right key, 0 otherwise
uint8_t usr_setup_recover_key(void);

#endif // USERSETUP_HEADER_THOMAS_CHEVALIER