#include "Passwords.h"

#include "Aes.h"
#include "Random.h"

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

	// Remove padding
	uint8_t i = 0;
	uint8_t j = 0;
	for(; i < 32; ++i)
	{
		if(CURRENT_PASSWORD_DATA[i] < 32)
			j=1;
		if(j)
			CURRENT_PASSWORD_DATA[i] = 0;
	}
}

void set_password(uint8_t* password, uint8_t pwd_len)
{
	const uint16_t pwd_iv_begin  = FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 42;
	const uint16_t pwd_aes_begin = pwd_iv_begin + 16;
	uint8_t iv[16];
	uint8_t aes[32];
	uint8_t i = 0;

	for(i = 0; i < pwd_len; ++i)
	{
		CURRENT_PASSWORD_DATA[i] = password[i];
	}

	// padding
	if(pwd_len < 32)
	{
		CURRENT_PASSWORD_DATA[pwd_len] = 31-pwd_len;
		for(i = pwd_len + 1; i < 32; ++i)
		{
			CURRENT_PASSWORD_DATA[i] = random_request_byte();
		}
	}

	random_fill(iv, 16);
	AES128_CBC_encrypt_buffer(aes, CURRENT_PASSWORD_DATA, 32, KEY, iv);
	fram_write_bytes(pwd_iv_begin, iv, 16);
	fram_write_bytes(pwd_aes_begin, aes, 32);
}

void goto_first_pwd()
{
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
		CURRENT_PASSWORD_ID = 0;
	else if(sortingMethod == 1)
	{
		CURRENT_PASSWORD_ID = FIRST_PWD_UTIL;
	}
	else if(sortingMethod == 2)
	{
		CURRENT_PASSWORD_ID = FIRST_PWD_ALPHA;
	}
}

uint16_t prev_pwd(uint16_t pwd_index)
{
	uint16_t pwdAddr = 0;
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
		pwd_index = (pwd_index == 0) ? NUM_PWD : pwd_index-1;
	else if(sortingMethod == 1)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwd_index + 0, (uint8_t*)(&pwdAddr), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwd_index + 4, (uint8_t*)(&pwdAddr), 2);
	}
	return pwdAddr;
}

uint16_t next_pwd(uint16_t pwd_index)
{
	uint16_t pwdAddr = 0;
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
		pwd_index = (pwd_index == NUM_PWD) ? 0 : pwd_index + 1;
	else if(sortingMethod == 1)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwd_index + 2, (uint8_t*)(&pwdAddr), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwd_index + 6, (uint8_t*)(&pwdAddr), 2);
	}
	return pwdAddr;
}


void increment_pwd_counter()
{
	uint16_t count;
	fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 8, (uint8_t*)(&count), 2);
	++count;
	fram_write_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 8, (uint8_t*)(&count), 2);
}

void set_pwd_name(char* pwd_name)
{
	fram_write_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 10, (uint8_t*)pwd_name, 32);
}

void read_pwd_name(char* pwd_name, uint16_t pwd_index)
{
	fram_read_bytes(FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwd_index + 10, (uint8_t*)pwd_name, 32);
}

void read_usr_name()
{
	const uint16_t usr_iv_begin  = FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * CURRENT_PASSWORD_ID + 90;
	const uint16_t usr_aes_begin = usr_iv_begin + 16;
	uint8_t iv[16];
	uint8_t aes[64];
	fram_read_bytes(usr_iv_begin, iv, 16);
	fram_read_bytes(usr_aes_begin, aes, 64);

	AES128_CBC_decrypt_buffer(CURRENT_USR_NAME+0, aes+0, 16, KEY, iv);
	AES128_CBC_decrypt_buffer(CURRENT_USR_NAME+16, aes+16, 16, 0, 0);
	AES128_CBC_decrypt_buffer(CURRENT_USR_NAME+32, aes+32, 32, 0, 0);
	AES128_CBC_decrypt_buffer(CURRENT_USR_NAME+48, aes+48, 48, 0, 0);

	// Remove padding
	uint8_t i = 0;
	uint8_t j = 0;
	for(; i < 64; ++i)
	{
		if(CURRENT_USR_NAME[i] < 32)
			j=1;
		if(j)
			CURRENT_USR_NAME[i] = 0;
	}
}

void read_all_names()
{
	read_pwd_name(PWD_NAME_1, prev_pwd(CURRENT_PASSWORD_ID));
	read_pwd_name(PWD_NAME_2, CURRENT_PASSWORD_ID);
	read_pwd_name(PWD_NAME_3, next_pwd(CURRENT_PASSWORD_ID));
}

void generate_password(char* output)
{
	uint8_t i = 0;
	for(; i < 32; ++i)
		output[i] = random_request_printable();
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
