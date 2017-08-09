#include "System.h"

#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"

#include "../Program/Program.h"
#include "../FSM/Events.h"

#include "Timer.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


void system_init(void)
{
	// Activate interrupt
	sei();
	
	timer_init();
}

// Read the reset source and display it
void system_read_reset_source(uint8_t mcusr)
{
	uint8_t str_index = 0;

	// Ignore power on reset
	if((mcusr & (1<<PORF )) || (mcusr == 0)) {return;}

	if(mcusr & (1<<EXTRF)) {str_index = str_reset_external_index;}
	if(mcusr & (1<<BORF )) {str_index = str_reset_brownout_index;}
	if(mcusr & (1<<WDRF )) {str_index = str_reset_watchdog_index;}
	if(mcusr & (1<<JTRF )) {str_index = str_reset_jtag_index;}

	draw_clear();
	draw_text_index(0, 0, str_index);
	draw_update();

	// Wait for anything
	program_pause_until_event(EVENT_WHENEVER);
}
