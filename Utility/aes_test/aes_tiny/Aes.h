#ifndef AES_HEADER_THOMAS_CHEVALIER
#define AES_HEADER_THOMAS_CHEVALIER

// From https://github.com/kokke/tiny-AES128-C

#include <stdint.h>

/**
 * @brief Encrypt a buffer with AES 128 bit in CBC mode.
 * @details String length must be evenly divisible by 16 bytes (str_len % 16 == 0)
 * You should pad the end of the string if this is not the case.
 * 
 * @param output Should be an allocated array of [lenght] bytes. The result of the chiper is stored there.
 * @param input Should be an allocated array of [lenght] bytes. The plain text to be encrypted.
 * @param length The size in bytes of output and input, should be a multiple of 16.
 * @param key An array of bytes ontains the key used to encrypt the data. Should be 16 bytes long.
 * @param iv The initial vector. Should be 16 bytes long.
 */
void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

/**
 * @brief Dencrypt a buffer with AES 128 bit in CBC mode.
 * @details String length must be evenly divisible by 16 bytes (str_len % 16 == 0)
 * You should pad the end of the string if this is not the case.
 * 
 * @param output Should be an allocated array of [lenght] bytes. Plain text is stored there.
 * @param input Should be an allocated array of [lenght] bytes. The chipered text to be decrypted.
 * @param length The size in bytes of output and input, should be a multiple of 16.
 * @param key An array of bytes ontains the key used to encrypt the data. Should be 16 bytes long.
 * @param iv The initial vector. Should be 16 bytes long.
 */
void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);


#endif // AES_HEADER_THOMAS_CHEVALIER
