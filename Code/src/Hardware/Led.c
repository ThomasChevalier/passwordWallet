#include "Led.h"

#include <avr/io.h>

#include "../Globals.h"
#include "PinDefinition.h"

void led_blink(uint8_t times)
{
	times *= 2;
	do
	{
		LED_PIN |= (1<<LED_PIN_NUM);
		delay_ms_f(LED_BLINK_DELAY);
	}while(--times);
}
