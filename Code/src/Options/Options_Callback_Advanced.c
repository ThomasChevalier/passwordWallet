#include "Options_Callback_Advanced.h"

#include "../Globals.h"

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"
#include "../Graphics/Ascii85.h"
#include "../Graphics/ProgressBar.h"

#include "../Security/Security.h"
#include "../Security/Random.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Buttons.h"

#include "../FSM/Events.h"
#include "../Program/Program.h"

#include "../System/Sleep.h"

void opt_callback_show_key(void)
{
	// Display key
	draw_clear();
	
	draw_text_index(17, 10, str_misc_hereIsKey_index);

	char outputText[20];
	encode_16B(KEY, outputText);
	draw_text(0, 30, outputText, 20);

	security_erase_data(outputText, 20);

	draw_update();

	DISABLE_SLEEP();
	while(buttons_pressed());
	program_pause_until_event(EVENT_ALL_BUTTONS);
	ENABLE_SLEEP();
}

void opt_callback_inverse_screen(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_ORIENTATION));
}

void opt_callback_change_keyboard(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_QWERTY));
}

void opt_callback_full_reset(void)
{
	DISABLE_SLEEP();

	// Erase all fram memory
	draw_clear();
	draw_text_index(17, 40, str_recovery_eraseMem_index);
	draw_update();

	progress_begin(FRAM_BYTE_SIZE/256);

	// Do .. While loop is 6 bytes smaller than for loop in this case
	uint8_t i = FRAM_BYTE_SIZE/256;
	do
	{
		--i;
		fram_set(i*256, 0, 128);
		fram_set(i*256+128, 0, 128);
		progress_add(1);
	}while(i);
	progress_end();

	// Clear variables
	CURRENT_PASSWORD_ID = GLOBALS_EVENTS = NUM_PWD = OPTIONS_FLAG = 0;
	random_reset();

	// change_master_key will enable sleep when it return
	// Get a master key
	change_master_key();

	// Device is now initialized !
	update_opt_flags((1<<OPTIONS_FLAG_OFFSET_INITIALIZED));
}

void opt_callback_enter_key(void)
{
	char usrKey[21] = {0};
	type_string(usrKey, 20);
	decode_16B(usrKey, KEY);
}