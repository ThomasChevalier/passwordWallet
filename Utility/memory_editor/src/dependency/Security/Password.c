
#include "Password.h"

#include <string.h>

#include "../Globals.h"

#include "../../Aes_transition.h"
#include "../../Random.h"

 

void password_write_id(f_addr_t framAddr, p_addr id)
{
#if p_addr_sz == 1
	fram_write_byte(framAddr, id);
#else
	fram_write_bytes(framAddr, (uint8_t*)(&id), p_addr_sz);
#endif
}

#if p_addr_sz > 1
p_addr password_read_id(f_addr_t framAddr)
{
	p_addr id;
	fram_read_bytes(framAddr, (uint8_t*)(&id), p_addr_sz);
	return id;
}
#endif

static void read_and_decrypt(uint8_t *output, f_addr_t addr_iv, f_addr_t addr_aes, uint8_t lenght_aes, uint8_t* key)
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
static void encrypt_and_write(uint8_t *input, uint8_t len, f_addr_t addr_iv, f_addr_t addr_aes, uint8_t lenght_aes, uint8_t *key)
{
	uint8_t iv[16];

	for(uint8_t i = 0; i < 16; ++i)
	{
		iv[i] = random_request_byte();
		 
	}

	AES128_CBC_encrypt_buffer(input, lenght_aes, key, iv);

	 

	fram_write_bytes(addr_iv, iv, 16);
	fram_write_bytes(addr_aes, input, lenght_aes);
}

void password_read_data(p_addr pwd_id, uint8_t* dst, uint8_t* key)
{
	const f_addr_t pwd_iv_begin  = PWD_ADDR(pwd_id, PWD_OFFSET_PWD_IV);
	const f_addr_t pwd_aes_begin = PWD_ADDR(pwd_id, PWD_OFFSET_PWD_DATA);
	read_and_decrypt(dst, pwd_iv_begin, pwd_aes_begin, 32, key);
}

void password_read_usr_name(p_addr pwd_id, uint8_t* dst, uint8_t* key)
{
	read_and_decrypt(dst, PWD_ADDR(pwd_id, PWD_OFFSET_USR_IV), PWD_ADDR(pwd_id, PWD_OFFSET_USR_NAME), 64, key);
}

void password_read_name(p_addr pwd_id, uint8_t* dst)
{
	fram_read_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_NAME), dst, 32);
}

p_addr password_read_prev_pwd_use(p_addr pwd_id)
{
	return password_read_id(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_USE));
}

p_addr password_read_next_pwd_use(p_addr pwd_id)
{
	return password_read_id(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_USE));
}

p_addr password_read_prev_pwd_alpha(p_addr pwd_id)
{
	return password_read_id(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_ALPHA));
}

p_addr password_read_next_pwd_alpha(p_addr pwd_id)
{
	return password_read_id(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_ALPHA));
}

uint16_t password_read_counter(p_addr pwd_id)
{
	uint16_t count = 0;
	fram_read_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_COUNT), (uint8_t*)&count, 2);
	return count;
}

// Progress complexity = 21
void password_set_data(p_addr pwd_id, uint8_t* pwd, uint8_t* key)
{
	encrypt_and_write(pwd, 32, PWD_ADDR(pwd_id, PWD_OFFSET_PWD_IV), PWD_ADDR(pwd_id, PWD_OFFSET_PWD_DATA), 32, key);
}

// Progress complexity = 21
void password_set_usr_name(p_addr pwd_id, uint8_t* usr, uint8_t* key)
{
	encrypt_and_write(usr, 64, PWD_ADDR(pwd_id, PWD_OFFSET_USR_IV), PWD_ADDR(pwd_id, PWD_OFFSET_USR_NAME), 64, key);
}
	
void password_set_name(p_addr pwd_id, uint8_t* name)
{
	fram_write_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_NAME), (uint8_t*)name, 32);
}

void password_set_prev_pwd_use(p_addr pwd_id, p_addr val)
{
	password_write_id(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_USE), val);
}

void password_set_next_pwd_use(p_addr pwd_id, p_addr val)
{
	password_write_id(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_USE), val);
}

void password_set_prev_pwd_alpha(p_addr pwd_id, p_addr val)
{
	password_write_id(PWD_ADDR(pwd_id, PWD_OFFSET_PREV_PWD_ALPHA), val);
}

void password_set_next_pwd_alpha(p_addr pwd_id, p_addr val)
{
	password_write_id(PWD_ADDR(pwd_id, PWD_OFFSET_NEXT_PWD_ALPHA), val);
}

void password_set_counter(p_addr pwd_id, uint16_t val)
{
	fram_write_bytes(PWD_ADDR(pwd_id, PWD_OFFSET_PWD_COUNT), (uint8_t*)&val, 2);
}

void password_increment_counter(p_addr pwd_id)
{
	const uint16_t counter = password_read_counter(pwd_id);
	password_set_counter(pwd_id, counter + 1);
}

// Progress complexity = 32 + 21 = 53
void password_regenerate(p_addr pwd_id)
{
	uint8_t newPwd[32];
	for(uint8_t i = 0; i < 32; ++i)
	{
		newPwd[i] = random_request_printable();
		 
	}
	password_set_data(pwd_id, newPwd, KEY);
}
