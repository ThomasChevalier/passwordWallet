#include "Options_Callback_Password.h"

#include "../Globals.h"

#include "../Graphics/Drawing.h"
#include "../Graphics/ProgressBar.h"
#include "../Graphics/String.h"

#include "../USB/Keyboard.h"

#include "../Security/Security.h"
#include "../Security/Password.h"
#include "../Security/Password_List.h"
#include "../Security/Backup.h"

#include "../System/Sleep.h"
#include "../Program/Program.h"

void opt_callback_add_pwd(void)
{
	if(NUM_PWD == MAXIMUM_NUMBER_OF_PWD-1)
	{
		draw_clear();
		draw_flash_str_cx(10, str_error_not_enough_mem);
		draw_update();
		program_pause_until_event(EVENT_ALL_BUTTONS);
		return;
	}
	char usrName[64] = {0};
	
	char pwdName[32] = {0};
	char pwdData[32] = {0};

	draw_clear();
	draw_flash_str_cx(10, str_order_type_pwd_name);
	draw_update();
	program_pause_until_event(EVENT_ALL_BUTTONS);
	type_string(pwdName,32);

	draw_clear();
	draw_flash_str_cx(10, str_order_type_pwd_data);
	draw_update();
	program_pause_until_event(EVENT_ALL_BUTTONS);
	type_string(pwdData,32);

	draw_clear();
	draw_flash_str_cx(10, str_order_type_usr_name);
	draw_update();
	program_pause_until_event(EVENT_ALL_BUTTONS);
	type_string(usrName,64); // usr name

	DISABLE_SLEEP();

	draw_clear();
	draw_flash_str_cx(40, str_option_add);

	progress_begin(42);

	uint8_t result = pwd_list_add_pwd((uint8_t*)pwdName, (uint8_t*)pwdData, (uint8_t*)usrName);
	progress_end();

	if(result == RETURN_ERROR)
	{
		draw_clear();
		draw_flash_str_cx(10, str_error_not_enough_mem);
		draw_flash_str_cx(22, str_error_cannot_add);
		draw_update();
		program_pause_until_event(EVENT_ALL_BUTTONS);
	}

	ENABLE_SLEEP();
}

void opt_callback_regenerate_pwd(void)
{
	if(!draw_confirmation_screen(str_option_regen_index, str_option_regen_centerX))
	{
		return;
	}

	DISABLE_SLEEP();

	draw_clear();
	draw_flash_str_cx(40, str_option_regen);
	progress_begin(53);

	backup_save(CURRENT_PASSWORD_ID, BACKUP_STATUS_UPDATE);

	password_regenerate(CURRENT_PASSWORD_ID);

	backup_free();

	progress_end();

	ENABLE_SLEEP();
}


void opt_callback_change_pwd(void)
{
	uint8_t tempPwd[32];

	password_read_data(CURRENT_PASSWORD_ID, tempPwd, KEY);

	if(!type_string((char*)tempPwd, 32))
	{
		// If nothing changed
		return;
	}

	DISABLE_SLEEP();

	draw_clear();
	draw_flash_str_cx(40, str_option_change_pwd);
	progress_begin(21);	

	backup_save(CURRENT_PASSWORD_ID, BACKUP_STATUS_UPDATE);

	password_set_data(CURRENT_PASSWORD_ID, tempPwd, KEY);

	backup_free();

	progress_end();

	ENABLE_SLEEP();
}


void opt_callback_change_usr_name(void)
{
	uint8_t tempStr[64];
	password_read_usr_name(CURRENT_PASSWORD_ID, tempStr, KEY);
	
	if(!type_string((char*)tempStr, 64))
	{
		// If nothing changed
		return;
	}

	DISABLE_SLEEP();

	draw_clear();
	draw_flash_str_cx(40, str_option_change_usr);
	progress_begin(21);

	backup_save(CURRENT_PASSWORD_ID, BACKUP_STATUS_UPDATE);

	password_set_usr_name(CURRENT_PASSWORD_ID, tempStr, KEY);

	backup_free();

	progress_end();

	ENABLE_SLEEP();
}


void opt_callback_change_name(void)
{
	char name[32];
	password_read_name(CURRENT_PASSWORD_ID, (uint8_t*)name);
	if(!type_string(name, 32))
	{
		// If nothing changed
		return;
	}

	password_set_name(CURRENT_PASSWORD_ID, (uint8_t*)name);
	// Get the list sorted
	pwd_list_sort_alpha();
}


void opt_callback_delete_pwd(void)
{
	if(!draw_confirmation_screen(str_option_delete_index, str_option_delete_centerX))
	{
		return;
	}

	p_addr prevPwd = 0;
	if(NUM_PWD > 1)
	{
		prevPwd = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
	}
	
	pwd_list_delete_pwd(CURRENT_PASSWORD_ID);

	CURRENT_PASSWORD_ID = prevPwd;
}

void opt_callback_send_pwd(void)
{
	uint8_t pwd_data [32];

	password_read_data(CURRENT_PASSWORD_ID, pwd_data, KEY);
	keyboard_send((char*)pwd_data, strlen_bound((char*)pwd_data, 32));
	password_increment_counter(CURRENT_PASSWORD_ID);
	pwd_list_sort_use();
}

void opt_callback_send_usr(void)
{
	uint8_t pwd_data [64];

	password_read_usr_name(CURRENT_PASSWORD_ID, pwd_data, KEY);
	keyboard_send((char*)pwd_data, strlen_bound((char*)pwd_data, 64));
}

void opt_callback_send_both(void)
{
	opt_callback_send_usr();
	keyboard_send_key_next();
	opt_callback_send_pwd();
}
