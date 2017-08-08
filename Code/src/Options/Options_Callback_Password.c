#include "Options_Callback_Password.h"

#include <string.h>
#include <util/delay.h>

#include "../Globals.h"

#include "../Graphics/Drawing.h"
#include "../Graphics/ProgressBar.h"
#include "../Graphics/String.h"

#include "../Security/Security.h"
#include "../Security/Password.h"
#include "../Security/Password_List.h"

void opt_callback_add_pwd(void)
{
if(NUM_PWD == MAXIMUM_NUMBER_OF_PWD-1)
	{
		draw_clear();
		draw_text_index(0, 10, str_error_not_enough_memory_index);
		draw_update();
		_delay_ms(2000);
		return;
	}
	char usrName[64] = {0};
	
	char pwdName[32] = {0};
	char pwdData[32] = {0};

	draw_clear();
	draw_text_index(0, 10, str_order_typePwdName_index);
	draw_update();
	_delay_ms(1000);
	type_string(pwdName,31);

	draw_clear();
	draw_text_index(0, 10, str_order_typePwdData_index);
	draw_update();
	_delay_ms(1000);
	type_string(pwdData,31);

	draw_clear();
	draw_text_index(0, 10, str_order_typeUsrName_index);
	draw_update();
	_delay_ms(1000);
	type_string(usrName,63); // usr name

	draw_clear();
	str_to_buffer(str_option_addPwd_index);
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
		draw_text_index(0, 10, str_error_not_enough_memory_index);
		draw_text_index(0, 22, str_error_cannot_add_pwd_index);
		draw_update();
		_delay_ms(2000);
	}
}

void opt_callback_regenerate_pwd(void)
{
	draw_clear();
	str_to_buffer(str_option_pwd_regenerate_index);
	draw_text(40, 40, str_buffer, 0);
	progress_begin(52);		

	password_regenerate(CURRENT_PASSWORD_ID);

	progress_end();
}


void opt_callback_change_pwd(void)
{
	uint8_t tempPwd[32] = {0};

	password_read_data(CURRENT_PASSWORD_ID, tempPwd, KEY);

	if(!type_string((char*)tempPwd, 31))
	{
		// If nothing changed
		return;
	}

	draw_clear();
	str_to_buffer(str_option_pwd_changePwd_index);
	draw_text(14, 40, str_buffer, 0);
	const uint8_t strLen = strlen((char*)tempPwd);
	progress_begin(21 + 32 - strLen);	

	password_set_data(CURRENT_PASSWORD_ID, tempPwd, strLen, KEY);

	security_erase_data(tempPwd, 32);

	progress_end();
}


void opt_callback_change_usr_name(void)
{
	uint8_t tempStr[64] = {0};
	password_read_usr_name(CURRENT_PASSWORD_ID, tempStr, KEY);
	
	if(!type_string((char*)tempStr, 63))
	{
		// If nothing changed
		return;
	}

	draw_clear();
	str_to_buffer(str_option_pwd_changeUsrName_index);
	draw_text(17, 40, str_buffer, 0);
	const uint8_t strLen = strlen((char*)tempStr);
	progress_begin(21 + 64 - strLen);

	password_set_usr_name(CURRENT_PASSWORD_ID, tempStr, strLen, KEY);
	security_erase_data(tempStr, 64);

	progress_end();
}


void opt_callback_change_name(void)
{
	char name[32];
	password_read_name(CURRENT_PASSWORD_ID, (uint8_t*)name);
	if(!type_string(name, 31))
	{
		// If nothing changed
		return;
	}

	password_set_name(CURRENT_PASSWORD_ID, (uint8_t*)name, 32);
	// Get the list sorted
	pwd_list_sort_alpha();
}


void opt_callback_delete_pwd(void)
{
	uint8_t prevPwd = 0;
	if(NUM_PWD > 1)
	{
		prevPwd = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
	}
	
	pwd_list_delete_pwd(CURRENT_PASSWORD_ID);

	CURRENT_PASSWORD_ID = prevPwd;
}