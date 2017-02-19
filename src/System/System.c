#include "System.h"

#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


void init_system(void)
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



	// set a2d prescaler so we are inside the desired 50-200 KHz range.

	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	cbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);

}
