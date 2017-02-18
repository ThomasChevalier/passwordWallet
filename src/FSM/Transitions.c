#include "Transitions.h"

#include <string.h>
#include <util/delay.h>

#include "../Globals.h"

#include "Events.h"
#include "States.h"

#include "../Security/Authentification.h"
#include "../Security/Passwords.h"

#include "../Hardware/Keyboard.h"
#include "../Hardware/Rfid.h"
#include "../Hardware/Fram.h"
#include "../Hardware/Oled.h"

#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"
#include "../Graphics/ProgressBar.h"


DECLARE_TRANSITION(STATE_INIT)
{
	if(event & EVENT_USB_DISCONNECTED)
		return STATE_SAVE;
	
	/*if(!check_key())
	{
		wait_for_valid_card();
		return STATE_INIT;
	}*/
	
	// Save some mA
	rfid_power_down();

	// Read the flags and data from fram
  	OPTIONS_FLAG = fram_read_byte(OFFSET_OPTIONS_FLAG);
  	FIRST_PWD_UTIL = fram_read_byte(OFFSET_FIRST_PWD_UTIL);;
 	FIRST_PWD_ALPHA = fram_read_byte(OFFSET_FIRST_PWD_ALPHA);
 	NUM_PWD = fram_read_byte(OFFSET_NUM_PWD);

 	// Delete all entropy pool, because it cannot be trusted (someone may have corrupt the data)
 	uint16_t entropyPoolSize = 0;
    fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);

    // Read the memory map
    fram_read_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, SIZE_MEMORY_MAP);

  	goto_first_pwd();
	read_all_names();
	draw_main_menu();

	return STATE_MAIN;
}


DECLARE_TRANSITION(STATE_MAIN)
{
	if(event & EVENT_USB_DISCONNECTED)
	{
		return STATE_SAVE;
	}

	if(event & EVENT_BUTTON_1)
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
		draw_update();
		return STATE_BROWSE;
	}

	return STATE_MAIN;
}


DECLARE_TRANSITION(STATE_BROWSE)
{
	static char letter = '@';

	if(event & EVENT_USB_DISCONNECTED)
	{
		return STATE_SAVE;
	}

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

static void show_executing(void)
{
	draw_clear();
	str_to_buffer(str_action_executing_index);
	draw_text(33, 28, str_buffer, 0);
	draw_update();
}

void do_action(uint8_t action)
{
	char tempStr[64] = {0};

	switch(action)
		{
		case 0:
			{
				draw_clear();
				str_to_buffer(str_option_regen_index);
				draw_text(40, 40, str_buffer, 0);
				progress_begin(52);		

				generate_password(tempStr);
				set_password((uint8_t*)(tempStr), 31, KEY);

				progress_end();
			}
			break;
		case 1:
			{
				memcpy(tempStr, CURRENT_PASSWORD_DATA, 32);
				type_string(tempStr, 32);

				draw_clear();
				str_to_buffer(str_option_changePwd_index);
				draw_text(14, 40, str_buffer, 0);
				const uint8_t strLen = strlen(tempStr);
				progress_begin(21 + 32 - strLen);	

				set_password((uint8_t*)(tempStr), strLen, KEY);

				progress_end();
			}
			break;
		case 2:
			{
				memcpy(tempStr, CURRENT_USR_NAME, 64);
				type_string(tempStr, 64);
	
				draw_clear();
				str_to_buffer(str_option_changeUsr_index);
				draw_text(17, 40, str_buffer, 0);
				const uint8_t strLen = strlen(tempStr);
				progress_begin(21 + 64 - strLen);	
	
				set_username((uint8_t*)(tempStr), strLen, KEY);

				progress_end();
			}
			break;
		case 3:
			show_executing();
			// pwd_delete();
			break;
		case 4:
			{
				char pwdName[32] = {0};
				char pwdData[32] = {0};
				
				str_to_buffer(str_order_typePwdName_index);
				draw_clear();
				draw_text(20, 10, str_buffer, 0);
				draw_update();
				_delay_ms(2000);
				type_string(pwdName,31);

				str_to_buffer(str_order_typePwdData_index);
				draw_clear();
				draw_text(20, 10, str_buffer, 0);
				draw_update();
				_delay_ms(2000);
				type_string(pwdData,31);

				str_to_buffer(str_order_typeUsrName_index);
				draw_clear();
				draw_text(20, 10, str_buffer, 0);
				draw_update();
				_delay_ms(2000);
				type_string(tempStr,63); // usr name

				draw_clear();
				str_to_buffer(str_option_addPwd_index);
				draw_text(17, 40, str_buffer, 0);
				uint8_t strLen = strlen(pwdData);
				uint8_t cpx = 21 + (32-strLen);
				cpx += 21 +(64-strlen(tempStr));
				progress_begin(cpx);

				add_password(pwdName, pwdData, tempStr);

				progress_end();
			}
			
			break;
		case 5:
			change_master_key();
			break;
		case 6:
			OPTIONS_FLAG ^= (1<<3);
			fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
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
	{
		return STATE_SAVE;
	}

	if(event & EVENT_BUTTON_1)
	{
		if(!confirming)
		{
			// Looping
			currentChoice = (currentChoice == 0) ? 6 : currentChoice - 1;
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
			read_all_names();
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
			currentChoice = (currentChoice == 6) ? 0 : currentChoice + 1;
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
