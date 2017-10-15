#include "Encryption.h"

#include <string.h>

#include <avr/eeprom.h>

#include "../Globals.h"

#include "Aes.h"
#include "Password.h"
#include "Security.h"
#include "Random.h"
#include "Backup.h"

#include "../Hardware/Fram.h"

#include "../Graphics/ProgressBar.h"

#include "../Memory/eeprom.h"

uint8_t encryption_check_key(void)
{
	// Read the random sequence from eeprom (address [0;15])
	uint8_t randSeq[sizeof(EEPROM_VARS.rand_seq)];
	eeprom_read_block(randSeq, EEPROM_VARS.rand_seq, sizeof(EEPROM_VARS.rand_seq));

	// Encrypt the random sequence with the KEY
	uint8_t zeroIv[16]  =
	{0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	AES128_CBC_encrypt_buffer(randSeq, sizeof(EEPROM_VARS.enc_validation), KEY, zeroIv);

	// And check if it match with the already encrypted data (address [16;31])
	for(uint8_t verifCounter = 0; verifCounter < sizeof(EEPROM_VARS.enc_validation); ++verifCounter)
	{
		if(randSeq[verifCounter] != eeprom_read_byte( &(EEPROM_VARS.enc_validation[verifCounter]) ) )
		{
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}


void encryption_update_validation(void)
{
	// Generate random sequence
	uint8_t randomSequence[sizeof(EEPROM_VARS.rand_seq)];
	for(uint8_t i = 0; i < sizeof(EEPROM_VARS.rand_seq); ++i)
	{
		randomSequence[i] = random_request_byte();
		
		eeprom_update_byte(&(EEPROM_VARS.rand_seq[i]), randomSequence[i]);
		progress_add(1);
	}
	
	// Encrypt that sequence
	uint8_t zeroIv[16]  = {0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	AES128_CBC_encrypt_buffer(randomSequence, sizeof(EEPROM_VARS.rand_seq), KEY, zeroIv);
	progress_add(2);

	// Write it to the eeprom
	for(uint8_t i = 0; i < sizeof(EEPROM_VARS.rand_seq); ++i)
	{
		eeprom_update_byte(&(EEPROM_VARS.enc_validation[i]), randomSequence[i]);
	}
	progress_add(2);
}


void encryption_update_key(uint8_t *new_key, uint8_t start_from)
{
	uint8_t buffer[64];

	uint8_t i = start_from / 8;
	uint8_t j = start_from % 8;
	
	// For all the chunk used ...
	for(; i < SIZE_MEMORY_MAP; ++i)
	{
		const uint8_t memory_byte = fram_read_byte(OFFSET_MEMORY_MAP + i);
		for(; j < 8; ++j)
		{
			const uint8_t pwd_id = i*8+j;

			if(pwd_id<MAXIMUM_NUMBER_OF_PWD)
			{
				if(memory_byte & (1<<j))
				{
					// Pause progress bar because otherwise password_set_* will add progress
					progress_pause();
					
					// Protect from unexpected reset
					backup_save(pwd_id, BACKUP_STATUS_CHANGE_KEY);

					// ... Change the key
					password_read_data(pwd_id, buffer, KEY);
					password_set_data (pwd_id, buffer, new_key);

					password_read_usr_name(pwd_id, buffer, KEY);
					password_set_usr_name (pwd_id, buffer, new_key);

					progress_continue();
					progress_add(1);

					// Mark the chunk as complete, but the process is not finished
					backup_set_status(BACKUP_STATUS_CHANGE_KEY_CHUNK_OK);
				}
			}
		}
	}
	memcpy(KEY, new_key, 16);

	security_erase_data(new_key, 16);
	security_erase_data(buffer, 64);
}
