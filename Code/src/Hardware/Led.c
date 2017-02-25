#include "Led.h"

#include <util/delay.h>
#include <avr/io.h>

#include "../Globals.h"
#include "PinDefinition.h"

void led_setup_harware(void)
{
	LED_DDR |= (1<<LED_PIN_NUM);
}

void led_blink(uint8_t times)
{
	while(times--)
	{
		LED_PORT |= (1<<5);
		_delay_ms(LED_BLINK_DELAY);
		LED_PORT &= ~(1<<5);
		_delay_ms(LED_BLINK_DELAY);
	}
}