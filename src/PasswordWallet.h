#ifndef PASSWORD_WALLET_HEADER_THOMAS_CHEVALIER
#define PASSWORD_WALLET_HEADER_THOMAS_CHEVALIER

#include <stdint.h>



// PASSWORD

void next_password();
void prev_password();
void send_password();
void decrypt_password(uint16_t* start, uint16_t* end);
#endif // PASSWORD_WALLET_HEADER_THOMAS_CHEVALIER