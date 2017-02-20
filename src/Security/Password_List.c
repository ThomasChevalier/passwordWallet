#include "Password_List.h"

#include <string.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"
#include "Password.h"

#include "../Graphics/ProgressBar.h"

#include "../Hardware/Fram.h"

static uint8_t get_sorting_method(void)
{
	return (OPTIONS_FLAG & ((1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L)|(1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H))) >> OPTIONS_FLAG_OFFSET_SORTING_METHOD_L;
}

// Return the address of the first chunk used (from memory map info), beginning to index (included)
// Loop if nothing found
// Return index if there is no entry
static uint8_t get_first_entry_from(const uint8_t index)
{
	for(uint8_t i = index / 8; i < SIZE_MEMORY_MAP; ++i)
	{
		// Begin at index
		uint8_t j = (i==index/8) ? index % 8 : 0;

		for(; j < 8; ++j)
		{
			if(i*8+j >= MAXIMUM_NUMBER_OF_PWD)
			{
				goto nothingFound;
			}
			if(MEMORY_MAP[i] & (1<<j)) // The bit is set, we have found a chunk
			{
				return i*8+j;
			}

		}
	}

	nothingFound:;
	if(index != 0)
	{
		return get_first_entry_from(0);
	}
	return index;
}

uint8_t pwd_list_get_first_pwd_id (void)
{
	uint8_t sortingMethod = get_sorting_method();
	if(sortingMethod == 0)
	{
		return pwd_list_get_first_pwd_id_sort_none();
	}
	else if(sortingMethod == 1)
	{
		return pwd_list_get_first_pwd_id_sort_usage();
	}
	else if(sortingMethod == 2)
	{
		return pwd_list_get_first_pwd_id_sort_alpha();
	}
	// Impossible
	return 0;
}

uint8_t pwd_list_get_first_pwd_id_sort_none (void)
{
	return get_first_entry_from(0);
}

uint8_t pwd_list_get_first_pwd_id_sort_usage (void)
{
	return FIRST_PWD_UTIL;
}

uint8_t pwd_list_get_first_pwd_id_sort_alpha (void)
{
	return FIRST_PWD_ALPHA;
}


uint8_t pwd_list_get_prev_pwd_id (uint8_t pwd_id)
{
	uint8_t sortingMethod = get_sorting_method();
	if(sortingMethod == 0)
	{
		return pwd_list_get_prev_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == 1)
	{
		return pwd_list_get_prev_pwd_id_sort_usage(pwd_id);
	}
	else if(sortingMethod == 2)
	{
		return pwd_list_get_prev_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

uint8_t pwd_list_get_prev_pwd_id_sort_none (uint8_t pwd_id)
{
	// Scans the memory map in reverse
	for(uint8_t i = pwd_id / 8; i != (uint8_t)-1; --i)
	{
		uint8_t j = (i == pwd_id / 8) ? pwd_id % 8 : 7;
		for(; j != (uint8_t)-1; --j)
		{
			if(MEMORY_MAP[i] & (1<<j))
			{
				return i*8+j;
			}
		}
	}
	// No password had been found before pwd_id, so loop (scan from MAXIMUM_NUMBER_OF_PWD to 0)
	if(pwd_id != MAXIMUM_NUMBER_OF_PWD-1)
	{
		return pwd_list_get_prev_pwd_id_sort_none(MAXIMUM_NUMBER_OF_PWD-1);
	}
	return pwd_id;
}

uint8_t pwd_list_get_prev_pwd_id_sort_usage (uint8_t pwd_id)
{
	return password_read_prev_pwd_util(pwd_id);
}

uint8_t pwd_list_get_prev_pwd_id_sort_alpha (uint8_t pwd_id)
{
	return password_read_prev_pwd_alpha(pwd_id);
}


uint8_t pwd_list_get_next_pwd_id (uint8_t pwd_id)
{
	uint8_t sortingMethod = get_sorting_method();
	if(sortingMethod == 0)
	{
		return pwd_list_get_next_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == 1)
	{
		return pwd_list_get_next_pwd_id_sort_usage(pwd_id);
	}
	else if(sortingMethod == 2)
	{
		return pwd_list_get_next_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

uint8_t pwd_list_get_next_pwd_id_sort_none (uint8_t pwd_id)
{
	return get_first_entry_from(pwd_id);
}

uint8_t pwd_list_get_next_pwd_id_sort_usage (uint8_t pwd_id)
{
	return password_read_next_pwd_util(pwd_id);
}

uint8_t pwd_list_get_next_pwd_id_sort_alpha (uint8_t pwd_id)
{
	return password_read_next_pwd_alpha(pwd_id);
}


void pwd_list_delete_pwd (uint8_t pwd_id)
{
	uint8_t prev_util = password_read_prev_pwd_util(pwd_id);
	uint8_t next_util = password_read_next_pwd_util(pwd_id);

	uint8_t prev_alpha = password_read_prev_pwd_alpha(pwd_id);
	uint8_t next_alpha = password_read_next_pwd_alpha(pwd_id);

	// Update the list
	password_set_next_pwd_util(prev_util, next_util);
	password_set_next_pwd_alpha(prev_alpha, next_alpha);

	password_set_prev_pwd_util(next_util, prev_util);
	password_set_prev_pwd_alpha(next_alpha, prev_alpha);

	// Unset memory map flag for this chunk
	MEMORY_MAP[pwd_id / 8] &= ~(1<<(pwd_id%8));
	fram_write_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, SIZE_MEMORY_MAP);

	// There is now one less password
	--NUM_PWD;
	fram_write_byte(OFFSET_NUM_PWD, NUM_PWD);

	// Clear the whole password chunk
	for(uint8_t i = 0; i < SIZE_OF_PWD_BLOCK; ++i)
	{
		fram_write_byte(PWD_ADDR(pwd_id, 0), 0);
	}
}

uint8_t pwd_list_add_pwd (uint8_t* name, uint8_t* data, uint8_t* usrName)
{
	// First check if there is a chunk free
	uint8_t chunk_free = 0;
	uint8_t pwd_id = 0;
	for(uint8_t i = 0; i < SIZE_MEMORY_MAP && (!chunk_free); ++i)
	{
		// Loop in all the bit of the bytes
		for(uint8_t j = 0; j < 8 && (!chunk_free); ++j)
		{
			if(i * 8 + j > MAXIMUM_NUMBER_OF_PWD)
				break;

			if( (MEMORY_MAP[i] & (1<<j))== 0) // the bit is not set, there is a free chunk
			{
				chunk_free = 1;
				pwd_id = i * 8 + j;
				MEMORY_MAP[i] &= (1<<j); // Set the bit, this chunk is now used
				fram_write_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, SIZE_MEMORY_MAP);
			}
		}
	}
	if(!chunk_free)
	{
		return 0; // Not enough memory
	}

	uint8_t prev_util	= 0;
	uint8_t next_util	= 0;
	uint8_t prev_alpha	= 0;
	uint8_t next_alpha	= 0;

	// If this is not the first password
	if(NUM_PWD != 0)
	{
		// The new password became the last of the list
		// The previous element is the last of the list
		// The next element is the begin of the list

		uint8_t last_pwd_util = pwd_list_get_prev_pwd_id_sort_usage(pwd_list_get_first_pwd_id_sort_usage());
		uint8_t last_pwd_alpha = pwd_list_get_prev_pwd_id_sort_alpha(pwd_list_get_first_pwd_id_sort_alpha());

		// Point to the last of the list (before insertion)
		prev_util = last_pwd_util;
		prev_alpha = last_pwd_alpha;

		// Point to the next element of the last of the list (before insertion). This should be the beginning of the list
		next_util = pwd_list_get_first_pwd_id_sort_usage();
		next_alpha = pwd_list_get_first_pwd_id_sort_alpha();

		// Set the last of the list to point to this new pwd
		password_set_next_pwd_util	(prev_util,  pwd_id);
		password_set_next_pwd_alpha	(prev_alpha, pwd_id);

		// Set the first of the list to point to this new pwd
		password_set_prev_pwd_util	(next_util,  pwd_id);
		password_set_prev_pwd_alpha	(next_alpha, pwd_id);
	}

	// Set up list
	password_set_prev_pwd_util	(pwd_id, prev_util);
	password_set_next_pwd_util	(pwd_id, next_util);
	password_set_prev_pwd_alpha	(pwd_id, prev_alpha);
	password_set_next_pwd_alpha	(pwd_id, next_alpha);

	// Set counter to 0
	password_set_counter(pwd_id, 0);

	// Set name, data and user name
	password_set_name		(pwd_id, name,		strlen((char*)name)			);
	password_set_data		(pwd_id, data,		strlen((char*)data),	KEY	);
	password_set_usr_name	(pwd_id, usrName,	strlen((char*)usrName),	KEY	);

	// The password has been added, increment counter
	++NUM_PWD;
    fram_write_byte(OFFSET_NUM_PWD, NUM_PWD);

	// We have inserted the password at the end of each list (alpha and usage) but it is not sorted.
	// We must sort them.
	pwd_list_sort_usage();
	pwd_list_sort_alpha();
	return 1; // Success
}

void pwd_list_swap (uint8_t pwd1, uint8_t pwd2)
{
	// Save values
	const uint8_t prev_util1	= password_read_prev_pwd_util	(pwd1);
	const uint8_t next_util1	= password_read_next_pwd_util	(pwd1);
	const uint8_t prev_alpha1	= password_read_prev_pwd_alpha	(pwd1);
	const uint8_t next_alpha1	= password_read_next_pwd_alpha	(pwd1);

	const uint8_t prev_util2	= password_read_prev_pwd_util	(pwd2);
	const uint8_t next_util2	= password_read_next_pwd_util	(pwd2);
	const uint8_t prev_alpha2	= password_read_prev_pwd_alpha	(pwd2);
	const uint8_t next_alpha2	= password_read_next_pwd_alpha	(pwd2);

	// Exchange values of pwd 1 and pwd 2
	password_set_prev_pwd_util	(pwd1, prev_util2);
	password_set_next_pwd_util	(pwd1, next_util2);
	password_set_prev_pwd_alpha	(pwd1, prev_alpha2);
	password_set_next_pwd_alpha	(pwd1, next_alpha2);

	password_set_prev_pwd_util	(pwd2, prev_util1);
	password_set_next_pwd_util	(pwd2, next_util1);
	password_set_prev_pwd_alpha	(pwd2, prev_alpha1);
	password_set_next_pwd_alpha	(pwd2, next_alpha1);

	// Set the previous and the next password of pwd 1 to point to pwd 2
	password_set_next_pwd_util	(prev_util1,	pwd2);
	password_set_next_pwd_alpha	(prev_alpha1,	pwd2);
	password_set_prev_pwd_util	(next_util1,	pwd2);
	password_set_prev_pwd_alpha	(next_alpha1,	pwd2);

	// Set the previous and the next password of pwd 2 to point to pwd 1
	password_set_next_pwd_util	(prev_util2,	pwd1);
	password_set_next_pwd_alpha	(prev_alpha2,	pwd1);
	password_set_prev_pwd_util	(next_util2,	pwd1);
	password_set_prev_pwd_alpha	(next_alpha2,	pwd1);
}

void pwd_list_sort_usage (void)
{
	if(NUM_PWD < 2)
	{
		return;
	}

	const uint8_t first = pwd_list_get_first_pwd_id_sort_usage();
	uint8_t current = password_read_next_pwd_util(first);
	uint8_t prev = first;

	// The condition will be met because the list loop
	while(current != first)
	{
		uint16_t counter = password_read_counter(current);
		while(counter < password_read_counter(prev))
		{
			pwd_list_swap(current, prev);
			prev = password_read_prev_pwd_util(current);
		}
		current = password_read_next_pwd_util(current);
	}
}

void pwd_list_sort_alpha (void)
{
	if(NUM_PWD < 2)
	{
		return;
	}

	const uint8_t first = pwd_list_get_first_pwd_id_sort_alpha();
	uint8_t current = password_read_next_pwd_alpha(first);
	uint8_t prev = first;

	// The condition will be met because the list loop
	while(current != first)
	{
		char nameCurrent[32];
		password_read_name(current, (uint8_t*)nameCurrent);

		char namePrev[32];
		password_read_name(prev, (uint8_t*)namePrev);

		while(strcmp(nameCurrent, namePrev) < 0)
		{
			pwd_list_swap(current, prev);
			prev = password_read_prev_pwd_alpha(current);
			password_read_name(prev, (uint8_t*) namePrev);
		}
		current = password_read_next_pwd_alpha(current);
	}
}
