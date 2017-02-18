#include <util/delay.h>  // for _delay_ms()

#include "Globals.h"

#include "System/System.h"
#include "Hardware/Hardware.h"
#include "Security/Security.h"

#include "Graphics/Drawing.h"

#include "Security/Authentification.h"
#include "Security/Random.h"

#include "Hardware/Keyboard.h"
#include "Hardware/Buttons.h"
#include "Hardware/Rfid.h"
#include "Hardware/Fram.h"

#include "FSM/States.h"
#include "FSM/Transitions.h"
#include "FSM/Events.h"

void init_program(void)
{
	draw_update();

	// Read the flags and data from fram
	OPTIONS_FLAG = fram_read_byte(OFFSET_OPTIONS_FLAG);
	FIRST_PWD_UTIL = fram_read_byte(OFFSET_FIRST_PWD_UTIL);;
	FIRST_PWD_ALPHA = fram_read_byte(OFFSET_FIRST_PWD_ALPHA);
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
	init_system();
	init_hardware();
	init_security();

	init_program();

	// If the device has not been initialized yet
	const uint8_t optFlag = fram_read_byte(OFFSET_OPTIONS_FLAG);
	if((optFlag & (1<<OPTIONS_FLAG_OFFSET_INITIALIZED)) == 0)
	{
		do_full_reset();
	}
	
	State states[NUM_STATES];
	states[STATE_WAIT_CARD] =	(State)
	{
		EVENT_ALL_BUTTON | EVENT_RFID_TAG_PRESENT, 	state_wait_card_transition, state_wait_card_begin, state_wait_card_end
	};
	states[STATE_RECOVERY]  =		(State)
	{
		EVENT_ALL_BUTTON, state_recovery_transition, state_recovery_begin, state_recovery_end
	};
	states[STATE_MAIN]  =		(State)
	{
		EVENT_ALL_BUTTON, state_main_transition, state_main_begin, state_main_end
	};
	states[STATE_BROWSE] =		(State)
	{
		EVENT_ALL_BUTTON, state_browse_transition, state_browse_begin, state_browse_end
	};
	states[STATE_OPTION]=		(State)
	{
		EVENT_ALL_BUTTON, state_options_transition, state_options_begin, state_options_end
	};

	State* currentState = &states[STATE_WAIT_CARD];
	uint8_t currentStateNum = (uint8_t)STATE_WAIT_CARD;

	currentState->begin();

	

	while(RUNNING)
	{
		// Check rfid event only when we are waiting card (and so we are sure that the rfid reader is on, because the state WAIT_CARD initialize it)
		if(currentStateNum == (uint8_t)STATE_WAIT_CARD)
		{
			if(rfid_PICC_IsNewCardPresent() && rfid_PICC_ReadCardSerial())
			{
				events_happen(EVENT_RFID_TAG_PRESENT);
			}
		}

		keyboard_loop();
		random_save_entropy();

		buttons_update_event();
		uint8_t event = events_get(); // Mask of events

		if(currentState->event_mask & event)
		{
			const uint8_t newState = currentState->transition(event);
			if(currentStateNum != newState)
			{
				currentState->end();
				currentState = &states[newState];
				currentStateNum = newState;
				currentState->begin();
			}
			
		}

		if(FIRST_PRESS)
		{
			_delay_ms(150);
		}
		else
		{
			_delay_ms(50);
			
		}
	}
}