#include "Timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()

#include "../Globals.h"
#include "../FSM/Events.h"

void timer_init(void)
{
	TCCR1B = 0;
	// set timer 1 prescale factor to 64
	TCCR1B |= (1<<CS11) | (1<<CS10);

	// Active millisecond timer
	TCCR0A = (1<<WGM01); // CTC mode
	TCCR0B = (1<<CS01) | (1<<CS00); // clk/64 prescaler
	OCR0A = 125;
	TIMSK0 = (1<<OCIE0A); // Enable interrupt
	TCNT0 = 0;
}

/**
 * @brief This interrupt is called every millisecond
 */
ISR (TIMER0_COMPA_vect)
{
	// Prevent overflow
	if(SERIAL_TIMEOUT_TIMER != SERIAL_TIMEOUT){
		++SERIAL_TIMEOUT_TIMER;
	}
	

	if(!(GLOBALS_EVENTS & EVENT_NO_SLEEP))
	{
		++ACTIVITY_TIMER;
	}
	if(ACTIVITY_TIMER == SHUTDOWN_DELAY)
	{
		GLOBALS_EVENTS |= EVENT_SLEEP_SHUTDOWN;
	}
}
