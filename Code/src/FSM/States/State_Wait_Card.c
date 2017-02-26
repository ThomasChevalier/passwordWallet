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


void state_wait_card_begin(void)
{
	// If for some reasons the rfid reader has been shutted down
	rfid_init();
	oled_dim(1); // Save 4 mA
}

uint8_t state_wait_card_transition (uint8_t event)
{
	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	// There is a new card to read
	if(rfid_PICC_IsNewCardPresent() && rfid_PICC_ReadCardSerial())
	{
		if(authenticate_on_card())
		{
			uint8_t buffer[18];
			uint8_t size = 18;

			// Trying to read master key ...
			if(rfid_MIFARE_read(4, buffer, &size) != STATUS_OK && size != 16)
			{
				// .. Failure
				draw_clear();
				str_to_buffer(str_error_read_index);
				draw_text(19, 20, str_buffer, 0);
				draw_update();
			}
			else
			{
				// .. Success
				memcpy(KEY, buffer, 16);
				security_erase_data(buffer, 18);

				if(encryption_check_key()) // If the key of the rfid is the good one.
				{
					// Then go to the main state
					CURRENT_PASSWORD_ID = pwd_list_get_first_pwd_id();
					return STATE_MAIN;
				}
				else
				{
					draw_clear();
					str_to_buffer(str_error_pwd_index);
					draw_text(15, 20, str_buffer, 0);
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
		return STATE_RECOVERY;
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
}
