#include "Authentification.h"

#include <string.h>

#include <avr/eeprom.h>
#include <util/delay.h>

#include "../Globals.h"

#include "Aes.h"
#include "Random.h"
#include "Encryption.h"
#include "Security.h"

#include "../Hardware/Rfid.h"
#include "../Hardware/Buttons.h"
#include "../Hardware/Oled.h"

#include "../Graphics/Ascii85.h"
#include "../Graphics/ProgressBar.h"
#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"

#include "../FSM/Events.h"

#include "../Program/Program.h"


static void waitRfidTag(void)
{
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

// Return 1 if success, 0 otherwise
uint8_t authenticate_on_card(void)
{
	// sak == 0x08 <=> MIFARE 1K
	if(rfid_uid.sak != 0x08)
	{
		draw_clear();
		str_to_buffer(str_error_card_index);
		draw_text(0, 20, str_buffer, 0);
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

		status = rfid_pcd_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, &key, &rfid_uid);
		if(status != STATUS_OK)
		{
			draw_clear();
			str_to_buffer(str_error_auth_index);
			draw_text(0, 20, str_buffer, 0);
			draw_update();
		}
		else
		{
			return 1; // SUCCESS !
		}
	}
	return 0;
}


void change_master_key(void)
{
	// rfid may be power down
	rfid_init();

	// Ask for card
	draw_clear();
	str_to_buffer(str_misc_approachCard_index);
	draw_text(8, 30, str_buffer, 0);
	draw_update();

	// Waiting for the user to present his card
	waitRfidTag();

	// Generate new key
	uint8_t newKey[16];
	draw_clear();
	str_to_buffer(str_misc_generateKey_index);
	draw_text(10, 40, str_buffer, 0);
	progress_begin(16);
	for(uint8_t i = 0; i < 16; ++i)
	{
		newKey[i] = random_request_byte();
		progress_add(1);
	}
	progress_end();

	// Write it to the rfid tag ...
	if(authenticate_on_card())
	{
		// Trying to write on tag
		if(rfid_MIFARE_write(4, newKey, 16) != STATUS_OK)
		{
			// .. Failure
			draw_clear();
			str_to_buffer(str_error_read_index);
			draw_text(0, 20, str_buffer, 0);
			draw_update();
			goto EXIT;
		}
	}
	else
	{
		// If we cannot authenticate, abort operation
		goto EXIT;
	}

	draw_clear();
	str_to_buffer(str_misc_updatePwd_index);
	draw_text(5, 40, str_buffer, 0);
	progress_begin(NUM_PWD);

	// Update encryption key
	encryption_update_key(newKey);

	progress_end();

	// Update encryption validation

	// Generate random sequence
	draw_clear();
	progress_begin(20);
	str_to_buffer(str_misc_updateEncryptVerif_index);
	draw_text(10, 40, str_buffer, 0);
	uint8_t randomSequence[16];
	uint8_t* eeprom_addr = 0;
	for(uint8_t i = 0; i < 16; ++i)
	{
		randomSequence[i] = random_request_byte();
		
		eeprom_update_byte(eeprom_addr, randomSequence[i]);
		++eeprom_addr;
		progress_add(1);
	}

	// Encrypt that sequence
	uint8_t encryptionValidation[16];
	const uint8_t zeroIv[16]  = {0x00, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	AES128_CBC_encrypt_buffer(encryptionValidation, randomSequence, 16, KEY, zeroIv);
	progress_add(2);

	// Write it to the eeprom
	eeprom_addr = 0;
	for(uint8_t i = 0; i < 16; ++i)
	{
		
		eeprom_update_byte(eeprom_addr+16, encryptionValidation[i]);
		++eeprom_addr;
	}
	progress_add(2);
	progress_end();


	// Display new key
	draw_clear();
	str_to_buffer(str_misc_hereIsKey_index);
	draw_text(17, 10, str_buffer, 0);
	char outputText[20];
	encode_16B(KEY, outputText);
	draw_text(0, 30, outputText, 20);
	security_erase_data(outputText, 20);
	draw_update();

	// Wait for the user to press a button
	EXIT:
	rfid_power_down();
	program_pause_until_event(EVENT_ALL_BUTTONS);
}
