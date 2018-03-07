#ifndef AES_TRANSITION_HEADER_THOMAS_CHEVALIER
#define AES_TRANSITION_HEADER_THOMAS_CHEVALIER

#include <cstdint>

void AES128_CBC_encrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv);
void AES128_CBC_decrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv);

#endif // AES_TRANSITION_HEADER_THOMAS_CHEVALIER