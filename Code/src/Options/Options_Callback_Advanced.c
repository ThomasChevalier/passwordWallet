#include "Options_Callback_Advanced.h"

#include <string.h>

#include "../Globals.h"

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"
#include "../Graphics/Ascii85.h"
#include "../Graphics/ProgressBar.h"

#include "../Security/Security.h"
#include "../Security/Random.h"
#include "../Security/Backup.h"
#include "../Security/Authentification.h"

#include "../Hardware/Rfid.h"
#include "../Hardware/Fram.h"
#include "../Hardware/Buttons.h"

#include "../FSM/Events.h"
#include "../Program/Program.h"

#include "../System/System.h"
#include "../System/Sleep.h"

void opt_callback_show_key(void)
{
	// Display key
	user_display_key();

	DISABLE_SLEEP();
	while(buttons_pressed());
	program_pause_until_event(EVENT_ALL_BUTTONS);
	ENABLE_SLEEP();
}

void opt_callback_inverse_screen(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_ORIENTATION));
}

void opt_callback_change_keyboard(void)
{
	update_opt_flags(OPTIONS_FLAG ^ (1<<OPTIONS_FLAG_OFFSET_QWERTY));
}

void opt_callback_full_reset(void)
{
	DISABLE_SLEEP();

	// Erase all fram memory
	draw_clear();
	draw_flash_str_cx(40, str_recovery_eraseMem);
	draw_update();

	progress_begin(FRAM_BYTE_SIZE/256);

	// Do .. While loop is 6 bytes smaller than for loop in this case
	uint8_t i = FRAM_BYTE_SIZE/256;
	do
	{
		--i;
		fram_set(i*256, 0, 128);
		fram_set(i*256+128, 0, 128);
		progress_add(1);
	}while(i);
	progress_end();

	// Clear variables
	CURRENT_PASSWORD_ID = GLOBALS_EVENTS = NUM_PWD = OPTIONS_FLAG = 0;
	random_reset();

	// change_master_key will enable sleep when it return
	// Get a master key
	user_change_key();

	// Device is now initialized !
	update_opt_flags((1<<OPTIONS_FLAG_OFFSET_INITIALIZED));
}

void opt_callback_enter_key(void)
{
	char usrKey[21] = {0};
	type_string(usrKey, 20);
	decode_16B(usrKey, KEY);
}

void opt_callback_force_card(void)
{
	rfid_init();
	user_wait_card();
	if(user_authenticate_card() != RETURN_SUCCESS)
	{
		goto EXIT;
	}

	uint8_t buffer[18];

	// Trying to read master key ...
	if(user_read_key_from_card(buffer, MIFARE_BLOCK_KEY) == RETURN_SUCCESS)
	{
		// .. Success
		memcpy(KEY, buffer, 16);
		security_erase_data(buffer, 18);

		user_update_validation();

		backup_free();
	}

	EXIT:
	rfid_power_down();
}

void opt_callback_force_enter(void)
{
	opt_callback_enter_key();
	user_update_validation();
	backup_free();
}

void opt_callback_system_info(void)
{
	DISABLE_SLEEP();

	program_wait();


	const uint16_t ram = 2560 - system_free_ram();
	
	while(!(events_get() & EVENT_ALL_BUTTONS))
	{
		program_update();

		draw_clear();
		draw_flash_str_cx(0, str_system_info);

		draw_flash_str(0, 10, str_system_ram);
		draw_num(str_system_ram_pixLen + 3, 10, ram);
		draw_num(str_system_ram_pixLen + 30 + 3, 10, ram*10/256);
		draw_char(str_system_ram_pixLen + 30 + 15 + 3, 10, '%');

		draw_flash_str(0, 20, str_system_pwd);
		draw_num(str_system_pwd_pixLen + 3, 20, NUM_PWD);
		//draw_text(str_system_pwd_pixLen + 20 + 3, 20, "/ " STRINGFY(MAXIMUM_NUMBER_OF_PWD), 7);

		draw_flash_str(0, 30, str_system_volt);
		draw_num(str_system_volt_pixLen + 3, 30, system_read_vcc());

		draw_flash_str(0, 40, str_system_entropy);
		draw_num(str_system_entropy_pixLen + 3, 40, entropy_pool_size);
		draw_update();

		program_wait();
	}

	ENABLE_SLEEP();
}
