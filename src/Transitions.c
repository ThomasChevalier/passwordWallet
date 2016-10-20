#include "Transitions.h"

#include <string.h>

#include "Authentification.h"
#include "Passwords.h"
#include "Keyboard.h"
#include "Globals.h"

#include "Events.h"
#include "States.h"

#include "String.h"
#include "Oled.h"



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
		CURRENT_PASSWORD_ID = prev_pwd(CURRENT_PASSWORD_ID);
		read_pwd_name(PWD_NAME_1, prev_pwd(CURRENT_PASSWORD_ID));
		read_pwd_name(PWD_NAME_2, CURRENT_PASSWORD_ID);
		read_pwd_name(PWD_NAME_3, next_pwd(CURRENT_PASSWORD_ID));
	}
	else if(event & EVENT_BUTTON_2)
	{
		keyboard_send((char*)CURRENT_PASSWORD_DATA, strlen((char*)CURRENT_PASSWORD_DATA));
	}
	else if(event & EVENT_BUTTON_3)
	{
		CURRENT_PASSWORD_ID = next_pwd(CURRENT_PASSWORD_ID);
		read_pwd_name(PWD_NAME_1, prev_pwd(CURRENT_PASSWORD_ID));
		read_pwd_name(PWD_NAME_2, CURRENT_PASSWORD_ID);
		read_pwd_name(PWD_NAME_3, next_pwd(CURRENT_PASSWORD_ID));
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_BROWSE;
	}

	return STATE_MAIN;
}

void draw_browse_dock(char letter)
{
	uint8_t x = 0;
	uint8_t y = 0;
	for(; y < 64; ++y)
	{
		for(x = 0; x < 7; ++x)
		{
			oled_draw_pixel(x,y,BLACK);
		}
	}
	oled_v_line(7, 0, 64, WHITE);
	x = letter - 3;
	y = 0;
	for(; x <= letter + 3; ++x )
	{
		char c = x;
		if(x < '@')
			c += '@';
		if(x > 'Z')
			c-='@';
		oled_draw_char(1, y*9, c);
		++y;
	}
	for(y = 0; y < 9; ++y)
		oled_h_line(0, 27+y, 7, INVERSE);
	oled_display();
}


DECLARE_TRANSITION(STATE_BROWSE)
{
	static char letter = '@';

	draw_browse_dock(letter);

	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	if(event & EVENT_BUTTON_1)
	{
		letter = (letter == '@') ? 'Z' : letter - 1;
		draw_browse_dock(letter);
	}
	else if(event & EVENT_BUTTON_2)
	{
		return STATE_MAIN;
	}
	else if(event & EVENT_BUTTON_3)
	{
		letter = (letter == 'Z') ? '@' : letter + 1;
		draw_browse_dock(letter);
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_OPTION;
	}
	return STATE_BROWSE;
}

void draw_option_menu(uint8_t currentChoice)
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
	draw_option_menu(currentChoice);
	oled_display();

	if(event & EVENT_BUTTON_1)
	{
		// Looping
		currentChoice = (currentChoice == 0) ? numberOfChoice-1 : currentChoice - 1;
		oled_clear_display();
		draw_option_menu(currentChoice);
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
		draw_option_menu(currentChoice);
		oled_display();
		return STATE_OPTION;
	}

	return STATE_OPTION;
}

DECLARE_TRANSITION(STATE_SAVE) 
{
	return STATE_SAVE;
}
