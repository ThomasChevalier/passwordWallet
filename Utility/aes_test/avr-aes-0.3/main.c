#include <stdio.h>
#include <stdint.h>



#include "aes.h"

void print_hex(uint8_t *hex, uint8_t sz)
{
	uint8_t i = 0;
	for(; i < sz; ++i)
	{
		printf("");
		if(hex[i]<0x10)
			printf("0");
		printf("%x", hex[i]&0xff);
	}
	printf("\n");
}

void mem_xor(uint8_t* out, uint8_t* xo)
{
	uint8_t i = 0;
	for(; i < 16; ++i)
	{
		out[i] ^= xo[i];
	}
}

void AES128_CBC_encrypt_buffer(uint8_t* input, uint8_t sz, uint8_t* key, uint8_t* iv)
{
	uint8_t i = 0;
	for(; i < sz; i+=16)
	{
		mem_xor(input, iv);
		aesCipher(key, input);  // This function return a modified key and accept the original key
		aesKeyRewind(key);  // Convert the modified key to the original
		iv=input;
		input+=16;
	}
}

void AES128_CBC_decrypt_buffer(uint8_t* input, uint8_t sz, uint8_t* key, uint8_t* iv)
{
	uint8_t i = 0;
	printf("Decryption\n");

	uint8_t tempCipher[16];
	uint8_t pos = 0;

	uint8_t *xorWith = iv;

	for(; i < sz; i+=16)
	{	
		uint8_t j = 0;
		if(pos == 0)
		{
			for(; j < 16; ++j)
			{
				tempCipher[j] = input[j];
			}
			pos = 1;
		}
		else
		{
			for(; j < 16; ++j)
			{
				iv[j] = input[j];
			}
			pos = 0;
		}

		aesKeyPatch(key);  // Patch the original key
		aesInvCipher(key, input);  // Decrypt data with a patched key. It return the original key.

		mem_xor(input, xorWith);

		xorWith = (pos == 1) ? tempCipher : iv;
		input+=16;
	}
}

int main()
{
	const uint8_t plainSz = 64;
	uint8_t plain[plainSz];
	uint8_t key[16] = {0};
	uint8_t iv[16];
	uint8_t i = 0;
	for(; i < 16; ++i)
	{
		key[i] = i;
		plain[i] = i;
		plain[i+16] = i;
		plain[i+32] = i*2;
		plain[i+48] = i * 3;
		iv[15-i]=i;
	}

	printf("Key :\n");
	print_hex(key, 16);
	printf("Iv :\n");
	print_hex(iv, 16);
	printf("Plain :\n");
	print_hex(plain, plainSz);

	AES128_CBC_encrypt_buffer(plain, plainSz, key, iv);

	printf("Cipher :\n");
	print_hex(plain, plainSz);

	AES128_CBC_decrypt_buffer(plain, plainSz, key, iv);
	printf("Plain : \n");
	print_hex(plain, plainSz);

	return 0;
} 
