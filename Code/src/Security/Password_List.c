#include "Password_List.h"

#include <string.h> // For strcmp()

#include "../Globals.h"

#include "Password.h"

#include "../Graphics/ProgressBar.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Led.h"

void pwd_write_id(f_addr_t framAddr, p_addr id)
{
#if p_addr_sz == 1
	fram_write_byte(framAddr, id);
#else
	fram_write_bytes(framAddr, (uint8_t*)(&id), p_addr_sz);
#endif
}

#if p_addr_sz > 1
p_addr pwd_read_id(f_addr_t framAddr)
{
	p_addr id;
	fram_read_bytes(framAddr, (uint8_t*)(&id), p_addr_sz);
	return id;
}
#endif

uint8_t pwd_list_get_sorting_method(void)
{
	return (OPTIONS_FLAG & ((1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L)|(1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H))) >> OPTIONS_FLAG_OFFSET_SORTING_METHOD_L;
}

// Return the address of the first chunk used (from memory map info), beginning to index (included)
// Loop if nothing found
// Return index if there is no entry
static p_addr get_first_entry_from(const p_addr index)
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

p_addr pwd_list_get_first_pwd_id (void)
{
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_first_pwd_id_sort_none();
	}
	else if(sortingMethod == PWD_SORTING_USE)
	{
		return pwd_list_get_first_pwd_id_sort_use();
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
	{
		return pwd_list_get_first_pwd_id_sort_alpha();
	}
	// Impossible
	return 0;
}

p_addr pwd_list_get_first_pwd_id_sort_none (void)
{
	return get_first_entry_from(0);
}

p_addr pwd_list_get_first_pwd_id_sort_use (void)
{
	return pwd_read_id(OFFSET_FIRST_PWD_USE);
}

p_addr pwd_list_get_first_pwd_id_sort_alpha (void)
{
	return pwd_read_id(OFFSET_FIRST_PWD_ALPHA);
}


p_addr pwd_list_get_prev_pwd_id (p_addr pwd_id)
{
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_prev_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_USE)
	{
		return pwd_list_get_prev_pwd_id_sort_use(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
	{
		return pwd_list_get_prev_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

// This function return the first id less than pwd_id, or if there is no such id
// it return the first id from the last password to the first password
// If there is only one password then this function returns pwd_id
// If there is no password then this function returns MAXIMUM_NUMBER_OF_PWD
p_addr pwd_list_get_prev_pwd_id_sort_none (p_addr pwd_id)
{
	const p_addr search_from = MAXIMUM_NUMBER_OF_PWD-1;
	p_addr found = MAXIMUM_NUMBER_OF_PWD; // Intialize it with an impossible value

	for(p_addr i = (search_from) / 8; i != (p_addr)-1; --i)
	{
		// Read the memory map stored in fram
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP+i);

		uint8_t j = (i == search_from / 8) ? search_from % 8 : 7;

		for(; j != (uint8_t)-1; --j)
		{
			// If there is a chunk used
			if(memory_byte & (1<<j))
			{
				const p_addr id = i*8+j;
				// Return the first id less than pwd_id
				if(id < pwd_id)
				{
					return id;
				}

				// If nothing has been found yet
				if(found == MAXIMUM_NUMBER_OF_PWD)
				{
					found = id;
				}
			}
		}
	}
	// Nothing has been found below pwd_id, return the first id we match
	return found;
}

p_addr pwd_list_get_prev_pwd_id_sort_use (p_addr pwd_id)
{
	p_addr id = password_read_prev_pwd_use(pwd_id);
	if(id == MAXIMUM_NUMBER_OF_PWD)
	{
		while(pwd_id != MAXIMUM_NUMBER_OF_PWD)
		{
			id = pwd_id;
			pwd_id = password_read_next_pwd_use(pwd_id);
		}
	}
	return id;
}

p_addr pwd_list_get_prev_pwd_id_sort_alpha (p_addr pwd_id)
{
	p_addr id = password_read_prev_pwd_alpha(pwd_id);
	if(id == MAXIMUM_NUMBER_OF_PWD)
	{
		while(pwd_id != MAXIMUM_NUMBER_OF_PWD)
		{
			id = pwd_id;
			pwd_id = password_read_next_pwd_alpha(pwd_id);
		}
	}
	return id;
}


p_addr pwd_list_get_next_pwd_id (p_addr pwd_id)
{
	uint8_t sortingMethod = pwd_list_get_sorting_method();
	if(sortingMethod == PWD_SORTING_NONE)
	{
		return pwd_list_get_next_pwd_id_sort_none(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_USE)
	{
		return pwd_list_get_next_pwd_id_sort_use(pwd_id);
	}
	else if(sortingMethod == PWD_SORTING_ALPHA)
	{
		return pwd_list_get_next_pwd_id_sort_alpha(pwd_id);
	}
	// Impossible
	return pwd_id;
}

p_addr pwd_list_get_next_pwd_id_sort_none (p_addr pwd_id)
{
	// Since get_first_entry_from(i) return i if the chunk i is used, we must add 1 to pwd_id
	return get_first_entry_from(pwd_id+1);
}

p_addr pwd_list_get_next_pwd_id_sort_use (p_addr pwd_id)
{
	p_addr id = password_read_next_pwd_use(pwd_id);
	// If pwd_id is the last password of the list, loop and return the first
	if(id == MAXIMUM_NUMBER_OF_PWD)
	{
		id = pwd_list_get_first_pwd_id_sort_use();
	}
	return id;
}

p_addr pwd_list_get_next_pwd_id_sort_alpha (p_addr pwd_id)
{
	p_addr id = password_read_next_pwd_alpha(pwd_id);
	// If pwd_id is the last password of the list, loop and return the first
	if(id == MAXIMUM_NUMBER_OF_PWD)
	{
		id = pwd_list_get_first_pwd_id_sort_alpha();
	}
	return id;
}

void pwd_list_delete_pwd (p_addr pwd_id)
{
	p_addr prev_use = password_read_prev_pwd_use(pwd_id);
	p_addr next_use = password_read_next_pwd_use(pwd_id);

	p_addr prev_alpha = password_read_prev_pwd_alpha(pwd_id);
	p_addr next_alpha = password_read_next_pwd_alpha(pwd_id);

	// If the next password is valid ...
	if(next_use != MAXIMUM_NUMBER_OF_PWD)
	{
		password_set_prev_pwd_use(next_use, prev_use);
	}
	// If this is the first password
	if(prev_use == MAXIMUM_NUMBER_OF_PWD)
	{
		// If it is not the last too
		if(next_use != MAXIMUM_NUMBER_OF_PWD)
		{
			pwd_write_id(OFFSET_FIRST_PWD_USE, next_use);
		}
		// It is the last pwd, set offset_first_ to 0
		else
		{
			pwd_write_id(OFFSET_FIRST_PWD_USE, 0);
		}
	}
	else
	{
		password_set_next_pwd_use(prev_use, next_use);
	}

	// If the next password is valid ...
	if(next_alpha != MAXIMUM_NUMBER_OF_PWD)
	{
		password_set_prev_pwd_alpha(next_alpha, prev_alpha);
	}
	if(prev_alpha == MAXIMUM_NUMBER_OF_PWD)
	{
		// If it is not the last too
		if(next_alpha != MAXIMUM_NUMBER_OF_PWD)
		{
			pwd_write_id(OFFSET_FIRST_PWD_ALPHA, next_alpha);
		}
		// It is the last pwd, set offset_first_ to 0
		else
		{
			pwd_write_id(OFFSET_FIRST_PWD_ALPHA, 0);
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
	pwd_write_id(OFFSET_NUM_PWD, NUM_PWD);

	// Clear the whole password chunk
	fram_set(PWD_ADDR(pwd_id, 0), 0, SIZE_OF_PWD_BLOCK);
}

uint8_t pwd_list_add_pwd (uint8_t* name, uint8_t* data, uint8_t* usrName)
{
	// First check if there is a chunk free
	uint8_t chunk_free = 0;
	p_addr pwd_id = 0;
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
		return RETURN_ERROR; // Not enough memory
	}

	// Initial value if there is no password
	p_addr prev_use   = MAXIMUM_NUMBER_OF_PWD;
	p_addr next_use   = MAXIMUM_NUMBER_OF_PWD;
	p_addr prev_alpha = MAXIMUM_NUMBER_OF_PWD;
	p_addr next_alpha = MAXIMUM_NUMBER_OF_PWD;

	// If this is not the first password
	if(NUM_PWD != 0)
	{
		// The new password became the last of the list
		// The previous element is the last of the list
		// The next element is nothing

		// Take the last element of the list
		p_addr last_pwd_use = pwd_list_get_prev_pwd_id_sort_use(pwd_list_get_first_pwd_id_sort_use());
		p_addr last_pwd_alpha = pwd_list_get_prev_pwd_id_sort_alpha(pwd_list_get_first_pwd_id_sort_alpha());

		// Point to the last of the list (before insertion)
		prev_use = last_pwd_use;
		prev_alpha = last_pwd_alpha;

		// Point to an invalid id to point out that this is the end of the list
		next_use = MAXIMUM_NUMBER_OF_PWD;
		next_alpha = MAXIMUM_NUMBER_OF_PWD;

		// Set the last of the list to point to this new pwd
		password_set_next_pwd_use	(prev_use,  pwd_id);
		password_set_next_pwd_alpha	(prev_alpha, pwd_id);
	}

	// Set up list
	password_set_prev_pwd_use	(pwd_id, prev_use);
	password_set_next_pwd_use	(pwd_id, next_use);
	password_set_prev_pwd_alpha	(pwd_id, prev_alpha);
	password_set_next_pwd_alpha	(pwd_id, next_alpha);

	// Set counter to 0
	password_set_counter(pwd_id, 0);

	// Set name, data and user name
	password_set_name		(pwd_id, name        );
	password_set_data		(pwd_id, data,    KEY);
	password_set_usr_name	(pwd_id, usrName, KEY);

	// The password has been added, increment counter
	++NUM_PWD;
	pwd_write_id(OFFSET_NUM_PWD, NUM_PWD);

	// We have inserted the password at the end of each list (alpha and use) but it is not sorted.
	// We must sort them.
	pwd_list_sort_use();
	pwd_list_sort_alpha();
	return RETURN_SUCCESS; // Success
}


static void swap_neighbours(p_addr lower, p_addr upper,
							p_addr(*read_prev)(p_addr), p_addr(*read_next)(p_addr),
							void(*set_prev)(p_addr, p_addr), void (*set_next)(p_addr, p_addr))
{
	const p_addr prev_lower	= read_prev	(lower);
	const p_addr next_upper	= read_next	(upper);

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
static void swap_neighbours_use(p_addr lower, p_addr upper)
{
	swap_neighbours(lower, upper, password_read_prev_pwd_use, password_read_next_pwd_use, password_set_prev_pwd_use, password_set_next_pwd_use);
}

static void swap_neighbours_alpha(p_addr lower, p_addr upper)
{
	swap_neighbours(lower, upper, password_read_prev_pwd_alpha, password_read_next_pwd_alpha, password_set_prev_pwd_alpha, password_set_next_pwd_alpha);
}

void pwd_list_sort_use (void)
{
	if(NUM_PWD < 2)
	{
		return;
	}

	p_addr first = pwd_list_get_first_pwd_id_sort_use();
	p_addr current = password_read_next_pwd_use(first);
	p_addr prev = 0;
	p_addr sorting_position = 0;
	
	while(current != MAXIMUM_NUMBER_OF_PWD)
	{
		sorting_position = current;
		prev = password_read_prev_pwd_use(current);

		uint16_t current_count = password_read_counter(current);
		uint16_t prev_count = password_read_counter(prev);

		while(prev != MAXIMUM_NUMBER_OF_PWD && current_count > prev_count)
		{
			// If we swap with the first pwd, be sure that we update the first pwd id
			if(prev == first)
			{
				pwd_write_id(OFFSET_FIRST_PWD_USE, current);
				first = current;
			}
			swap_neighbours_use(prev, current);
			sorting_position = prev;
			prev = password_read_prev_pwd_use(current);
			if(prev == MAXIMUM_NUMBER_OF_PWD)
			{
				break;
			}
			prev_count = password_read_counter(prev);
		}
		current = password_read_next_pwd_use(sorting_position);
	}
}

/**
 * @brief Converts each character of str to its uppercase equivalent if the character is a lowercase letter and has an uppercase equivalent.
 * If no such conversion is possible, the value stay to the character unchanged.
 * 
 * @param str The string to transform.
 */	
static void upper(char* str)
{
	while(*str != '\0')
	{
		if( *str >= 'a' && *str <= 'z')
			*str = *str - ('a' - 'A');
		++str;
	}
}

void pwd_list_sort_alpha (void)
{
	if(NUM_PWD < 2)
	{
		return;
	}

	p_addr first = pwd_list_get_first_pwd_id_sort_alpha();
	p_addr current = password_read_next_pwd_alpha(first);
	p_addr prev = 0;
	p_addr sorting_position = 0;

	while(current != MAXIMUM_NUMBER_OF_PWD)
	{
		sorting_position = current;
		prev = password_read_prev_pwd_alpha(current);

		char nameCurrent[32];
		password_read_name(current, (uint8_t*)nameCurrent);
		char namePrev[32];
		password_read_name(prev, (uint8_t*)namePrev);
		// String should be in upper case for proper sorting
		upper(nameCurrent);
		upper(namePrev);

		while(prev != MAXIMUM_NUMBER_OF_PWD && strcmp(nameCurrent, namePrev) < 0)
		{
			// If we swap with the first pwd, be sure that we update the first pwd id
			if(prev == first)
			{
				pwd_write_id(OFFSET_FIRST_PWD_ALPHA, current);
				first = current;
			}
			swap_neighbours_alpha(prev, current);
			sorting_position = prev;
			prev = password_read_prev_pwd_alpha(current);
			if(prev == MAXIMUM_NUMBER_OF_PWD)
			{
				break;
			}
			password_read_name(prev, (uint8_t*) namePrev);
			upper(namePrev);
		}
		current = password_read_next_pwd_alpha(sorting_position);
	}
}
