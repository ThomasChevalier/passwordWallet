#include "Backup.h"

#include <string.h> // for memcpy

#include "../Globals.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Rfid.h"

#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"

#include "../Security/Encryption.h"
#include "../Security/Authentification.h"

#include "../Program/Program.h"
#include "../FSM/Events.h"

#include "../System/Sleep.h"

// CORE

void backup_save (uint8_t pwd_id, uint8_t status)
{
	// Copy the chunk to the backup
	uint8_t i = 0;
	for(; i < SIZE_OF_PWD_BLOCK; ++i)
	{
		const uint8_t byte = fram_read_byte(PWD_ADDR(pwd_id, i));
		fram_write_byte(OFFSET_BACKUP_DATA + i, byte);
	}
	fram_write_byte(OFFSET_BACKUP_ID, pwd_id);
	// At the very last update the back status byte.
	fram_write_byte(OFFSET_BACKUP_STATUS, status);
}

void backup_restore (void)
{
	// Don't update with a invalid backup
	if(fram_read_byte(OFFSET_BACKUP_STATUS) == BACKUP_STATUS_OK)
	{
		return;
	}
	const uint8_t pwd_id = fram_read_byte(OFFSET_BACKUP_ID);

	uint8_t i = 0;
	for(; i < SIZE_OF_PWD_BLOCK; ++i)
	{
		const uint8_t byte = fram_read_byte(OFFSET_BACKUP_DATA + i);
		fram_write_byte(PWD_ADDR(pwd_id, i), byte);
	}

	// Free the backup section
	backup_free();
}

void backup_free (void)
{
	// First write the block as OK, because if a reset occurs
	// when setting data to 0, backup_recover() will restore
	// a invalid chunk.
	fram_write_byte(OFFSET_BACKUP_STATUS, BACKUP_STATUS_OK);
	fram_set(OFFSET_BACKUP_DATA, 0, SIZE_BACKUP_DATA);
	fram_write_byte(OFFSET_BACKUP_ID, 0);
}

void backup_set_status(uint8_t status)
{
	fram_write_byte(OFFSET_BACKUP_STATUS, status);
}

// LOGIC

void backup_recover(void)
{
	uint8_t backupStatus = fram_read_byte(OFFSET_BACKUP_STATUS);
	if(backupStatus == BACKUP_STATUS_OK)
	{
		return;
	}
	else
	{
		uint8_t newKey[18];
		uint8_t lastKey[18];
		uint8_t textY = 0;

		// This is easy to fix, just restore the last data and we are done.
		if(backupStatus == BACKUP_STATUS_UPDATE)
		{
			backup_restore();
			return;
		}
		// This is less easy because we have to continue the operation ...
		if(backupStatus == BACKUP_STATUS_CHANGE_KEY ||
		   backupStatus == BACKUP_STATUS_CHANGE_KEY_CHUNK_OK ||
		   backupStatus == BACKUP_STATUS_CHANGE_KEY_VALIDATION ||
		   backupStatus == BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD)
		{
			// We need to read from the card the "last key" and the "new key"
			// Some passwords may be already updated with "new key" but we need
			// To update those that are still encrypted with "new key"
			// And along those password there is obviously the password that we have to restore.


			// This operation take sometimes so disable the sleep mode.
			DISABLE_SLEEP();

			// Rfid is power down, so power it up
			rfid_init();

			// Waiting for the user to present his card
			wait_rfid_tag();

			if(!authenticate_on_card())
			{
				// If we cannot authenticate on the card abort the operation.
				// The user can retry to restore his data by rebooting the device.
				goto EXIT;
			}

			// Display what we are doing.
			draw_clear();
			draw_flash_str(0, textY, str_backup_what);
			textY += 10;
			draw_update();

			// Read the keys

			// Trying to read the "new key"
			if(read_key_from_card(newKey, MIFARE_BLOCK_TEMP_KEY) != RETURN_SUCCESS)
			{
				goto EXIT;
			}
			
			// Trying to read the "last key"
			if(read_key_from_card(lastKey, MIFARE_BLOCK_KEY) != RETURN_SUCCESS)
			{
				goto EXIT;
			}

			// .. Success
			// We have know the "new key", and the "last key"
			// We can decrypt with "last key" the passwords not updated with "new key"

			// Copy "last key" to the master key
			memcpy(KEY, lastKey, 16);			
		}

		if(backupStatus == BACKUP_STATUS_CHANGE_KEY || backupStatus == BACKUP_STATUS_CHANGE_KEY_CHUNK_OK)
		{
			// Read the id of the first password not updated
			// It is the one stored in the backup section
			uint8_t pwd_id = fram_read_byte(OFFSET_BACKUP_ID);

			// Restore the password, if the update has not been completed on it
			if(backupStatus == BACKUP_STATUS_CHANGE_KEY){
				backup_restore();
			}

			// If the update was complete for this password, start to update the next
			else if(backupStatus == BACKUP_STATUS_CHANGE_KEY_CHUNK_OK)
			{
				++pwd_id;
			}

			// Inform user
			draw_flash_str(0, textY, str_change_key_update_pwd);
			textY += 10;
			draw_update();

			// Let the function do its job.
			// If there is another problem during the update this function will be called at next startup
			encryption_update_key(newKey, pwd_id);

			// Now KEY contains newKey. We must set up the encryption validation.
			backup_set_status(BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD);
			backupStatus = BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD;
		}
		else
		{
			// If encryption_update_key() has not been called
			memcpy(KEY, newKey, 16);
		}

		if(backupStatus == BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD)
		{
			draw_flash_str(0, textY, str_change_key_update_card);
			textY += 10;
			draw_update();
			if(write_key_to_card(KEY, MIFARE_BLOCK_KEY) != RETURN_SUCCESS)
			{
				goto EXIT;
			}

			backup_set_status(BACKUP_STATUS_CHANGE_KEY_VALIDATION);
			backupStatus = BACKUP_STATUS_CHANGE_KEY_VALIDATION;
		}		

		if(backupStatus == BACKUP_STATUS_CHANGE_KEY_VALIDATION)
		{
			draw_flash_str(0, textY, str_change_key_update_validation);
			textY += 10;
			draw_update();

			encryption_update_validation();

			backup_free();
		}

		EXIT:
		rfid_power_down();
		program_pause_until_event(EVENT_ALL_BUTTONS);
		ENABLE_SLEEP();

	} // There is something to restore
}