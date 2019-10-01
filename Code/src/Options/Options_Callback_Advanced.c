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
#include "../Hardware/Oled.h"

#include "../FSM/Events.h"
#include "../Program/Program.h"

#include "../System/System.h"
#include "../System/Sleep.h"

#include "../Graphics/font.h" // For font width

void opt_callback_show_key(void)
{
	// Display key
	user_display_key();

	DISABLE_SLEEP();
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
	if(!draw_confirmation_screen(str_recovery_full_index, str_recovery_full_centerX))
	{
		return;
	}

	DISABLE_SLEEP();

	// Erase all fram memory
	draw_clear();
	draw_flash_str_cx(40, str_recovery_eraseMem);
	draw_update();

	progress_begin(FRAM_BYTE_SIZE/FRAM_SIZE_DIVIDER);

	// Do .. While loop is 6 bytes smaller than for loop in this case
	uint8_t i = FRAM_BYTE_SIZE/FRAM_SIZE_DIVIDER;
	do
	{
		--i;
		#if FRAM_SIZE_DIVIDER == 256
		fram_set(i*FRAM_SIZE_DIVIDER, 0, 128);
		fram_set(i*FRAM_SIZE_DIVIDER+128, 0, 128);
		#elif FRAM_SIZE_DIVIDER == 1024
		// Compiler emit Warning: iteration 64 invokes undefined behavior [-Waggressive-loop-optimizations]
		// This is safe because the loop does only 8 iterations (1024/128 = 8)
		for(uint8_t j = 0; j < FRAM_SIZE_DIVIDER/128; ++j)
		{
			fram_set(i*FRAM_SIZE_DIVIDER+j*128, 0, 128);
		}
		#else
		#error Unsupported fram size
		#endif

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

		user_update_validation();

		backup_free();
	}

	EXIT:
	rfid_power_down();
}

void opt_callback_force_enter(void)
{
	opt_callback_enter_key();
	// Possible bug here were backup is set but there is no card
	user_update_validation();
	backup_free();
}

#ifdef VCC_GRAPH_ENABLE
static uint8_t for_graph(uint16_t value)
{
	if(value > 3400){
		return 15;
	}
	else if(value < 3100){
		return 0;
	}
	return (value - 3100)/20;
}

#define graph_x (80)
#define graph_y (32)
#define graph_len (30)

#endif

void opt_callback_system_info(void)
{
	DISABLE_SLEEP();

	program_wait();

#ifdef VCC_GRAPH_ENABLE
	uint8_t graph_pos = 0;

	uint8_t graph[graph_len] = {0};
#endif

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
		draw_num(str_system_pwd_pixLen + 4*FONT_WIDTH, 20, NUM_PWD*100/MAXIMUM_NUMBER_OF_PWD);
		draw_char(str_system_pwd_pixLen + 8*FONT_WIDTH, 20, '%');

		uint16_t volt = system_read_vcc();

#ifdef VCC_GRAPH_ENABLE
		if(graph_pos == graph_len)
		{
			for(uint8_t i = 0; i < graph_len-1; ++i)
			{
				graph[i] = graph[i+1];
			}
			graph[graph_len-1] = for_graph(volt);
		}
		else
		{
			graph[graph_pos]=for_graph(volt);
			graph_pos++;
		}
		for(uint8_t i = 0; i < graph_pos; ++i)
		{
			oled_draw_pixel(graph_x + i, graph_y - graph[i]+7, WHITE);
		}
#endif

		draw_flash_str(0, 30, str_system_volt);
		draw_num(str_system_volt_pixLen + 3, 30, volt);

		draw_flash_str(0, 40, str_system_entropy);
		draw_num(str_system_entropy_pixLen + 3, 40, entropy_pool_size);
		
		draw_flash_str(0, 50, str_system_version);
		
		draw_update();

		program_wait();
	}

	ENABLE_SLEEP();
}
