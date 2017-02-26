#include "State_Options.h"

#include <util/delay.h>
#include <string.h>

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/String.h"
#include "../../Graphics/ProgressBar.h"
#include "../../Graphics/Drawing.h"

#include "../../Hardware/Fram.h"

#include "../../Security/Security.h"
#include "../../Security/Password_List.h"
#include "../../Security/Authentification.h"

static uint8_t currentChoice = 0;

void state_options_begin (void)
{
	currentChoice = 0;
	draw_list(currentChoice, 0, OPTION_MENU_MAX_CHOICE-1);
}


static void do_inverse_screen(void)
{
	OPTIONS_FLAG ^= (1<<OPTIONS_FLAG_OFFSET_ORIENTATION);
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

static void do_new_pwd(void)
{
	if(NUM_PWD == MAXIMUM_NUMBER_OF_PWD-1)
	{
		draw_clear();
		str_to_buffer(str_error_not_enough_memory_index);
		draw_text(0, 10, str_buffer, 0);
		draw_update();
		_delay_ms(2000);
		return;
	}
	char usrName[64] = {0};
	
	char pwdName[32] = {0};
	char pwdData[32] = {0};

	str_to_buffer(str_order_typePwdName_index);
	draw_clear();
	draw_text(0, 10, str_buffer, 0);
	draw_update();
	_delay_ms(1000);
	type_string(pwdName,31);

	str_to_buffer(str_order_typePwdData_index);
	draw_clear();
	draw_text(0, 10, str_buffer, 0);
	draw_update();
	_delay_ms(1000);
	type_string(pwdData,31);

	str_to_buffer(str_order_typeUsrName_index);
	draw_clear();
	draw_text(0, 10, str_buffer, 0);
	draw_update();
	_delay_ms(1000);
	type_string(usrName,63); // usr name

	draw_clear();
	str_option_to_buffer(str_option_addPwd_index);
	draw_text(12, 40, str_buffer, 0);
	uint8_t strLen = strlen(pwdData);
	uint8_t cpx = 21 + (32-strLen);
	cpx += 21 +(64-strlen(usrName));
	progress_begin(cpx);

	uint8_t result = pwd_list_add_pwd((uint8_t*)pwdName, (uint8_t*)pwdData, (uint8_t*)usrName);
	progress_end();

	security_erase_data(pwdData, 32);
	security_erase_data(usrName, 64);

	if(!result)
	{
		draw_clear();
		str_to_buffer(str_error_not_enough_memory_index);
		draw_text(0, 10, str_buffer, 0);
		str_to_buffer(str_error_cannot_add_pwd_index);
		draw_text(0, 22, str_buffer, 0);
		draw_update();
		_delay_ms(2000);
	}
}


uint8_t state_options_transition (uint8_t event)
{
	if(event & EVENT_INIT_COMMUNICATION)
	{
		return STATE_COMMUNICATION;
	}
	
	if(event & EVENT_BUTTON_1)
	{
		if(currentChoice == 0)
		{
			currentChoice = OPTION_MENU_MAX_CHOICE - 1;
		}
		else
		{
			--currentChoice;
		}
	}
	else if(event & EVENT_BUTTON_2)
	{
		// if else statements is bigger in source code than switch for this case
		switch(currentChoice)
		{
			case 0:
				return STATE_OPTION_PASSWORD;
			case 1:
				return STATE_OPTION_SORT;
			case 2:
				return STATE_OPTION_ADVANCED;
			case 3:
				do_new_pwd();
				return STATE_MAIN;
			case 4:
				do_inverse_screen();
				return STATE_MAIN;
			default:
				// Impossible
				break;
		}
	}
	else if(event & EVENT_BUTTON_3)
	{
		++currentChoice;
		if(currentChoice == OPTION_MENU_MAX_CHOICE)
		{
			currentChoice = 0;
		}
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_BROWSE;
	}

	if(event & EVENT_ALL_BUTTONS)
	{
		draw_list(currentChoice, 0, OPTION_MENU_MAX_CHOICE-1);
	}

	return STATE_OPTION;
}

void state_options_end (void)
{

}
