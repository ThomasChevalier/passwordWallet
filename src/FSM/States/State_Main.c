#include "State_Main.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

#include "../../Hardware/Keyboard.h"

#include "../../Security/Password.h"
#include "../../Security/Password_List.h"

void state_main_begin (void)
{
	password_read_data(CURRENT_PASSWORD_ID, CURRENT_PASSWORD_DATA, KEY);
	draw_main_menu();
}

uint8_t state_main_transition (uint8_t event)
{
	if(event & EVENT_BUTTON_1)
	{
		if(NUM_PWD > 0)
		{
			CURRENT_PASSWORD_ID = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
			password_read_data(CURRENT_PASSWORD_ID, CURRENT_PASSWORD_DATA, KEY);
		}
		
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_2)
	{
		keyboard_send((char*)CURRENT_PASSWORD_DATA, strlen((char*)CURRENT_PASSWORD_DATA));
		password_increment_counter(CURRENT_PASSWORD_ID);
	}
	else if(event & EVENT_BUTTON_3)
	{
		if(NUM_PWD > 0)
		{
			CURRENT_PASSWORD_ID = pwd_list_get_next_pwd_id(CURRENT_PASSWORD_ID);
			password_read_data(CURRENT_PASSWORD_ID, CURRENT_PASSWORD_DATA, KEY);
		}
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
