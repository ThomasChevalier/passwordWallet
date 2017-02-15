#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  // for sei()
#include <util/delay.h>  // for _delay_ms()
#include <string.h>

#include "Spi.h"
#include "Oled.h"
#include "Rfid.h"
#include "Fram.h"
#include "Keyboard.h"
#include "Buttons.h"

#include "States.h"
#include "Transitions.h"
#include "Events.h"

#include "Globals.h"
#include "Authentification.h"

#include "Random.h"

#include "UserSetup.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void init_system()
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


void init_hardware()
{
	spi_setup_hardware();
	fram_setup_hardware();
	oled_setup_hardware();
	rfid_setup_hardware();
	buttons_setup_hardware();
}

void init_software()
{
	oled_init();
	rfid_init();
	random_init();
  //keyboard_init();
}

void blink(uint8_t n)
{
  DDRD |= (1<<5);
  uint8_t i = 0;
  for(;i < n*2; ++i)
  {
    PORTD ^= (1<<5);
    _delay_ms(1000);
  }
}

int main()
{
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

  	// Init of the µC
	init_system();
	init_hardware();
	init_software();
	
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
		random_save_entropy();

		buttons_update_event();
		uint8_t event = getEvents(); // Mask of events

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