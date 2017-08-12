#include "State_Main.h"

#include "../../Globals.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

#include "../../USB/Keyboard.h"

#include "../../Security/Security.h"
#include "../../Security/Password.h"
#include "../../Security/Password_List.h"

#include "../../Hardware/Oled.h"

void state_main_begin (void)
{
	draw_main_menu();
}

uint8_t state_main_transition (uint8_t event)
{
	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
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
		uint8_t* pwd_data = oled_data; // Use the oled buffer

		password_read_usr_name(CURRENT_PASSWORD_ID, pwd_data, KEY);
		if(pwd_data[0] != 0){ // If the user name is not empty
			keyboard_send((char*)pwd_data, strlen_bound((char*)pwd_data, 64));
			// Send tab
			keyboard_send_key_next();
		}

		password_read_data(CURRENT_PASSWORD_ID, pwd_data, KEY);

		keyboard_send((char*)pwd_data, strlen_bound((char*)pwd_data, 32));

		security_erase_data(pwd_data, 64);
		
		password_increment_counter(CURRENT_PASSWORD_ID);
		pwd_list_sort_use();

		// The menu can have changed (with pwd_list_sort_use())
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_3)
	{
		CURRENT_PASSWORD_ID = pwd_list_get_next_pwd_id(CURRENT_PASSWORD_ID);
		draw_main_menu();
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_BROWSE;
	}

	return STATE_MAIN;
}

void state_main_end (void)
{
	
}
