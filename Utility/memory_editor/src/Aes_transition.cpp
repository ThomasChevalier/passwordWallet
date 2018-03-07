#include "Aes_transition.h"

#include "Aes.h"


void AES128_CBC_encrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv)
{
	uint8_t *output = new uint8_t[length];
	AES128_CBC_encrypt_buffer(output, input, length, key, iv);
	for(int i(0); i < length; ++i){
		input[i] = output[i];
	}
}

void AES128_CBC_decrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv)
{
	uint8_t *output = new uint8_t[length];
	AES128_CBC_decrypt_buffer(output, input, length, key, iv);
	for(int i(0); i < length; ++i){
		input[i] = output[i];
	}
}

