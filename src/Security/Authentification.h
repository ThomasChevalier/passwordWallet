#ifndef AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
#define AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Try to authenticate on the rfid tag. Return 1 if success, 0 otherwise
uint8_t authenticate_on_card(void);

// Change the key used to encrypt the fram memory
void change_master_key(void);

#endif // AUTHENTIFICATION_HEADER_THOMAS_CHEVALIER
