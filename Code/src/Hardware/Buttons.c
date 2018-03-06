#include "Buttons.h"

#include <avr/io.h>

#include "../Globals.h"
#include "../FSM/Events.h"
#include "PinDefinition.h"

void buttons_update_event(void)
{
	buttons_debounce();
}

/**
 * @brief Convert a raw button status to an event status according to screen orientation
 * 
 * @param raw Bit 0 to 3 are for buttons 1 to 4
 * @return The events that matche the raw status
 */
static uint8_t button_to_event(uint8_t raw)
{
	// If screen is inversed, perfom a rotation to left of 2 bit on the low nibble
	// /!\ EVENT_BUTTON_1 to EVENT_BUTTON_4 must be in range 0x01 to 0x0F
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION))
	{
		raw <<= 2;
		raw = (raw & 0x0F) | (raw >> 4);
	}
	return raw;
}


uint8_t buttons_pressed(void)
{
	return GLOBALS_EVENTS & 0x0F;
}

void buttons_debounce(void)
{
	static uint32_t buttons_lastChange[4]; //< Last time (in ms) when the button has been changed
	static uint8_t debounce_state = 0; //< Hold the status of the buttons during the debouncing process
	static uint8_t event_called = 0; //< Tell if the event has already been called for a button press

	uint8_t raws[4];
	raws[0] = ~(BUTTON_1_PIN >> BUTTON_1_PIN_NUM) & 0x01;
	raws[1] = ~(BUTTON_2_PIN >> BUTTON_2_PIN_NUM) & 0x01;
	raws[2] = ~(BUTTON_3_PIN >> BUTTON_3_PIN_NUM) & 0x01;
	raws[3] = ~(BUTTON_4_PIN >> BUTTON_4_PIN_NUM) & 0x01;

	for(uint8_t i = 0; i < 4; ++i)
	{
		const uint8_t curr_state = ((debounce_state >> i) & 0x01);

		// If the state of the button changed
		if(raws[i] ^ curr_state){
			// Then reset the timer
			buttons_lastChange[i] = MILLISECOND_TIMER;
			if(raws[i]){
				debounce_state |= (1<<i);
			}
			else{
				debounce_state &= ~(1<<i);
			}
		}

		// Determine what delay to use
		const uint32_t lastChange = buttons_lastChange[i];
		uint32_t delay = RELEASE_TIME_MS;
		if(raws[i]){
			delay = PRESS_TIME_MS;
		}

		// If the button reach the required delay
		if(MILLISECOND_TIMER - lastChange >= delay){
			// Then the button is in a stable state

			// If pressed
			if(raws[i]){

				// Signal that the button is pressed
				// Only called once
				if(!(event_called & (1<<i))){
					events_happen(button_to_event(1<<i));
					event_called |= (1<<i);
				}

				// If the button is pressed, there may be a long press
				if(MILLISECOND_TIMER - lastChange >= LONG_PRESS_TIME_MS){
					// Continuously signaling that the button is pressed
					events_happen(button_to_event(1<<i));
				}
			}
			// If released
			else{
				// Reset the event call flag
				event_called &= ~(1<<i);
				// Do not clear the event because the ui must react to all of the inputs
			}
		}
	}
}
