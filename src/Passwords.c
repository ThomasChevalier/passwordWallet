#include "Passwords.h"

#include "Aes.h"

void read_password()
{
	const uint16_t pwd_iv_begin  = FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 42;
	const uint16_t pwd_aes_begin = pwd_iv_begin + 16;
	uint8_t iv[16];
	uint8_t aes[32];
	fram_read_bytes(pwd_iv_begin, iv, 16);
	fram_read_bytes(pwd_aes_begin, aes, 32);

	AES128_CBC_decrypt_buffer(CURRENT_PASSWORD_DATA+0, aes+0, 16, KEY, iv);
	AES128_CBC_decrypt_buffer(CURRENT_PASSWORD_DATA+16, aes+16, 16, 0, 0);
}

void goto_first_password()
{

}

void goto_prev_pwd()
{
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
		CURRENT_PASSWORD_ID = (CURRENT_PASSWORD_ID == 0) ? NUM_PWD : CURRENT_PASSWORD_ID-1;
	else if(sortingMethod == 1)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 0, (uint8_t*)(&CURRENT_PASSWORD_ID), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 4, (uint8_t*)(&CURRENT_PASSWORD_ID), 2);
	}
}

void goto_next_pwd()
{
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
		CURRENT_PASSWORD_ID ++;
	else if(sortingMethod == 1)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 2, (uint8_t*)(&CURRENT_PASSWORD_ID), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 6, (uint8_t*)(&CURRENT_PASSWORD_ID), 2);
	}
}

void increment_pwd_counter()
{
	uint16_t count;
	fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 8, (uint8_t*)(&count), 2);
	++count;
	fram_write_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 8, (uint8_t*)(&count), 2);
}

void generate_password(char* output)
{

}

void change_password()
{

}

void change_username()
{

}

void delete_password()
{

}

void add_password()
{

}
