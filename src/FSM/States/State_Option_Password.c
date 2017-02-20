#include "State_Option_Password.h"

#include <string.h>

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"
#include "../../Graphics/ProgressBar.h"
#include "../../Graphics/String.h"

#include "../../Security/Password.h"
#include "../../Security/Password_List.h"

static uint8_t currentChoice = 0;

static void do_regenerate_pwd(void)
{
	draw_clear();
	str_option_to_buffer(str_option_pwd_regenerate_index);
	draw_text(40, 40, str_buffer, 0);
	progress_begin(52);		

	password_regenerate(CURRENT_PASSWORD_ID);

	progress_end();
}

static void do_change_pwd(void)
{
	char tempPwd[32] = {0};
	memcpy(tempPwd, CURRENT_PASSWORD_DATA, 32);
	type_string(tempPwd, 31);

	draw_clear();
	str_option_to_buffer(str_option_pwd_changePwd_index);
	draw_text(14, 40, str_buffer, 0);
	const uint8_t strLen = strlen(tempPwd);
	progress_begin(21 + 32 - strLen);	

	password_set_data(CURRENT_PASSWORD_ID, (uint8_t*)tempPwd, strLen, KEY);

	progress_end();
}

static void do_change_id(void)
{
	char tempStr[64] = {0};
	memcpy(tempStr, CURRENT_USR_NAME, 64);
	type_string(tempStr, 63);

	draw_clear();
	str_option_to_buffer(str_option_pwd_changeUsrName_index);
	draw_text(17, 40, str_buffer, 0);
	const uint8_t strLen = strlen(tempStr);
	progress_begin(21 + 64 - strLen);

	password_set_usr_name(CURRENT_PASSWORD_ID, (uint8_t*)(tempStr), strLen, KEY);

	progress_end();
}

static void do_change_name(void)
{
	char name[32];
	password_read_name(CURRENT_PASSWORD_ID, (uint8_t*)name);
	type_string(name, 31);

	password_set_name(CURRENT_PASSWORD_ID, (uint8_t*)name, 32);
}

static void do_delete_password(void)
{
	uint8_t prevPwd = 0;
	if(NUM_PWD > 1)
	{
		prevPwd = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
	}
	
	pwd_list_delete_pwd(CURRENT_PASSWORD_ID);

	CURRENT_PASSWORD_ID = prevPwd;
}

void state_option_password_begin (void)
{
	currentChoice = 0;
	draw_list(	currentChoice + OPTION_PASSWORD_MENU_MIN_STR_INDEX,
				OPTION_PASSWORD_MENU_MIN_STR_INDEX,
				OPTION_PASSWORD_MENU_MIN_STR_INDEX + OPTION_PASSWORD_MENU_MAX_CHOICE-1);
}

uint8_t state_option_password_transition (uint8_t event)
{
	if(event & EVENT_BUTTON_1)
	{
		if(currentChoice == 0)
		{
			currentChoice = OPTION_PASSWORD_MENU_MAX_CHOICE - 1;
		}
		else
		{
			--currentChoice;
		}
	}
	else if(event & EVENT_BUTTON_2)
	{
		if(NUM_PWD == 0)
		{
			return STATE_MAIN;
		}
		switch(currentChoice)
		{
			case 0:
				do_regenerate_pwd();
				return STATE_MAIN;
			case 1:
				do_change_pwd();
				return STATE_MAIN;
			case 2:
				do_change_id();
				return STATE_MAIN;
			case 3:
				do_change_name();
				return STATE_MAIN;
			case 4:
				do_delete_password();
				return STATE_MAIN;
			default:
				// Impossible
				break;
		}
	}
	else if(event & EVENT_BUTTON_3)
	{
		++currentChoice;
		if(currentChoice == OPTION_PASSWORD_MENU_MAX_CHOICE)
		{
			currentChoice = 0;
		}
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_OPTION;
	}

	if(event & EVENT_ALL_BUTTONS)
	{
		draw_list(	currentChoice + OPTION_PASSWORD_MENU_MIN_STR_INDEX,
				OPTION_PASSWORD_MENU_MIN_STR_INDEX,
				OPTION_PASSWORD_MENU_MIN_STR_INDEX + OPTION_PASSWORD_MENU_MAX_CHOICE-1);
	}

	return STATE_OPTION_PASSWORD;
}

void state_option_password_end (void)
{

}
