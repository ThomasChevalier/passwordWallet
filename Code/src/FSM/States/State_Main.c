#include "State_Main.h"

#include <util/atomic.h>

#include "../../Globals.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

#include "../../Security/Security.h"
#include "../../Security/Password.h"
#include "../../Security/Password_List.h"

#include "../../Options/Options.h"

#include "../../Hardware/Buttons.h"

void state_main_begin (void)
{
	draw_main_menu();
}

uint8_t state_main_transition (uint8_t event)
{
	//static uint8_t but2Pressed = FALSE;

	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	// If there is no password don't navigate in the list or send password
	if( (event & (EVENT_BUTTON_1 | EVENT_BUTTON_2 | EVENT_BUTTON_3))
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
		options_display(OPTIONS_LIST_SEND);
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
