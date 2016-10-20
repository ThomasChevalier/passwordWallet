#ifndef AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
#define AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Wait for a valid rfid card
void wait_for_valid_card();
// Check the main_key buffer against a hash, should return true
uint8_t check_key();

void changeMainPassword();

#endif // AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER