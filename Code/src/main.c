#include <avr/interrupt.h>

#include "Globals.h"

#include "System/System.h"
#include "Hardware/Hardware.h"
#include "Security/Security.h"
#include "Program/Program.h"

#include "Security/Encryption.h"
#include "Security/Backup.h"

#include "Hardware/Fram.h"
#include "Hardware/SelfTest.h"
#include "Hardware/Led.h"

#include "USB/USB.h"

#include "FSM/States.h"
#include "FSM/Events.h"

#include "Options/Options_Callback_Advanced.h"

/**
 * @brief Freeze the controller in case of a wrong interrupt
 */
ISR(BADISR_vect)
{
	for (;;)
	{
		led_blink(1);
	}
}

int main(void)
{
	// Read asap the reset register
	uint8_t mcusr = MCUSR;
	MCUSR = 0;

	// Initialization
	system_init();
	hardware_init();
	USB_init();
	security_init();

	// Check the device
	if(self_test_check() == RETURN_ERROR)
	{
		// The device does not behave correctly, freeze it
		while(1){}
	}

	// Read variables from fram and draw the screen
	program_init();

	// Check the reset reason and display it
	system_read_reset_source(mcusr);

	// If the device has not been initialized yet
	const uint8_t optFlag = fram_read_byte(OFFSET_OPTIONS_FLAG);
	if( (optFlag & (1 << OPTIONS_FLAG_OFFSET_INITIALIZED)) == 0)
	{
		opt_callback_full_reset();
	}

	// In case of something goes wrong at the last session,
	// recover broken data.
	backup_recover();

	static const State states[NUM_STATES] =
	{
		{state_wait_card_transition,        state_wait_card_begin,        state_wait_card_end},        // STATE_WAIT_CARD
		{state_main_transition,             state_main_begin,             state_main_end},             // STATE_MAIN
		{state_browse_transition,           state_browse_begin,           state_browse_end},           // STATE_BROWSE
	};

	uint8_t currentStateNum = STATE_WAIT_CARD;
	State const * currentState = &states[currentStateNum];

	currentState->begin();

	while(true)
	{
		program_update();
		uint8_t event = events_get();  // Mask of events

		const uint8_t newState = currentState->transition(event);
		if(currentStateNum != newState)
		{
			currentState->end();
			currentState = &states[newState];
			currentStateNum = newState;
			currentState->begin();
		}

		program_wait();
	}
}
