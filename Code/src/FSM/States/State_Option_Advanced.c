#include "State_Option_Advanced.h"

#include "../../Globals.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "State_Recovery.h"

#include "../../Graphics/Drawing.h"
#include "../../Graphics/String.h"
#include "../../Graphics/Ascii85.h"

#include "../../Hardware/Buttons.h"
#include "../../Hardware/Fram.h"
#include "../../Hardware/Serial.h"

#include "../../Security/Security.h"
#include "../../Security/Authentification.h"
#include "../../Security/Encryption.h"

#include "../Hardware/SelfTest.h"

static uint8_t currentChoice = 0;

static void do_show_key(void)
{
	 // Display key
    draw_clear();
    str_to_buffer(str_misc_hereIsKey_index);
    draw_text(17, 10, str_buffer, 0);
    char outputText[20];
    encode_16B(KEY, outputText);
    draw_text(0, 30, outputText, 20);

    security_erase_data(outputText, 20);

    draw_update();
}

void state_option_advanced_begin (void)
{
	currentChoice = 0;
	draw_list(	currentChoice + OPTION_ADVANCED_MENU_MIN_STR_INDEX,
				OPTION_ADVANCED_MENU_MIN_STR_INDEX,
				OPTION_ADVANCED_MENU_MIN_STR_INDEX + OPTION_ADVANCED_MENU_MAX_CHOICE-1);
}

uint8_t state_option_advanced_transition (uint8_t event)
{
	// Let the user note his key when do_show_key is called
	static uint8_t exit_at_next_event = 0;

	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	
	if(exit_at_next_event && (event&EVENT_ALL_BUTTONS))
	{
		exit_at_next_event = 0;
		return STATE_MAIN;
	}

	if(event & EVENT_BUTTON_1)
	{
		if(currentChoice == 0)
		{
			currentChoice = OPTION_ADVANCED_MENU_MAX_CHOICE - 1;
		}
		else
		{
			--currentChoice;
		}
	}
	else if(event & EVENT_BUTTON_2)
	{
		if(currentChoice == 0)
		{
			change_master_key();
		}
		else if(currentChoice == 1)
		{
			if((OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_NO_ENCRYPTION)) == 0)
			{
				encryption_disable();
			}
		}
		else if(currentChoice == 2)
		{
			// If device is not encrypted
			if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_NO_ENCRYPTION))
			{
				change_master_key();
				encryption_enable();
			}
		}
		else if(currentChoice == 3)
		{
			do_show_key();
			exit_at_next_event = 1;
			return STATE_OPTION_ADVANCED;
		}
		else if(currentChoice == 4)
		{
			self_test_execute();
		}
		else if(currentChoice == 5)
		{
			state_recovery_do_full_reset();
		}
		return STATE_MAIN;
	}
	else if(event & EVENT_BUTTON_3)
	{
		++currentChoice;
		if(currentChoice == OPTION_ADVANCED_MENU_MAX_CHOICE)
		{
			currentChoice = 0;
		}
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_OPTION;
	}

	if(event & EVENT_ALL_BUTTONS)
	{
		draw_list(	currentChoice + OPTION_ADVANCED_MENU_MIN_STR_INDEX,
				OPTION_ADVANCED_MENU_MIN_STR_INDEX,
				OPTION_ADVANCED_MENU_MIN_STR_INDEX + OPTION_ADVANCED_MENU_MAX_CHOICE-1);
	}

	return STATE_OPTION_ADVANCED;
}

void state_option_advanced_end (void)
{

}
