#include <util/delay.h>  // for _delay_ms()

#include "Globals.h"

#include "System/System.h"
#include "Hardware/Hardware.h"
#include "Security/Security.h"

#include "Graphics/Drawing.h"

#include "Security/Random.h"
#include "Security/Encryption.h"
#include "Security/Password_List.h"

#include "Hardware/Keyboard.h"
#include "Hardware/Buttons.h"
#include "Hardware/Fram.h"
#include "Hardware/SelfTest.h"

#include "FSM/States.h"
#include "FSM/Events.h"

void program_init(void)
{
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

int main(void)
{
	// Initialization
	system_init();
	hardware_init();
	security_init();

	// Check the device
	if(!self_test_check())
	{
		// The device does not behave correctly, freeze it
		draw_clear();
		draw_update();
		while(1);
	}

	program_init();

	// If the device has not been initialized yet
	const uint8_t optFlag = fram_read_byte(OFFSET_OPTIONS_FLAG);
	if((optFlag & (1<<OPTIONS_FLAG_OFFSET_INITIALIZED)) == 0)
	{
		state_recovery_do_full_reset();
	}
	
	const State states[NUM_STATES] = 
	{
		{state_wait_card_transition,		state_wait_card_begin, 			state_wait_card_end}, 			// STATE_WAIT_CARD
		{state_recovery_transition,			state_recovery_begin, 			state_recovery_end},			// STATE_RECOVERY
		{state_main_transition,				state_main_begin, 				state_main_end},				// STATE_MAIN
		{state_browse_transition,			state_browse_begin, 			state_browse_end},				// STATE_BROWSE
		{state_options_transition,			state_options_begin, 			state_options_end},				// STATE_OPTION
		{state_option_password_transition,	state_option_password_begin, 	state_option_password_end},		// STATE_OPTION_PASSWORD
		{state_option_sort_transition,		state_option_sort_begin, 		state_option_sort_end},			// STATE_OPTION_SORT
		{state_option_advanced_transition,	state_option_advanced_begin, 	state_option_advanced_end}		// STATE_OPTION_ADVANCED 
	};

	State const * currentState = &states[STATE_WAIT_CARD];
	uint8_t currentStateNum = STATE_WAIT_CARD;

	// If the device is not encrypted, skip WAIT_CARD state
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_NO_ENCRYPTION))
	{
		currentState = &states[STATE_MAIN];
		currentStateNum = STATE_MAIN;
		encryption_copy_key_from_eeprom();
	}

	currentState->begin();

	while(RUNNING)
	{
		keyboard_loop();
		random_save_entropy();

		buttons_update_event();
		uint8_t event = events_get(); // Mask of events

		const uint8_t newState = currentState->transition(event);
		if(currentStateNum != newState)
		{
			currentState->end();
			currentState = &states[newState];
			currentStateNum = newState;
			currentState->begin();
		}

		if(FIRST_PRESS)
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
			for(uint8_t i = 0; i < 50; ++i)
			{
				_delay_ms(1);
				if(!RUNNING)
				{
					break;
				}
			}
			
		}
	}
	security_erase_data(KEY, 16);
}
