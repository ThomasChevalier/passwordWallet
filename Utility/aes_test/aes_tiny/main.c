#include <stdio.h>
#include <stdint.h>

#include "Aes.h"

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

int main()
{
	uint8_t plain[16];
	uint8_t key[16] = {0};
	uint8_t cipher[16];
	uint8_t iv[16];
	uint8_t i = 0;
	for(; i < 16; ++i)
	{
		plain[i] = i;
		iv[15-i]=i;
	}

	printf("Key :\n");
	print_hex(key, 16);
	printf("Iv :\n");
	print_hex(iv, 16);
	printf("Plain :\n");
	print_hex(plain, 16);

	AES128_CBC_encrypt_buffer(cipher, plain, 16, key, iv);

	printf("Cipher :\n");
	print_hex(cipher, 16);

	return 0;
} 
