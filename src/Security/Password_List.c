#include "Password_List.h"

#include <string.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"
#include "Password.h"

#include "../Graphics/ProgressBar.h"

#include "../Hardware/Fram.h"



// Return the address of the first chunk used (from memory map info), beginning to index (included)
// Return MAXIMUM_NUMBER_OF_PWD if there is no entry
static uint8_t getFirstEntryFrom(const uint8_t index)
{
	for(uint8_t i = index / 8; i < SIZE_MEMORY_MAP; ++i)
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
			COUNT_DOWN_LOOP_BEG(i, (SIZE_MEMORY_MAP-1))
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
			COUNT_DOWN_LOOP_END(i, (SIZE_MEMORY_MAP-1))
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
	uint8_t sortingMethod = ((OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L)) + (OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H))) >> OPTIONS_FLAG_OFFSET_SORTING_METHOD_L;
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


void read_all_names(void)
{
	password_read_name (	prev_pwd(CURRENT_PASSWORD_ID), 	(uint8_t*)PWD_NAME_1);
	password_read_name (	CURRENT_PASSWORD_ID, 			(uint8_t*)PWD_NAME_2);
	password_read_name (	next_pwd(CURRENT_PASSWORD_ID), 	(uint8_t*)PWD_NAME_3);
}

// Progress complexity = 31
void generate_password(char* output)
{
	uint8_t i = 0;
	for(; i < 31; ++i)
	{
		output[i] = random_request_printable();
		progress_add(1);
	}
	output[31] = 0;
} 


void delete_password(void)
{

}

uint8_t add_password(char* passwordName, char* passwordData, char* userName)
{
	return 1;
	// // Read the memory map
 //    fram_read_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, SIZE_MEMORY_MAP);

 //    uint8_t chunkFree = 0;
 //    uint8_t chunk_id = 0;
 //    for(uint8_t i = 0; i < SIZE_MEMORY_MAP; ++i)
 //    {
 //    	// Loop in all the bit of the bytes
 //    	for(uint8_t j = 0; j < 8; ++j)
 //    	{
 //    		if(i * 8 + j > MAXIMUM_NUMBER_OF_PWD)
 //    			break;

 //    		if( (MEMORY_MAP[i] & (1<<j))== 0) // the bit is not set, there is a free chunk
 //    		{
	// 			chunkFree = 1;
	// 			chunk_id = i * 8 + j;
	// 			MEMORY_MAP[i] &= (1<<j); // Set the bit, this chunk is now used
	// 			goto CHUNK_FOUND;
 //    		}
 //    	}
 //    }
 //    // Error, no enough memory
 //    if(!chunkFree)
 //    {
 //        return 1;
 //    }
    
 //    CHUNK_FOUND:;

 //    uint8_t save_options_flag = OPTIONS_FLAG;

 //    // Prev Pwd Util
 //    OPTIONS_FLAG = 0x02; // Sort by number of utilisations.
	// uint8_t prevPwd = 0;
 //    if(NUM_PWD != 0) // If this is NOT the first pwd
 //    {
 //    	prevPwd = prev_pwd(FIRST_PWD_UTIL); // prev_pwd should loop.
 //    }
 //    fram_write_byte( PWD_ADDR(chunk_id,PWD_OFFSET_PREV_PWD_UTIL), prevPwd);

 //    // Next Pwd Util
 //    fram_write_byte( PWD_ADDR(chunk_id,PWD_OFFSET_NEXT_PWD_UTIL), FIRST_PWD_UTIL);

 //    // Prev Pwd Alpha
 //    OPTIONS_FLAG = 0x04; // Sort by alphabet.
	// prevPwd = 0;
 //    if(NUM_PWD != 0) // If this is NOT the first pwd
 //    {
 //    	prevPwd = prev_pwd(FIRST_PWD_ALPHA); // prev_pwd should loop.
 //    }
 //    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PREV_PWD_ALPHA), prevPwd);

 //    // Next Pwd Alpha
 //    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_NEXT_PWD_ALPHA), FIRST_PWD_ALPHA);

 //    // Restore options flag.
 //    OPTIONS_FLAG = save_options_flag;

 //    // Pwd count
 //    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_COUNT) , 0);
 //    fram_write_byte( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_COUNT) + 1, 0);

 //    // Pwd name
 //    fram_write_bytes( PWD_ADDR(chunk_id, PWD_OFFSET_PWD_NAME), (uint8_t*)(passwordName), 32);

 //    // Pwd iv and data
 //   	uint8_t save_current_pwd = CURRENT_PASSWORD_ID;
 //   	set_password((uint8_t *)(passwordData), strlen(passwordData), KEY); 

 //   	// Usr iv and name
 //   	set_username((uint8_t*)(userName), strlen(userName), KEY);

 //   	//Restore current pwd id
 //   	CURRENT_PASSWORD_ID = save_current_pwd;

 //   	// Incremenet the number of password stored.
 //    ++ NUM_PWD;
 //    fram_write_byte(OFFSET_NUM_PWD, NUM_PWD);
 //    return 0;
}
