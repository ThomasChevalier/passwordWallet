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

	// Check the reset reason and display it
	system_read_reset_source(mcusr);

	// Check the device
	if(!self_test_check())
	{
		// The device does not behave correctly, freeze it
		while(1){}
	}

	// Read and initialize various variable
	program_init();

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
		{state_communication_transition,    state_communication_begin,    state_communication_end}     // STATE_COMMUNICATION
	};

	State const * currentState = &states[STATE_WAIT_CARD];
	uint8_t currentStateNum = STATE_WAIT_CARD;

	currentState->begin();

	while(!(GLOBALS_EVENTS & EVENT_STOP))
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
		// Don't wait in communication state, it slow down the transmission
		if(currentStateNum != STATE_COMMUNICATION)
		{
			program_wait();
		}
	}

	while(1){}
}
