#include "Encryption.h"

#include <string.h>

#include <avr/eeprom.h>

#include "../Globals.h"

#include "Aes.h"
#include "Password.h"
#include "Security.h"

#include "../Hardware/Fram.h"

#include "../Graphics/ProgressBar.h"

uint8_t encryption_check_key(void)
{
	// Read the random sequence from eeprom (address [0;15])
	uint8_t randSeq[EEPROM_RANDSEQ_SIZE];
	eeprom_read_block(randSeq, (void*)EEPROM_OFFSET_RANDSEQ, EEPROM_RANDSEQ_SIZE);

	// Encrypt the random sequence with the KEY
	uint8_t zeroIv[16]  =
	{0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	AES128_CBC_encrypt_buffer(randSeq, EEPROM_VALIDATION_SIZE, KEY, zeroIv);

	// And check if it match with the already encrypted data (address [16;31])
	uint8_t* eeprom_addr = (uint8_t*)EEPROM_VALIDATION_SIZE;
	for(uint8_t verifCounter = 0; verifCounter < EEPROM_VALIDATION_SIZE; ++verifCounter)
	{
		if(randSeq[verifCounter] != eeprom_read_byte(eeprom_addr))
		{
			return RETURN_ERROR;
		}
		++eeprom_addr;
	}
	return RETURN_SUCCESS;
}


void encryption_update_key(uint8_t *new_key)
{
	uint8_t buffer[64];
	// For all the chunk used ...
	for(uint8_t i = 0; i < SIZE_MEMORY_MAP; ++i)
	{
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP + i);
		for(uint8_t j = 0; j < 8; ++j)
		{
			if(i*8+j<MAXIMUM_NUMBER_OF_PWD)
			{
				if(memory_byte & (1<<j))
				{
					// Pause progress bar because otherwise password_set_* will add progress
					progress_pause();
					
					// ... Change the key
					password_read_data(i*8+j, buffer, KEY);
					password_set_data (i*8+j, buffer, new_key);

					password_read_usr_name(i*8+j, buffer, KEY);
					password_set_usr_name (i*8+j, buffer, new_key);

					progress_continue();
					progress_add(1);
				}
			}
		}
	}
	memcpy(KEY, new_key, 16);
	security_erase_data(new_key, 16);
	security_erase_data(buffer, 64);
}
