#include "Encryption.h"

#include <string.h>

#include <avr/eeprom.h>

#include "../Globals.h"

#include "Aes.h"
#include "Password.h"

#include "../Hardware/Fram.h"

#include "../Graphics/ProgressBar.h"

void encryption_copy_key_from_eeprom(void)
{
	eeprom_read_block(KEY, (void*)32, 16);
}

void encryption_disable(void)
{
	// Store key in eeprom
	eeprom_update_block(KEY, (void*)32, 16);

	// Update flags
	OPTIONS_FLAG |= (1<<OPTIONS_FLAG_OFFSET_NO_ENCRYPTION);
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

void encryption_enable(void)
{
	// Remove key from eeprom
	uint8_t* eeprom_addr = (uint8_t *)32;
	for(uint8_t i = 0; i < 16; ++i)
	{
        eeprom_update_byte(eeprom_addr, 0);
        ++eeprom_addr;
	}
	
	// Update flags
	OPTIONS_FLAG &= ~(1<<OPTIONS_FLAG_OFFSET_NO_ENCRYPTION);
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

uint8_t encryption_check_key(void)
{
	// Read the random sequence from eeprom (address [0;15])
	uint8_t randSeq[16];
	eeprom_read_block(randSeq, (void*)0, 16);

	// Encrypt the random sequence with the KEY
	uint8_t output[16];
	const uint8_t zeroIv[16]  =
	{0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	AES128_CBC_encrypt_buffer(output, randSeq, 16, KEY, zeroIv);

	// And check if it match with the already encrypted data (address [16;31])
	uint8_t* eeprom_addr = (uint8_t*)0;
	for(uint8_t verifCounter = 0; verifCounter < 16; ++verifCounter)
	{
		if(output[verifCounter] != eeprom_read_byte(eeprom_addr+16))
		{
			return 0;
		}
		++eeprom_addr;
	}
	return 1;
}


void encryption_update_key(uint8_t *new_key)
{
	uint8_t buffer[64];
	// For all the chunk used ...
	for(uint8_t i = 0; i < SIZE_MEMORY_MAP; ++i)
	{
		const uint8_t memory_byte = MEMORY_MAP[i];
		for(uint8_t j = 0; j < 8; ++j)
		{
			if(i*8+j<MAXIMUM_NUMBER_OF_PWD)
			{
				if(memory_byte & (1<<j))
				{
					// Pause progress bar because otherwise password_set_* will add progress
					progress_pause();
					
					// ... Change the key
					CURRENT_PASSWORD_ID = i*8+j;

					password_read_data(i*8+j, buffer, KEY);
					password_set_data(i*8+j,buffer, strlen((char*)(buffer)), new_key);

					password_read_usr_name(i*8+j, buffer, KEY);
					password_set_usr_name(i*8+j, buffer, strlen((char*)(buffer)), new_key);

					progress_continue();
					progress_add(1);
				}
			}
		}
	}
	memcpy(KEY, new_key, 16);
}
