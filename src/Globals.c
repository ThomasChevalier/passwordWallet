#include "Globals.h"

#include <avr/io.h>
#include <util/delay.h>

uint8_t FIRST_PRESS = 1;

volatile uint8_t RUNNING = 1;
uint8_t GLOBALS_EVENTS = 0;

uint8_t KEY[16];
uint8_t CURRENT_PASSWORD_ID = 0;

uint8_t OPTIONS_FLAG = 0;
uint8_t NUM_PWD = 0;

uint8_t MEMORY_MAP[SIZE_MEMORY_MAP];

void blink(uint8_t times)
{
	DDRD |= (1<<5);
	uint8_t i = 0;
	for(;i < times*2; ++i)
	{ \
	  PORTD ^= (1<<5);
	    _delay_ms(500);
	}
}
