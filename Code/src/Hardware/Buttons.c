#include "Buttons.h"

#include <avr/io.h>

#include "../Globals.h"
#include "../FSM/Events.h"
#include "PinDefinition.h"

uint8_t buttons_update_event(void)
{
	static uint8_t last_butts = 0;

	uint8_t butts = buttons_pressed();
	if(butts)
	{
		events_happen(butts);
	}
	if(butts != last_butts)
	{
		last_butts = butts;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t buttons_pressed(void)
{
	uint8_t rep = 0;
	// Check the 4 buttons and write corresponding byte
	// Note : there is input pullup so BUTTON_1_PIN & (1<<BUTTON_1_PIN_NUM) != 0 <=> button released
	if (!(BUTTON_1_PIN & (1<<BUTTON_1_PIN_NUM)))
	{
		rep |= EVENT_BUTTON_1;
	}
	if (!(BUTTON_2_PIN & (1<<BUTTON_2_PIN_NUM)))
	{
		rep |= EVENT_BUTTON_2;
	}
	if (!(BUTTON_3_PIN & (1<<BUTTON_3_PIN_NUM)))
	{
		rep |= EVENT_BUTTON_3;
	}
	if (!(BUTTON_4_PIN & (1<<BUTTON_4_PIN_NUM)))
	{
		rep |= EVENT_BUTTON_4;
	}

	// If screen is inversed, perfom a rotation to left of 2 bit on the low nibble
	// /!\ EVENT_BUTTON_1 to EVENT_BUTTON_4 must be in range 0x01 to 0x0F
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION))
	{
		rep <<= 2;
		rep = (rep & 0x0F) | (rep >> 4);
	}
	return rep;
}
