#include "Sleep.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "../Globals.h"

#include "../Hardware/Oled.h"
#include "../Hardware/Rfid.h"
#include "../Hardware/Buttons.h"
#include "../Hardware/PinDefinition.h"
#include "../Hardware/Fram.h"

#include "../Security/Random.h"

#include "../USB/USB.h"

void sleep_device(void)
{
	uint8_t sreg = SREG;
	cli();

	uint8_t rfid_was_init = FALSE;

	// Disable oled
	oled_active_display(FALSE);

	// Disable rfid
	if(!rfid_is_power_down())
	{
		rfid_power_down();
		rfid_was_init = TRUE;
	}

	// Disable led
	uint8_t old_led_port = LED_PORT;
	LED_PORT &= ~(1<<LED_PIN_NUM);

	// Disable USB
	USB_Disable();

	// Disable fram
	fram_sleep();

	// Turn off Watchdog
	wdt_reset();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1<<WDRF);
	/* Write logical one to WDCE and WDE */
	/* Keep old prescaler setting to prevent
	unintentional time-out */
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;

	// Power reduction register
	uint8_t oldPRR0 = PRR0;
	uint8_t oldPPR1 = PRR1;
	power_all_disable();

	// PCINT should be enabled

	set_sleep_mode (SLEEP_MODE_PWR_DOWN);

	sleep_enable();

	// Timed sequence
	sei();
	sleep_cpu();

	// ZzZzz ...

	sleep_disable();

	// Restore old power reduction register
	PRR0 = oldPRR0;
	PRR1 = oldPPR1;

	oled_active_display(TRUE);

	if(rfid_was_init)
	{
		rfid_init();
	}

	// Active led
	LED_PORT = old_led_port;

	USB_init();
	random_init();
	fram_wakeup();

	ACTIVITY_TIMER = 0;

	SREG = sreg;

	
	while(buttons_pressed());
}

// Nothing, wake up the device
//EMPTY_INTERRUPT(PCINT0_vect);
ISR(PCINT0_vect)
{
	buttons_debounce();
}

void sleep_idle(void)
{
	uint8_t sreg = SREG;
	set_sleep_mode (SLEEP_MODE_IDLE);

	sleep_enable();

	// Timed sequence
	sei();
	sleep_cpu();

	// ZzZzz ...

	sleep_disable();
	SREG = sreg;
}