#include "State_Recovery.h"

#include <string.h>

#include <util/delay.h>

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/String.h"
#include "../../Graphics/ProgressBar.h"
#include "../../Graphics/Ascii85.h"
#include "../../Graphics/Drawing.h"

#include "../../Hardware/Fram.h"
#include "../../Hardware/Buttons.h"

#include "../../Security/Encryption.h"
#include "../../Security/Authentification.h"

#include "../../Program/Program.h"

void state_recovery_do_full_reset(void)
{
	// Save orientation
	uint8_t orientation = (OPTIONS_FLAG&OPTIONS_FLAG_OFFSET_ORIENTATION);

	// Erase all fram memory
	draw_clear();
	str_to_buffer(str_recovery_eraseMem_index);
	draw_text(20, 40, str_buffer, 0);
	draw_update();

	progress_begin(FRAM_BYTE_SIZE/128);

	// Do .. While loop is 6 bytes smaller than for loop in this case
	uint8_t i = FRAM_BYTE_SIZE/128;
	do
	{
		--i;
		fram_set(i*128, 0, 128);
		progress_add(1);
	}while(i);
	progress_end();

	// Restore orientation
	update_opt_flags(orientation << OPTIONS_FLAG_OFFSET_ORIENTATION);

	// Clear variables
	CURRENT_PASSWORD_ID = GLOBALS_EVENTS = NUM_PWD = OPTIONS_FLAG = 0;

	// Get a master key
	change_master_key();

	// Device is now initialized !
	update_opt_flags((1<<OPTIONS_FLAG_OFFSET_INITIALIZED));
}

static uint8_t do_recover_key(void)
{
	char usrKey[21] = {0};
	type_string(usrKey, 20);
	decode_16B(usrKey, KEY);
	if(encryption_check_key())
	{
		return 1;
	}
	return 0;
}

static uint8_t recovery_choice = 0;

void state_recovery_begin (void)
{
	recovery_choice  = 0;

	str_to_buffer(str_recovery_title_index);
	draw_clear();
	draw_text(10, 10, str_buffer, 0);
	str_to_buffer(str_recovery_choice1_index);
	draw_text(10, 30, str_buffer, 0);
	str_to_buffer(str_recovery_choice2_index);
	draw_text(10, 50, str_buffer, 0);
	draw_update();

	// Wait for the user to release all the keys
	while(buttons_pressed())
	{
		program_update(); // Don't freeze the program
	};
	events_get(); // Clear event variable
}

uint8_t state_recovery_transition (uint8_t event)
{
	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	// Select recovery option
	if(recovery_choice == 0)
	{
		if(event & EVENT_BUTTON_1)
		{
			recovery_choice = 1;
		}
		else if(event & EVENT_BUTTON_3)
		{
			recovery_choice = 2;
		}
	}
	else if(recovery_choice == 1)
	{
		state_recovery_do_full_reset();
		return STATE_MAIN;
	}
	else if(recovery_choice == 2)
	{
		if(do_recover_key())
		{
			return STATE_MAIN;
		}
		// The user was unable to recover is key, recovery mode start again
		else
		{
			state_recovery_begin();
			return STATE_RECOVERY;
		}
	}

	return STATE_RECOVERY;
}

void state_recovery_end (void)
{

}
