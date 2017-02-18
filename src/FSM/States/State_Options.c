#include "State_Options.h"

#include <util/delay.h>
#include <string.h>

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/String.h"
#include "../../Graphics/ProgressBar.h"
#include "../../Graphics/Drawing.h"

#include "../../Hardware/Fram.h"

#include "../../Security/Passwords.h"
#include "../../Security/Authentification.h"

void state_options_begin (void)
{

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

uint8_t state_options_transition (uint8_t event)
{
	static uint8_t currentChoice = 0;
	static uint8_t confirming = 0;
	static uint8_t confirmYesNo = 0;

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

void state_options_end (void)
{

}