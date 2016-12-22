#include "Passwords.h"

#include <string.h>

#include "Globals.h"
#include "Aes.h"
#include "Random.h"

void read_password()
{
	const uint16_t pwd_iv_begin  = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_IV);
	const uint16_t pwd_aes_begin = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_DATA);
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
	const uint16_t pwd_iv_begin  = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_IV);
	const uint16_t pwd_aes_begin = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_DATA);
	uint8_t iv[16];
	uint8_t aes[32];
	uint8_t i = 0;

	memcpy(CURRENT_PASSWORD_DATA, password, pwd_len);

	// padding
	if(pwd_len < 32)
	{
		const uint8_t randByteLessThan32 = random_request_byte() >> 3;
		CURRENT_PASSWORD_DATA[pwd_len] = randByteLessThan32;
		for(i = pwd_len + 1; i < 32; ++i)
		{
			CURRENT_PASSWORD_DATA[i] = random_request_byte();
		}
	}

	random_fill(iv, 16);
	AES128_CBC_encrypt_buffer(aes, CURRENT_PASSWORD_DATA, 32, KEY, iv);
	fram_write_bytes(pwd_iv_begin, iv, 16);
	fram_write_bytes(pwd_aes_begin, aes, 32);

	memcpy(CURRENT_PASSWORD_DATA, password, pwd_len);
	for(uint8_t i = pwd_len; i < 32; ++i)
	{
		CURRENT_PASSWORD_DATA[i] = 0;
	}
}

void set_username(uint8_t* usr_name, uint8_t usr_len)
{
	const uint16_t usr_iv_begin  = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_USR_IV);
	const uint16_t usr_aes_begin = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_USR_NAME);
	uint8_t iv[16];
	uint8_t aes[64];
	uint8_t i = 0;

	memcpy(CURRENT_USR_NAME, usr_name, usr_len);

	// padding
	if(usr_len < 64)
	{
		const uint8_t randByteLessThan32 = random_request_byte() >> 3;
		CURRENT_USR_NAME[usr_len] = randByteLessThan32;
		for(i = usr_len + 1; i < 64; ++i)
		{
			CURRENT_USR_NAME[i] = random_request_byte();
		}
	}

	random_fill(iv, 16);
	AES128_CBC_encrypt_buffer(aes, CURRENT_USR_NAME, 64, KEY, iv);
	fram_write_bytes(usr_iv_begin, iv, 16);
	fram_write_bytes(usr_aes_begin, aes, 64);

	memcpy(CURRENT_USR_NAME, usr_name, usr_len);
	for(uint8_t i = usr_len; i < 64; ++i)
	{
		CURRENT_USR_NAME[i] = 0;
	}
}

// Return the address of the first chunk used (from memory map info), beginning to index (included)
// Return MAXIMUM_NUMBER_OF_PWD if there is no entry
static uint8_t getFirstEntryFrom(const uint8_t index)
{
	for(uint8_t i = index / 8; i < MEMORY_MAP_SIZE; ++i)
	{
		// Begin at index
		uint8_t j = (i==index/8) ? index % 8 : 0;

		for(; j < 8; ++j)
		{
			if(i*8+j > MAXIMUM_NUMBER_OF_PWD)
				return MAXIMUM_NUMBER_OF_PWD;
			if(MEMORY_MAP[i] & (1<<j)) // The bit is set, we have found a chunk
			{
				return i*8+j;
			}

		}
	}
	return MAXIMUM_NUMBER_OF_PWD;
}

void goto_first_pwd()
{
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
	{
		CURRENT_PASSWORD_ID = getFirstEntryFrom(0);
	}
	else if(sortingMethod == 1)
	{
		CURRENT_PASSWORD_ID = FIRST_PWD_UTIL;
	}
	else if(sortingMethod == 2)
	{
		CURRENT_PASSWORD_ID = FIRST_PWD_ALPHA;
	}
}

// Count from up to 0, [up;0]
#define COUNT_DOWN_LOOP_BEG(variable, up) for(uint8_t variable = 0; variable <= up; ++variable){variable = up - variable;
#define COUNT_DOWN_LOOP_END(variable, up) variable = up - variable;}

uint16_t prev_pwd(uint16_t pwd_index)
{
	uint16_t pwdAddr = 0;
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
	{
		// The following code try to find a chunk used, before the pwd_index.
		// If there is no such chunk, it loop and search from the end of the memory.

		const uint8_t byteAddr = pwd_index / 8;

		// I had a very strange bug that freeze the µC if i wrote a for like this : for(uint8_t i = byteAddr; i >= 0; --i)
		COUNT_DOWN_LOOP_BEG(i, byteAddr)
			COUNT_DOWN_LOOP_BEG(j, ((i==byteAddr) ? pwd_index % 8 : 7))

				if(MEMORY_MAP[i] && (1<<j)) // The bit is set, we have found a chunk
				{
					pwdAddr = i*8+j;
					goto loopExit_1;
				}

			COUNT_DOWN_LOOP_END(j, ((i==byteAddr) ? pwd_index % 8 : 7))
		COUNT_DOWN_LOOP_END(i, byteAddr)

		loopExit_1:; // This ';' is not a mistake

		if(pwdAddr == pwd_index) // No previous password
		{
			COUNT_DOWN_LOOP_BEG(i, (MEMORY_MAP_SIZE-1))
				COUNT_DOWN_LOOP_BEG(j, 7)
					if(i*8+j < MAXIMUM_NUMBER_OF_PWD)
					{
						if(MEMORY_MAP[i] && (1<<j)) // The bit is set, we have found a chunk
						{
							pwdAddr = i*8+j;
							goto loopExit_2;
						}
					}
				COUNT_DOWN_LOOP_END(j, 7)
			COUNT_DOWN_LOOP_END(i, (MEMORY_MAP_SIZE-1))
			loopExit_2:;
		}
		
	}
	else if(sortingMethod == 1)
	{
		fram_read_bytes( PWD_ADDR(pwd_index, PWD_OFFSET_PREV_PWD_UTIL), (uint8_t*)(&pwdAddr), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes( PWD_ADDR(pwd_index, PWD_OFFSET_PREV_PWD_ALPHA), (uint8_t*)(&pwdAddr), 2);
	}
	return pwdAddr;
}

uint16_t next_pwd(uint16_t pwd_index)
{
	uint16_t pwdAddr = 0;
	uint8_t sortingMethod = (OPTIONS_FLAG >> 1) & 0x03;
	if(sortingMethod == 0)
	{
		// Search for a new entry
		pwd_index = getFirstEntryFrom(pwd_index+1);
		// Loop if not found
		if(pwd_index == MAXIMUM_NUMBER_OF_PWD) // There is always at least one password
			pwd_index = getFirstEntryFrom(0);
	}
	else if(sortingMethod == 1)
	{
		fram_read_bytes(PWD_ADDR(pwd_index, PWD_OFFSET_NEXT_PWD_UTIL), (uint8_t*)(&pwdAddr), 2);
	}
	else if(sortingMethod == 2)
	{
		fram_read_bytes(PWD_ADDR(pwd_index, PWD_OFFSET_NEXT_PWD_ALPHA), (uint8_t*)(&pwdAddr), 2);
	}
	return pwdAddr;
}


void increment_pwd_counter()
{
	uint16_t count;
	fram_read_bytes(PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_COUNT), (uint8_t*)(&count), 2);
	++count;
	fram_write_bytes(PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_COUNT), (uint8_t*)(&count), 2);
}

void set_pwd_name(char* pwd_name)
{
	fram_write_bytes(PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_NAME), (uint8_t*)pwd_name, 32);
}

void read_pwd_name(char* pwd_name, uint16_t pwd_index)
{
	fram_read_bytes(PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_PWD_NAME), (uint8_t*)pwd_name, 32);
}

void read_usr_name()
{
	const uint16_t usr_iv_begin  = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_USR_IV);
	const uint16_t usr_aes_begin = PWD_ADDR(CURRENT_PASSWORD_ID, PWD_OFFSET_USR_NAME);
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
	for(; i < 31; ++i)
		output[i] = random_request_printable();
	output[31] = 0;
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

uint8_t add_password(char* passwordName, char* passwordData, char* userName)
{
	// Read the memory map
    fram_read_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, MEMORY_MAP_SIZE);

    uint8_t chunkFree = 0;
    uint8_t chunk_id = 0;
    for(uint8_t i = 0; i < MEMORY_MAP_SIZE; ++i)
    {
    	// Loop in all the bit of the bytes
    	for(uint8_t j = 0; j < 8; ++j)
    	{
    		if(i * 8 + j > MAXIMUM_NUMBER_OF_PWD)
    			break;

    		if( (MEMORY_MAP[i] & (1<<j))== 0) // the bit is not set, there is a free chunk
    		{
				chunkFree = 1;
				chunk_id = i * 8 + j;
				MEMORY_MAP[i] &= (1<<j); // Set the bit, this chunk is now used
    		}
    	}
    }
    // Error, no enough memory
    if(!chunkFree)
    {
        return 1;
    }
    

    uint8_t save_options_flag = OPTIONS_FLAG;

    // Prev Pwd Util
    OPTIONS_FLAG = 0x02; // Sort by number of utilisations.
	uint8_t prevPwd = 0;
    if(NUM_PWD != 0) // If this is NOT the first pwd
    {
    	prevPwd = prev_pwd(FIRST_PWD_UTIL); // prev_pwd should loop.
    }
    fram_write_byte( PWD_ADDR(chunk_id,PWD_OFFSET_PREV_PWD_UTIL), prevPwd);

    // Next Pwd Util
    fram_write_byte( PWD_ADDR(chunk_id,PWD_OFFSET_NEXT_PWD_UTIL), FIRST_PWD_UTIL);

    // Prev Pwd Alpha
    OPTIONS_FLAG = 0x04; // Sort by alphabet.
	prevPwd = 0;
    if(NUM_PWD != 0) // If this is NOT the first pwd
    {
    	prevPwd = prev_pwd(FIRST_PWD_ALPHA); // prev_pwd should loop.
    }
    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PREV_PWD_ALPHA), prevPwd);

    // Next Pwd Alpha
    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_NEXT_PWD_ALPHA), FIRST_PWD_ALPHA);

    // Restore options flag.
    OPTIONS_FLAG = save_options_flag;

    // Pwd count
    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_COUNT) , 0);
    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_COUNT) + 1, 0);

    // Pwd name
    fram_write_bytes( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_NAME), (uint8_t*)(passwordName), 32);

    // Pwd iv and data
   	uint8_t save_current_pwd = CURRENT_PASSWORD_ID;
   	set_password((uint8_t *)(passwordData), strlen(passwordData)); 

   	// Usr iv and name
   	set_username((uint8_t*)(userName), strlen(userName));

   	//Restore current pwd id
   	CURRENT_PASSWORD_ID = save_current_pwd;

   	// Incremenet the number of password stored.
    ++ NUM_PWD;
    fram_write_byte(OFFSET_NUM_PWD, NUM_PWD);
    return 0;

}
