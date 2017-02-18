#include "State_Main.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

#include "../../Hardware/Keyboard.h"

#include "../../Security/Passwords.h"

void state_main_begin (void)
{
	read_all_names();
	draw_main_menu();
}

uint8_t state_main_transition (uint8_t event)
{
	if(event & EVENT_BUTTON_1)
	{
		CURRENT_PASSWORD_ID = prev_pwd(CURRENT_PASSWORD_ID);
		read_all_names();
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_2)
	{
		keyboard_send((char*)CURRENT_PASSWORD_DATA, strlen((char*)CURRENT_PASSWORD_DATA));
		increment_pwd_counter();
	}
	else if(event & EVENT_BUTTON_3)
	{
		CURRENT_PASSWORD_ID = next_pwd(CURRENT_PASSWORD_ID);
		read_all_names();
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_4)
	{
		draw_browse_dock(0,1);
		draw_update();
		return STATE_BROWSE;
	}

	return STATE_MAIN;
}

void state_main_end (void)
{
	
}