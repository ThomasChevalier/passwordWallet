#include "Transitions.h"

#include <string.h>

#include "Authentification.h"
#include "Events.h"
#include "States.h"

#include "Oled.h"
#include "String.h"


DECLARE_TRANSITION(STATE_INIT)
{
	if(!check_key())
	{
		wait_for_valid_card();
		return STATE_INIT;
	}
	return STATE_MAIN;
}

DECLARE_TRANSITION(STATE_MAIN)
{
	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	if(event & EVENT_BUTTON_1)
	{

	}
	else if(event & EVENT_BUTTON_2)
	{
	}
	else if(event & EVENT_BUTTON_3)
	{

	}
	else if(event & EVENT_BUTTON_4)
	{

	}

	return STATE_MAIN;
}

DECLARE_TRANSITION(STATE_BROWSE)
{

	return STATE_BROWSE;
}

void drawOptionMenu(uint8_t currentChoice)
{
	const char* strTable[6] = {
	str_option_genNew,
	str_option_changePwd,
	str_option_changeUsr,
	str_option_delPwd,
	str_option_addPwd,
	str_option_changeMasterKey
	};

	uint8_t pos;
	if(currentChoice == 0)
		pos = 0;
	else if(currentChoice == 5)
		pos = 3;
	else
		pos = 2;

	if(currentChoice > 0)
		--currentChoice;

	uint8_t i = 0;
	for(; i < 3; ++i)
	{
		str_to_buffer(strTable[currentChoice]);
		++currentChoice;
		oled_draw_text(0, i*8, str_buffer, 0);
	}
	for(i = 0; i < 8; ++i)
	{
		oled_h_line(0, pos * 8 + i, 128, INVERSE);
	}
}

DECLARE_TRANSITION(STATE_OPTION)
{
	const uint8_t numberOfChoice = 6;
	static uint8_t currentChoice = 0;

	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	oled_clear_display();
	drawOptionMenu(currentChoice);
	oled_display();

	if(event & EVENT_BUTTON_1)
	{
		// Looping
		currentChoice = (currentChoice == 0) ? numberOfChoice-1 : currentChoice - 1;
		oled_clear_display();
		drawOptionMenu(currentChoice);
		oled_display();
		return STATE_OPTION;
	}
	else if(event & EVENT_BUTTON_2)
	{
		switch(currentChoice)
		{
		case 0:
			//pwd_generateNew();
			break;
		case 1:
			//pwd_change();
			break;
		case 2:
			//username_change();
			break;
		case 3:
			// pwd_delete();
			break;
		case 4:
			// pwd_add_new();
			break;
		case 5:
			// change_master_key();
			break;
		default:
			break;
		}
		return STATE_MAIN;
	}
	else if(event & EVENT_BUTTON_3)
	{
		// Looping
		currentChoice = (currentChoice == numberOfChoice-1) ? 0 : currentChoice + 1;
		oled_clear_display();
		drawOptionMenu(currentChoice);
		oled_display();
		return STATE_OPTION;
	}

	return STATE_OPTION;
}

DECLARE_TRANSITION(STATE_SAVE) 
{
	return STATE_SAVE;
}
