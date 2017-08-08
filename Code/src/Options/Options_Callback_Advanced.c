#include "Options_Callback_Advanced.h"

#include "../Globals.h"

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"
#include "../Graphics/Ascii85.h"

#include "../Security/Security.h"

#include "../FSM/Events.h"
#include "../Program/Program.h"

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

	program_pause_until_event(EVENT_ALL_BUTTONS);
}

void opt_callback_inverse_screen(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_ORIENTATION));
}

void opt_callback_change_keyboard(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_QWERTY));
}