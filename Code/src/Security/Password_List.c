#include "Password_List.h"

#include <string.h>

#include "../Globals.h"

#include "Password.h"

#include "../Graphics/ProgressBar.h"

#include "../Hardware/Fram.h"

uint8_t pwd_list_get_sorting_method(void)
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
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+i);
		// Begin at index
		uint8_t j = (i==index/8) ? index % 8 : 0;

		for(; j < 8; ++j)
		{
			if(i*8+j >= MAXIMUM_NUMBER_OF_PWD)
			{
				goto nothingFound;
			}
			if(memory_byte & (1<<j)) // The bit is set, we have found a chunk
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
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_first_pwd_id_sort_none();
	}
	else if(sortingMethod == PWD_SORTING_USAGE)
	{
		return pwd_list_get_first_pwd_id_sort_usage();
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
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
	return fram_read_byte(OFFSET_FIRST_PWD_UTIL);
}

uint8_t pwd_list_get_first_pwd_id_sort_alpha (void)
{
	return fram_read_byte(OFFSET_FIRST_PWD_ALPHA);
}


uint8_t pwd_list_get_prev_pwd_id (uint8_t pwd_id)
{
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_prev_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_USAGE)
	{
		return pwd_list_get_prev_pwd_id_sort_usage(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
	{
		return pwd_list_get_prev_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

uint8_t pwd_list_get_prev_pwd_id_sort_none (uint8_t pwd_id)
{
	// Scans the memory map in reverse
	// If we do not decrement pwd_id then the function will found pwd_id directly and it will be bad

	// Loop
	uint8_t first_candidate = pwd_id - 1;
	if(pwd_id == 0)
	{
		first_candidate = MAXIMUM_NUMBER_OF_PWD-1;
	}

	for(uint8_t i = first_candidate / 8; i != (uint8_t)-1; --i)
	{
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+i);
		uint8_t j = (i == first_candidate / 8) ? first_candidate % 8 : 7;
		for(; j != (uint8_t)-1; --j)
		{
			if(memory_byte & (1<<j))
			{
				return i*8+j;
			}
		}
	}
	// No password had been found before pwd_id, so loop (scan from MAXIMUM_NUMBER_OF_PWD to 0)
	if(pwd_id != 0) // If pwd_id == 0 then we have already search all the chunk
	{
		uint8_t last_pwd = MAXIMUM_NUMBER_OF_PWD-1;
		for(uint8_t i = last_pwd / 8; i != (uint8_t)-1; --i)
		{
			const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+i);
			uint8_t j = (i == last_pwd / 8) ? last_pwd % 8 : 7;
			for(; j != (uint8_t)-1; --j)
			{
				if(memory_byte & (1<<j))
				{
					return i*8+j;
				}
			}
		}
	}
	// There is no password in the list, return pwd_id
	return pwd_id;
}

static uint8_t get_id_helper(uint8_t pwd_id, uint8_t(*read_first)(uint8_t), uint8_t(*read_second)(uint8_t))
{
	uint8_t id = read_first(pwd_id);
	// This is the beginning of the list
	if(id == MAXIMUM_NUMBER_OF_PWD)
	{
		for(uint8_t i = 0; i < NUM_PWD-1; ++i)
		{
			pwd_id = read_second(pwd_id);
		}
		return pwd_id;
	}
	return id;
}
uint8_t pwd_list_get_prev_pwd_id_sort_usage (uint8_t pwd_id)
{
	return get_id_helper(pwd_id, password_read_prev_pwd_util, password_read_next_pwd_util);
}

uint8_t pwd_list_get_prev_pwd_id_sort_alpha (uint8_t pwd_id)
{
	return get_id_helper(pwd_id, password_read_prev_pwd_alpha, password_read_next_pwd_alpha);
}


uint8_t pwd_list_get_next_pwd_id (uint8_t pwd_id)
{
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_next_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_USAGE)
	{
		return pwd_list_get_next_pwd_id_sort_usage(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
	{
		return pwd_list_get_next_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

uint8_t pwd_list_get_next_pwd_id_sort_none (uint8_t pwd_id)
{
	// Since get_first_entry_from(i) return i if the chunk i is used, we must add 1 to pwd_id
	return get_first_entry_from(pwd_id+1);
}

uint8_t pwd_list_get_next_pwd_id_sort_usage (uint8_t pwd_id)
{
	return get_id_helper(pwd_id, password_read_next_pwd_util, password_read_prev_pwd_util);
}

uint8_t pwd_list_get_next_pwd_id_sort_alpha (uint8_t pwd_id)
{
	return get_id_helper(pwd_id, password_read_next_pwd_alpha, password_read_prev_pwd_alpha);
}

void pwd_list_delete_pwd (uint8_t pwd_id)
{
	uint8_t prev_util = password_read_prev_pwd_util(pwd_id);
	uint8_t next_util = password_read_next_pwd_util(pwd_id);

	uint8_t prev_alpha = password_read_prev_pwd_alpha(pwd_id);
	uint8_t next_alpha = password_read_next_pwd_alpha(pwd_id);

	// First password of the list util
	if(next_util != MAXIMUM_NUMBER_OF_PWD)
	{
		password_set_prev_pwd_util(next_util, prev_util);
	}
	if(prev_util == MAXIMUM_NUMBER_OF_PWD)
	{
		// If it is not the last too
		if(next_util != MAXIMUM_NUMBER_OF_PWD)
		{
			fram_write_byte(OFFSET_FIRST_PWD_UTIL, next_util);
		}
		// It is the last pwd, set offset_first_ to 0
		else
		{
			fram_write_byte(OFFSET_FIRST_PWD_UTIL, 0);
		}
	}
	else
	{
		password_set_next_pwd_util(prev_util, next_util);
	}

	// First password of the list alpha
	if(next_alpha != MAXIMUM_NUMBER_OF_PWD)
	{
		password_set_prev_pwd_alpha(next_alpha, prev_alpha);
	}
	if(prev_alpha == MAXIMUM_NUMBER_OF_PWD)
	{
		// If it is not the last too
		if(next_alpha != MAXIMUM_NUMBER_OF_PWD)
		{
			fram_write_byte(OFFSET_FIRST_PWD_ALPHA, next_alpha);
		}
		// It is the last pwd, set offset_first_ to 0
		else
		{
			fram_write_byte(OFFSET_FIRST_PWD_ALPHA, 0);
		}
	}
	else
	{
		password_set_next_pwd_alpha(prev_alpha, next_alpha);
	}
	
	// Unset memory map flag for this chunk
	const uint8_t mem_byte_id = pwd_id / 8;
	const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+mem_byte_id);
	// Update the byte that has changed
	fram_write_byte(OFFSET_MEMORY_MAP + mem_byte_id, memory_byte & (~(1<<(pwd_id%8))) ); 

	// There is now one less password
	--NUM_PWD;
	fram_write_byte(OFFSET_NUM_PWD, NUM_PWD);

	// Clear the whole password chunk
	fram_set(PWD_ADDR(pwd_id, 0), 0, SIZE_OF_PWD_BLOCK);
}

uint8_t pwd_list_add_pwd (uint8_t* name, uint8_t* data, uint8_t* usrName)
{
	// First check if there is a chunk free
	uint8_t chunk_free = 0;
	uint8_t pwd_id = 0;
	for(uint8_t i = 0; i < SIZE_MEMORY_MAP && (!chunk_free); ++i)
	{
		// Loop in all the bit of the bytes
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+i);
		for(uint8_t j = 0; j < 8 && (!chunk_free); ++j)
		{
			if(i * 8 + j >= MAXIMUM_NUMBER_OF_PWD)
				break;

			if( (memory_byte & (1<<j))== 0) // the bit is not set, there is a free chunk
			{
				chunk_free = 1;
				pwd_id = i * 8 + j;

				 // Set the bit, this chunk is now used
				// Update the byte that has changed
				fram_write_byte(OFFSET_MEMORY_MAP+i, memory_byte | (1<<j));
			}
		}
	}
	if(!chunk_free)
	{
		return 0; // Not enough memory
	}

	// Initial value if there is no password
	uint8_t prev_util	= MAXIMUM_NUMBER_OF_PWD;
	uint8_t next_util	= MAXIMUM_NUMBER_OF_PWD;
	uint8_t prev_alpha	= MAXIMUM_NUMBER_OF_PWD;
	uint8_t next_alpha	= MAXIMUM_NUMBER_OF_PWD;

	// If this is not the first password
	if(NUM_PWD != 0)
	{
		// The new password became the last of the list
		// The previous element is the last of the list
		// The next element is nothing

		uint8_t last_pwd_util = pwd_list_get_prev_pwd_id_sort_usage(pwd_list_get_first_pwd_id_sort_usage());
		uint8_t last_pwd_alpha = pwd_list_get_prev_pwd_id_sort_alpha(pwd_list_get_first_pwd_id_sort_alpha());

		// Point to the last of the list (before insertion)
		prev_util = last_pwd_util;
		prev_alpha = last_pwd_alpha;

		// Point to an invalid id to point out that this is the end of the list
		next_util = MAXIMUM_NUMBER_OF_PWD;
		next_alpha = MAXIMUM_NUMBER_OF_PWD;

		// Set the last of the list to point to this new pwd
		password_set_next_pwd_util	(prev_util,  pwd_id);
		password_set_next_pwd_alpha	(prev_alpha, pwd_id);
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
	//pwd_list_sort_usage();
	//pwd_list_sort_alpha();
	return 1; // Success
}


static void swap_neighbours(uint8_t lower, uint8_t upper,
							uint8_t(*read_prev)(uint8_t), uint8_t(*read_next)(uint8_t),
							void(*set_prev)(uint8_t, uint8_t), void (*set_next)(uint8_t, uint8_t))
{
	const uint8_t prev_lower	= read_prev	(lower);
	const uint8_t next_upper	= read_next	(upper);

	// First case : the lower id
	set_prev (lower, upper);
	set_next (lower, next_upper);

	// Upper id
	set_prev (upper, prev_lower);
	set_next (upper, lower);

	if(prev_lower != MAXIMUM_NUMBER_OF_PWD)
	{
		set_next(prev_lower, upper);
	}
	if(next_upper != MAXIMUM_NUMBER_OF_PWD)
	{
		set_prev	(next_upper, lower);
	}
}
static void swap_neighbours_util(uint8_t lower, uint8_t upper)
{
	swap_neighbours(lower, upper, password_read_prev_pwd_util, password_read_next_pwd_util, password_set_prev_pwd_util, password_set_next_pwd_util);
}

static void swap_neighbours_alpha(uint8_t lower, uint8_t upper)
{
	swap_neighbours(lower, upper, password_read_prev_pwd_alpha, password_read_next_pwd_alpha, password_set_prev_pwd_alpha, password_set_next_pwd_alpha);
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

	for(uint8_t i = 0; i < NUM_PWD-1; ++i)
	{
		uint16_t current_count = password_read_counter(current);
		uint16_t prev_count = password_read_counter(prev);

		while(prev != MAXIMUM_NUMBER_OF_PWD && current_count < prev_count)
		{
			// If we swap with the first pwd, be sure that we update the first pwd id
			if(prev == first)
			{
				fram_write_byte(OFFSET_FIRST_PWD_UTIL, current);
			}
			swap_neighbours_util(prev, current);
			prev = password_read_prev_pwd_util(current);
			if(prev == MAXIMUM_NUMBER_OF_PWD)
			{
				break;
			}
			prev_count = password_read_counter(prev);
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
	uint8_t sorting_position = current;

	while(current != MAXIMUM_NUMBER_OF_PWD)
	{
		char nameCurrent[32];
		password_read_name(current, (uint8_t*)nameCurrent);

		char namePrev[32];
		password_read_name(prev, (uint8_t*)namePrev);

		while(prev != MAXIMUM_NUMBER_OF_PWD && strcmp(nameCurrent, namePrev) < 0)
		{
			// If we swap with the first pwd, be sure that we update the first pwd id
			if(prev == first)
			{
				fram_write_byte(OFFSET_FIRST_PWD_ALPHA, current);
			}
			swap_neighbours_alpha(prev, current);
			sorting_position = prev;
			prev = password_read_prev_pwd_alpha(current);
			if(prev == MAXIMUM_NUMBER_OF_PWD)
			{
				break;
			}
			password_read_name(prev, (uint8_t*) namePrev);
		}
		current = password_read_next_pwd_alpha(sorting_position);
	}
	// // The condition will be met because the list loop
	// while(current != first)
	// {
	// 	char nameCurrent[32];
	// 	password_read_name(current, (uint8_t*)nameCurrent);

	// 	char namePrev[32];
	// 	password_read_name(prev, (uint8_t*)namePrev);

	// 	while(strcmp(nameCurrent, namePrev) < 0)
	// 	{
	// 		pwd_list_swap(current, prev);
	// 		prev = password_read_prev_pwd_alpha(current);
	// 		password_read_name(prev, (uint8_t*) namePrev);
	// 	}
	// 	current = password_read_next_pwd_alpha(current);
	// }
}
