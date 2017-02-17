#include <util/delay.h>  // for _delay_ms()

#include "Globals.h"

#include "Init.h"
#include "UserSetup.h"
#include "Oled.h"
#include "Authentification.h"
#include "Keyboard.h"
#include "Random.h"
#include "Buttons.h"

#include "States.h"
#include "Transitions.h"
#include "Events.h"

int main(void)
{
	// Init of the µC
	init_system();
	init_hardware();
	init_software();
	
	State states[NUM_STATES];
	states[STATE_INIT]  =   (State)
	{
		EVENT_PASSWORD_ENTERED,                    TRANSITION(STATE_INIT)
	};
	states[STATE_MAIN]  =   (State)
	{
		EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_MAIN)
	};
	states[STATE_BROWSE] = (State)
	{
		EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_BROWSE)
	};
	states[STATE_OPTION]=   (State)
	{
		EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_OPTION)
	};
	states[STATE_SAVE]  =   (State)
	{
		EVENT_USB_CONNECTED,                       TRANSITION(STATE_SAVE)
	};

	State* currentState = &states[STATE_INIT];
	
	if(!usr_setup_is_initialized() || 1)
	{
    	// Device is not initialized ...
		if(!usr_setup_do_initialization())
		{
      		// Device has not been initialized
			oled_clear_display();
			oled_display();
			wait_for_valid_card();
		}
	}
	else if(usr_setup_check_card_lost())
	{
		// User has lost his card
		if(!usr_setup_recover_key())
		{
			// And typed a wrong key
			oled_clear_display();
			oled_display();
			wait_for_valid_card();
		}
	}
	else
	{
    #ifdef FRAM_BUFFER
		oled_clear_display();
    #endif

		oled_display();
    	// Wait until there is a rfid card, with the good password
		wait_for_valid_card();
	}

	while(RUNNING)
	{
		keyboard_loop();
		random_save_entropy();

		buttons_update_event();
		uint8_t event = events_get(); // Mask of events

		if(currentState->event_mask & event)
		{
			uint8_t newState = currentState->transition(event);
			currentState = &states[newState];
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