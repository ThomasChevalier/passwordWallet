#include "Program.h"

#include <util/delay.h>

#include "../Globals.h"

#include "../Graphics/Drawing.h"

#include "../Security/Random.h"
#include "../Security/Password_List.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Buttons.h"
#include "../Hardware/Oled.h"

#include "../USB/USB.h"

#include "../FSM/Events.h"

#include "../System/Timer.h"
#include "../System/Sleep.h"


static uint8_t first_press;

void program_init(void)
{
	first_press = 1;

	// Read the flags and data from fram
	OPTIONS_FLAG = fram_read_byte(OFFSET_OPTIONS_FLAG);
	NUM_PWD = pwd_read_id(OFFSET_NUM_PWD);
	
	draw_update();
}

void program_update(void)
{
	USB_loop();
	random_save_entropy();

	first_press = buttons_update_event();

	if(GLOBALS_EVENTS & EVENT_SLEEP_SHUTDOWN)
	{
		GLOBALS_EVENTS &= ~(EVENT_SLEEP_SHUTDOWN);
		sleep_device();
	}
}

static void idle_delay(uint8_t num)
{
	const uint8_t old_button_state = buttons_pressed();
	uint8_t i = 0;
	for(; i < num; ++i){
		sleep_idle();
		if(old_button_state != buttons_pressed()) // The user has pressed a button
		{
			break;
		}
	}
}

void program_wait(void)
{
	if(first_press)
	{
		program_small_wait();
	}	
	else
	{
		// Wait 100 ms
		idle_delay(100);
	}
}

void program_small_wait(void)
{
	if(first_press ){
		// Wait 200 ms
		idle_delay(200);
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
