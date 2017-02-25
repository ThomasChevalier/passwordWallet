#include "State_Main.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

#include "../../Hardware/Keyboard.h"

#include "../../Security/Security.h"
#include "../../Security/Password.h"
#include "../../Security/Password_List.h"

void state_main_begin (void)
{
	draw_main_menu();
}

uint8_t state_main_transition (uint8_t event)
{
	// If there is no password don't navigate in the list or send password
	if( ((event & EVENT_BUTTON_1) || (event & EVENT_BUTTON_2) || (event & EVENT_BUTTON_3))
		&& NUM_PWD == 0)
	{
		return STATE_MAIN;
	}
	
	if(event & EVENT_BUTTON_1)
	{
		CURRENT_PASSWORD_ID = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_2)
	{
		uint8_t pwd_data[32] = {0};
		password_read_data(CURRENT_PASSWORD_ID, pwd_data, KEY);

		keyboard_send((char*)pwd_data, strlen((char*)pwd_data));

		security_erase_data(pwd_data, 32);
		
		password_increment_counter(CURRENT_PASSWORD_ID);
		if(pwd_list_get_sorting_method() == PWD_SORTING_USAGE)
		{
			pwd_list_sort_usage();
		}
	}
	else if(event & EVENT_BUTTON_3)
	{
		CURRENT_PASSWORD_ID = pwd_list_get_next_pwd_id(CURRENT_PASSWORD_ID);
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
