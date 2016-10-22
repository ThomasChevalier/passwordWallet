#include "Transitions.h"

#include <string.h>

#include "Authentification.h"
#include "Passwords.h"
#include "Keyboard.h"
#include "Globals.h"

#include "Rfid.h"

#include "Events.h"
#include "States.h"

#include "Oled.h"
#include "Drawing.h"


DECLARE_TRANSITION(STATE_INIT)
{
	/*if(!check_key())
	{
		wait_for_valid_card();
		return STATE_INIT;
	}*/
	
	rfid_power_down();

	// Read the flags and data from fram
  	OPTIONS_FLAG = fram_read_byte(0);
  	fram_read_bytes(1, (uint8_t*)(&FIRST_PWD_UTIL), 2);
 	fram_read_bytes(3, (uint8_t*)(&FIRST_PWD_ALPHA), 2);
 	fram_read_bytes(5, (uint8_t*)(&NUM_PWD), 2);

  	goto_first_pwd();
	read_all_names();
	draw_main_menu();

	return STATE_MAIN;
}


DECLARE_TRANSITION(STATE_MAIN)
{
	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	if((event & EVENT_BUTTON_1) && (event & EVENT_BUTTON_2))
	{
		ORIENTATION ^= 1;
	}
	else if(event & EVENT_BUTTON_1)
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
		oled_display();
		return STATE_BROWSE;
	}

	return STATE_MAIN;
}


DECLARE_TRANSITION(STATE_BROWSE)
{
	static char letter = '@';

	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	draw_browse_dock(letter,1);
	oled_display();	

	if(event & EVENT_BUTTON_1)
	{
		letter = (letter == '@') ? 'Z' : letter - 1;
		draw_browse_dock(letter,1);
		oled_display();
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
		oled_display();
	}
	else if(event & EVENT_BUTTON_4)
	{
		draw_option_menu(0);
		return STATE_OPTION;
	}
	return STATE_BROWSE;
}

void do_action(uint8_t action)
{
	switch(action)
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
}

DECLARE_TRANSITION(STATE_OPTION)
{
	static uint8_t currentChoice = 0;
	static uint8_t confirming = 0;
	static uint8_t confirmYesNo = 0;

	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;

	if(event & EVENT_BUTTON_1)
	{
		if(!confirming)
		{
			// Looping
			currentChoice = (currentChoice == 0) ? 5 : currentChoice - 1;
			draw_option_menu(currentChoice);
		}
		else
		{
			confirmYesNo ^= 1;
			draw_confirmation_screen(currentChoice, confirmYesNo);
		}
		
	}
	else if(event & EVENT_BUTTON_2)
	{
		if(confirming && confirmYesNo)
		{
			confirming = 0;
			do_action(currentChoice);
			currentChoice = 0;
			draw_main_menu();
			return STATE_MAIN;
		}
		else if (confirming)
		{
			confirming = 0;
			draw_option_menu(currentChoice);
		}
		else
		{
			confirming = 1;
			confirmYesNo = 0;
			draw_confirmation_screen(currentChoice, confirmYesNo);
		}
		
	}
	else if(event & EVENT_BUTTON_3)
	{
		if(!confirming)
		{
			// Looping
			currentChoice = (currentChoice == 5) ? 0 : currentChoice + 1;
			draw_option_menu(currentChoice);
		}
		else
		{
			confirmYesNo ^= 1;
			draw_confirmation_screen(currentChoice, confirmYesNo);
		}
		
	}
	else if(event & EVENT_BUTTON_4)
	{
		if(!confirming)
		{
			currentChoice = 0;
			draw_main_menu();
			return STATE_MAIN;
		}
		else
		{
			confirming = 0;
			draw_option_menu(currentChoice);
		}
		
	}

	return STATE_OPTION;
}

DECLARE_TRANSITION(STATE_SAVE) 
{
	return STATE_SAVE;
}
