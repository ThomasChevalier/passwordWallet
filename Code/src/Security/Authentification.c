#include "Authentification.h"

#include <avr/eeprom.h>
#include <util/delay.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"
#include "Encryption.h"
#include "Security.h"
#include "Backup.h"

#include "../Hardware/Rfid.h"
#include "../Hardware/Buttons.h"

#include "../Graphics/Ascii85.h"
#include "../Graphics/ProgressBar.h"
#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"

#include "../FSM/Events.h"

#include "../Program/Program.h"

#include "../System/Sleep.h"

void wait_rfid_tag(void)
{
	// Ask for card
	draw_clear();
	draw_flash_str(3, 30, str_order_approach);
	draw_update();

	while(1)
	{
		if(!rfid_PICC_IsNewCardPresent())
		{
			continue;
		}

		if(rfid_PICC_ReadCardSerial())
			break;
	}
}

uint8_t authenticate_on_card(void)
{
	// sak == 0x08 <=> MIFARE 1K
	if(rfid_uid.sak != 0x08)
	{
		draw_clear();
		draw_flash_str(0, 20, str_error_card);
		draw_update();
	}
	else
	{
		MIFARE_Key key;
		StatusCode status;
		for(uint8_t i = 0; i < 6; ++i)
		{
			key.keyByte[i] = 0xFF;
		}

		// 7 is the trailer block of sector 1
		status = rfid_pcd_authenticate(PICC_CMD_MF_AUTH_KEY_A, MIFARE_AUTH_TRAILER_BLOCK, &key, &rfid_uid);
		if(status != STATUS_OK)
		{
			draw_clear();
			draw_flash_str(0, 20, str_error_auth);
			draw_update();
		}
		else
		{
			return RETURN_SUCCESS;
		}
	}
	return RETURN_ERROR;
}

void user_update_validation (void)
{
	// Change the status to eeprom validation state
	backup_set_status(BACKUP_STATUS_CHANGE_KEY_VALIDATION);

	// Update encryption validation

	draw_clear();
	draw_flash_str(15, 3, str_comm_no_unplug);
	draw_flash_str(10, 40, str_change_key_what);
	progress_begin(EEPROM_RANDSEQ_SIZE + 4);

	encryption_update_validation();

	progress_end();

}

void change_master_key(void)
{
	DISABLE_SLEEP();
	// rfid may be power down
	rfid_init();

	// Waiting for the user to present his card
	wait_rfid_tag();

	if(authenticate_on_card() != RETURN_SUCCESS)
	{
		goto EXIT;
	}

	// Generate new key
	uint8_t newKey[16];
	draw_clear();
	draw_flash_str(10, 40, str_change_key_what);
	progress_begin(16);
	for(uint8_t i = 0; i < 16; ++i)
	{
		newKey[i] = random_request_byte();
		progress_add(1);
	}
	progress_end();

	// Write it to the rfid tag ...
	if(write_key_to_card(newKey, MIFARE_BLOCK_TEMP_KEY) != RETURN_SUCCESS)
	{
		goto EXIT;
	}

	// From here if a reset happen the backup system must be able to 
	// recover information and finish the work

	draw_clear();
	draw_flash_str(15, 3, str_comm_no_unplug);
	draw_flash_str(5, 40, str_change_key_update_pwd);
	progress_begin(NUM_PWD);

	// Update encryption key
	encryption_update_key(newKey, 0);
	// Now KEY contains the value of newKey

	progress_end();

	// Change the status to update card state
	backup_set_status(BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD);

	// Update rfid data
	// Write the new key to the normal key block
	if(write_key_to_card(KEY, MIFARE_BLOCK_KEY) != RETURN_SUCCESS)
	{
		// Crash the device. The user is forced to restart it and
		// the backup system will be able to recover data.
		while(1);
	}
	// Erasing the new key block is not needed

	user_update_validation();

	// We are done !!!
	backup_free();

	// Display new key
	display_master_key();

	// Wait for the user to press a button
	EXIT:
	rfid_power_down();
	program_pause_until_event(EVENT_ALL_BUTTONS);

	ENABLE_SLEEP();
}

uint8_t read_key_from_card(uint8_t* keyOut, uint8_t keyBlock)
{
	uint8_t size = 18;
	if(rfid_MIFARE_read(keyBlock, keyOut, &size) != STATUS_OK && size != 16)
	{
		// .. Failure
		draw_clear();
		draw_flash_str(19, 20, str_error_read);
		draw_update();
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

uint8_t write_key_to_card(uint8_t* keyIn, uint8_t keyBlock)
{
	if(rfid_MIFARE_write(keyBlock, keyIn, 16) != STATUS_OK)
	{
		// .. Failure
		draw_clear();
		draw_flash_str(0, 20, str_error_write);
		draw_update();
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

void display_master_key(void)
{
	draw_clear();
	draw_flash_str(17, 10, str_change_key_here);
	char outputText[20];
	encode_16B(KEY, outputText);
	draw_text(0, 30, outputText, 20);
	security_erase_data(outputText, 20);
	draw_update();
}