#include "State_Browse.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

void state_browse_begin (void)
{

}

uint8_t state_browse_transition (uint8_t event)
{
	static char letter = '@';

	draw_browse_dock(letter,1);
	draw_update();	

	if(event & EVENT_BUTTON_1)
	{
		letter = (letter == '@') ? 'Z' : letter - 1;
		draw_browse_dock(letter,1);
		draw_update();
	}
	else if(event & EVENT_BUTTON_2)
	{
		draw_main_menu();
		return STATE_MAIN;
	}
	else if(event & EVENT_BUTTON_3)
	{
		letter = (letter == 'Z') ? '@' : letter + 1;
		draw_browse_dock(letter,1);
		draw_update();
	}
	else if(event & EVENT_BUTTON_4)
	{
		draw_option_menu(0);
		return STATE_OPTION;
	}
	return STATE_BROWSE;
}

void state_browse_end (void)
{

}
