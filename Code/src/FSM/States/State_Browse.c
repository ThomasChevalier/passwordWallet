#include "State_Browse.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Security/Password_List.h"
#include "../../Security/Password.h"

#include "../../Hardware/Oled.h"

#include "../../Graphics/Drawing.h"

#include "../../Options/Options.h"

static char letter = '@';

void state_browse_begin (void)
{
	draw_browse_dock(letter,1);
	draw_update();	
}

/**
 * @brief This function set CURRENT_PASSWORD_ID to the password whose the first letter match the most the letter
 * @details It take some time to return ...
 */
static void select_pwd_from_letter(void)
{
	const p_addr firstId = pwd_list_get_first_pwd_id_sort_alpha();
	p_addr pwdId = firstId;

	uint8_t minDif = 255;

	// We can use oled_data because the first 32 bytes will be updated
	uint8_t *name = oled_data;
	do
	{
		password_read_name(pwdId, name);

		// Upper first character
		if( name[0] >= 'a' && name[0] <= 'z'){
			name[0] = name[0] - ('a' - 'A');
		}
		// Transform other char in symbol
		if( name[0] < 'A' || name[0] > 'Z'){
			name[0] = '@';
		}
		if(name[0] == letter)
		{
			CURRENT_PASSWORD_ID = pwdId;
			break;
		}
		else if(name[0] > letter && name[0] - letter < minDif)
		{
			minDif = name[0] - letter;
			CURRENT_PASSWORD_ID = pwdId;
		}

		pwdId = pwd_list_get_next_pwd_id_sort_alpha(pwdId);
	}while(pwdId != firstId);
}

uint8_t state_browse_transition (uint8_t event)
{
	if(event & EVENT_BUTTON_1)
	{
		letter = (letter == '@') ? 'Z' : letter - 1;
		
	}
	else if(event & EVENT_BUTTON_2)
	{
		select_pwd_from_letter();
		return STATE_MAIN;
	}
	else if(event & EVENT_BUTTON_3)
	{
		letter = (letter == 'Z') ? '@' : letter + 1;
	}
	else if(event & EVENT_BUTTON_4)
	{
		options_display(OPTIONS_LIST_NORMAL);
		return STATE_MAIN;
	}

	if(event & (EVENT_BUTTON_1 | EVENT_BUTTON_3))
	{
		draw_browse_dock(letter,1);
		draw_update();
	}

	return STATE_BROWSE;
}

void state_browse_end (void)
{

}
