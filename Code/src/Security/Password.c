
#include "Password.h"

#include <string.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"

#include "../Graphics/ProgressBar.h"

#include "../Hardware/Fram.h"

static void read_and_decrypt(uint8_t *output, uint16_t addr_iv, uint16_t addr_aes, uint8_t lenght_aes, uint8_t* key)
{
	uint8_t iv[16];
	fram_read_bytes(addr_iv, iv, 16);
	fram_read_bytes(addr_aes, output, lenght_aes);
	AES128_CBC_decrypt_buffer(output, lenght_aes, key, iv);
	// Remove padding
	uint8_t i = 0;
	uint8_t j = 0;
	for(; i < lenght_aes; ++i)
	{
		if(output[i] < 32)
			j=1;
		if(j)
			output[i] = 0;
	}
}

// Progress complexity = 21
static void encrypt_and_write(uint8_t *input, uint8_t len, uint16_t addr_iv, uint16_t addr_aes, uint8_t lenght_aes, uint8_t *key)
{
	uint8_t iv[16];

	// Random padding
	// if(len < lenght_aes)
	// {
	// 	const uint8_t randByteLessThan32 = 0;
	// 	input[len] = randByteLessThan32;
	// 	for(uint8_t i = len + 1; i < lenght_aes; ++i)
	// 	{
	// 		input[i] = random_request_byte();
	// 		progress_add(1);
	// 	}
	// }

	// Zero padding
	// for(; len < lenght_aes; ++len)
	// {
	// 	input[len] = 0;
	// }

	for(uint8_t i = 0; i < 16; ++i)
	{
		iv[i] = random_request_byte();
		progress_add(1);
	}

	AES128_CBC_encrypt_buffer(input, lenght_aes, key, iv);

	progress_add(5);

	fram_write_bytes(addr_iv, iv, 16);
	fram_write_bytes(addr_aes, input, lenght_aes);
}

void password_read_data(uint8_t pwd_id, uint8_t* dst, uint8_t* key)
{
	const uint16_t pwd_iv_begin  = PWD_ADDR(pwd_id, PWD_OFFSET_PWD_IV);
	const uint16_t pwd_aes_begin = PWD_ADDR(pwd_id, PWD_OFFSET_PWD_DATA);
	read_and_decrypt(dst, pwd_iv_begin, pwd_aes_begin, 32, key);
}

void password_read_usr_name(uint8_t pwd_id, uint8_t* dst, uint8_t* key)
{
	read_and_decrypt(dst, PWD_ADDR(pwd_id, PWD_OFFSET_USR_IV), PWD_ADDR(pwd_id, PWD_OFFSET_USR_NAME), 64, key);
}

void password_read_name(uint8_t pwd_id, uint8_t* dst)
{
	fram_read_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_NAME), dst, 32);
}

uint8_t password_read_prev_pwd_use(uint8_t pwd_id)
{
	return fram_read_byte(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_USE));
}

uint8_t password_read_next_pwd_use(uint8_t pwd_id)
{
	return fram_read_byte(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_USE));
}

uint8_t password_read_prev_pwd_alpha(uint8_t pwd_id)
{
	return fram_read_byte(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_ALPHA));
}

uint8_t password_read_next_pwd_alpha(uint8_t pwd_id)
{
	return fram_read_byte(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_ALPHA));
}

uint16_t password_read_counter(uint8_t pwd_id)
{
	uint16_t count = 0;
	fram_read_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_COUNT), (uint8_t*)&count, 2);
	return count;
}

// Progress complexity = 21
void password_set_data(uint8_t pwd_id, uint8_t* pwd, uint8_t* key)
{
	encrypt_and_write(pwd, 32, PWD_ADDR(pwd_id, PWD_OFFSET_PWD_IV), PWD_ADDR(pwd_id, PWD_OFFSET_PWD_DATA), 32, key);
}

// Progress complexity = 21
void password_set_usr_name(uint8_t pwd_id, uint8_t* usr, uint8_t* key)
{
	encrypt_and_write(usr, 64, PWD_ADDR(pwd_id, PWD_OFFSET_USR_IV), PWD_ADDR(pwd_id, PWD_OFFSET_USR_NAME), 64, key);
}
	
void password_set_name(uint8_t pwd_id, uint8_t* name)
{
	fram_write_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_NAME), (uint8_t*)name, 32);
}

void password_set_prev_pwd_use(uint8_t pwd_id, uint8_t val)
{
	fram_write_byte(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_USE), val);
}

void password_set_next_pwd_use(uint8_t pwd_id, uint8_t val)
{
	fram_write_byte(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_USE), val);
}

void password_set_prev_pwd_alpha(uint8_t pwd_id, uint8_t val)
{
	fram_write_byte(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_ALPHA), val);
}

void password_set_next_pwd_alpha(uint8_t pwd_id, uint8_t val)
{
	fram_write_byte(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_ALPHA), val);
}

void password_set_counter(uint8_t pwd_id, uint16_t val)
{
	fram_write_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_COUNT), (uint8_t*)&val, 2);
}

void password_increment_counter(uint8_t pwd_id)
{
	const uint16_t counter = password_read_counter(pwd_id);
	password_set_counter(pwd_id, counter + 1);
}

// Progress complexity = 32 + 21 = 53
void password_regenerate(uint8_t pwd_id)
{
	uint8_t newPwd[32];
	for(uint8_t i = 0; i < 32; ++i)
	{
		newPwd[i] = random_request_printable();
		progress_add(1);
	}
	password_set_data(pwd_id, newPwd, KEY);
}
