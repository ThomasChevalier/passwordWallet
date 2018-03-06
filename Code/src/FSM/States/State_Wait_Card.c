#include "State_Wait_Card.h"

#include <string.h>

#include "../../Globals.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Hardware/Rfid.h"
#include "../../Hardware/Oled.h"

#include "../../Security/Security.h"
#include "../../Security/Authentification.h"
#include "../../Security/Password_List.h"
#include "../../Security/Encryption.h"

#include "../../Graphics/String.h"
#include "../../Graphics/Drawing.h"

#include "../../Options/Options.h"

void state_wait_card_begin(void)
{
	// If for some reasons the rfid reader has been shutted down
	rfid_init();
	oled_dim(1); // Save 4 mA
}

uint8_t state_wait_card_transition (uint8_t event)
{
	if(event & EVENT_KEY_ENTERED){
		return STATE_MAIN;
	}

	// In some cases the rfid is disabled after a sleep. Strange ...
	if(rfid_is_power_down()){
		rfid_init();
	}
	
	// There is a new card to read
	if(rfid_PICC_IsNewCardPresent() && rfid_PICC_ReadCardSerial())
	{
		if(user_authenticate_card())
		{
			uint8_t buffer[18];

			// Trying to read master key ...
			if(user_read_key_from_card(buffer, MIFARE_BLOCK_KEY) == RETURN_SUCCESS)
			{
				// .. Success
				memcpy(KEY, buffer, 16);

				if(encryption_check_key()) // If the key of the rfid is the good one.
				{
					events_happen(EVENT_KEY_ENTERED);
					// Then go to the main state and reset activity timer
					ACTIVITY_TIMER = 0;
					return STATE_MAIN;
				}
				else
				{
					draw_clear();
					draw_flash_str_cx(20, str_error_pwd);
					draw_update();
				}
			}
		}
		else
		{
			// If the authentication fail, wait for an other card
		}

		// Necessary to procede to other communications
		rfid_PICC_haltA();
		rfid_pcd_stopCrypto1();
	}
	// Check for the recovery sequence
	else if((event & EVENT_ALL_BUTTONS) == EVENT_ALL_BUTTONS)
	{
		do
		{
			options_display(OPTIONS_LIST_RECOVERY);
		}while(encryption_check_key() != RETURN_SUCCESS);
		events_happen(EVENT_KEY_ENTERED);
		return STATE_MAIN;
	}

	// No rfid tag or wrong rfid tag, try again ...
	// Or no events
	return STATE_WAIT_CARD;
}

void state_wait_card_end(void)
{
	// Save some mA
	rfid_power_down();
	// Normal contrast
	oled_dim(0);

	// Initialize and sort all the data
	CURRENT_PASSWORD_ID = pwd_list_get_first_pwd_id();
	pwd_list_sort_use();
	pwd_list_sort_alpha();
}
