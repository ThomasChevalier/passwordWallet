#include "Program.h"

#include "../Globals.h"

#include "../Graphics/Drawing.h"

#include "../Security/Random.h"
#include "../Security/Password.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Buttons.h"
#include "../Hardware/Oled.h"

#include "../USB/USB.h"

#include "../FSM/Events.h"

#include "../System/Timer.h"
#include "../System/Sleep.h"

void program_init(void)
{
	// Read the flags and data from fram
	OPTIONS_FLAG = fram_read_byte(OFFSET_OPTIONS_FLAG);
	NUM_PWD = password_read_id(OFFSET_NUM_PWD);
	
	draw_update();
}

void program_update(void)
{
	USB_loop();
	random_save_entropy();
	buttons_update_event();

	if(GLOBALS_EVENTS & EVENT_SLEEP_SHUTDOWN)
	{
		GLOBALS_EVENTS &= ~(EVENT_SLEEP_SHUTDOWN);
		sleep_device();
	}
}

void program_wait(void)
{
	const uint8_t old_button_state = buttons_pressed();
	uint8_t i = 0;
	for(; i < WAIT_DELAY; ++i){
		sleep_idle();
		if(old_button_state != buttons_pressed()) // The user has pressed a button
		{
			break;
		}
	}
}

void program_pause_until_event(uint8_t event)
{
	uint8_t gEvent = 0;
	do
	{
		program_update();
		gEvent = events_get();
	}while((gEvent & event) == 0);
}
