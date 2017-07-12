#include "System.h"

#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"

#include "../Program/Program.h"
#include "../FSM/Events.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


void system_init(void)
{
	// this needs to be called before setup() or some functions won't
	// work there
	sei();

	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);

	// set timer 0 prescale factor to 64

	// Thomas Chevalier : this combination freeze the controller
	//#elif defined(TCCR0B) && defined(CS01) && defined(CS00)
	// this combination is for the standard 168/328/1280/2560
	// sbi(TCCR0B, CS01);
	// sbi(TCCR0B, CS00);

	// this combination is for the __AVR_ATmega645__ series
	sbi(TCCR0A, CS01);
	sbi(TCCR0A, CS00);

	// enable timer 0 overflow interrupt
	sbi(TIMSK0, TOIE0);


	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

	TCCR1B = 0;

	// set timer 1 prescale factor to 64
	sbi(TCCR1B, CS11);

	sbi(TCCR1B, CS10);

	// put timer 1 in 8-bit phase correct pwm mode
	sbi(TCCR1A, WGM10);


	sbi(TCCR3B, CS31);    // set timer 3 prescale factor to 64
	sbi(TCCR3B, CS30);
	sbi(TCCR3A, WGM30);   // put timer 3 in 8-bit phase correct pwm mode

	/* beginning of timer4 block for 32U4 and similar */
	sbi(TCCR4B, CS42);    // set timer4 prescale factor to 64
	sbi(TCCR4B, CS41);
	sbi(TCCR4B, CS40);
	sbi(TCCR4D, WGM40);   // put timer 4 in phase- and frequency-correct PWM mode 
	sbi(TCCR4A, PWM4A);   // enable PWM mode for comparator OCR4A
	sbi(TCCR4C, PWM4D);   // enable PWM mode for comparator OCR4D



#if defined(ADCSRA)
	// set a2d prescaler so we are inside the desired 50-200 KHz range.
	#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
		cbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#else // 128 kHz / 2 = 64 KHz -> This is the closest you can get, the prescaler is 2
		cbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#endif
	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#endif

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
