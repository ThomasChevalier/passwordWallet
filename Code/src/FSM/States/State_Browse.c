#include "State_Browse.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"

static char letter = '@';

void state_browse_begin (void)
{
	draw_main_menu();
	draw_browse_dock(letter,1);
	draw_update();	
}

uint8_t state_browse_transition (uint8_t event)
{
	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	if(event & EVENT_BUTTON_1)
	{
		letter = (letter == '@') ? 'Z' : letter - 1;
		draw_browse_dock(letter,1);
		draw_update();
	}
	else if(event & EVENT_BUTTON_2)
	{
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
		return STATE_OPTION;
	}
	return STATE_BROWSE;
}

void state_browse_end (void)
{

}
