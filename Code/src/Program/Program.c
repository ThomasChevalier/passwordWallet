#include "Program.h"

#include <util/delay.h>

#include "../Globals.h"

#include "../Graphics/Drawing.h"

#include "../Security/Random.h"

#include "../Hardware/Fram.h"
#include "../Hardware/Keyboard.h"
#include "../Hardware/Buttons.h"

#include "../FSM/Events.h"

static uint8_t first_press;

void program_init(void)
{
	first_press = 1;

	draw_update();

	// Read the flags and data from fram
	OPTIONS_FLAG = fram_read_byte(OFFSET_OPTIONS_FLAG);
	NUM_PWD = fram_read_byte(OFFSET_NUM_PWD);

	// Delete all entropy pool, because it cannot be trusted (someone may have corrupt the data)
	uint16_t entropyPoolSize = 0;
	fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);

	// Read the memory map
	fram_read_bytes(OFFSET_MEMORY_MAP, MEMORY_MAP, SIZE_MEMORY_MAP);
}

void program_update(void)
{
	keyboard_loop();
	random_save_entropy();

	first_press = buttons_update_event();
	
}

void program_wait(void)
{
	if(first_press)
	{
		for(uint8_t i = 0; i < 150; ++i)
		{
			_delay_ms(1);
			if(!RUNNING)
			{
				break;
			}
		}
	}
	else
	{
		for(uint8_t i = 0; i < 40; ++i)
		{
			_delay_ms(1);
			if(!RUNNING)
			{
				break;
			}
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