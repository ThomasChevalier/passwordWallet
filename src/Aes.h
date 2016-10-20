#ifndef AES_HEADER_THOMAS_CHEVALIER
#define AES_HEADER_THOMAS_CHEVALIER

// From https://github.com/kokke/tiny-AES128-C

#include <stdint.h>

// Input should always be 16 bytes long
void AES128_CBC_encrypt_buffer(uint8_t* output, const uint8_t* input, const uint8_t* key, const uint8_t* iv);
void AES128_CBC_decrypt_buffer(uint8_t* output, const uint8_t* input, const uint8_t* key, const uint8_t* iv);



#endif // AES_HEADER_THOMAS_CHEVALIER
