#include "Globals.h"

#include <avr/io.h>
#include <util/delay.h>
#include "Hardware/Led.h"

#include "Hardware/Fram.h"

uint8_t KEY[KEY_SIZE];
p_addr NUM_PWD = 0;

p_addr CURRENT_PASSWORD_ID = 0;
volatile uint16_t ACTIVITY_TIMER = 0;

volatile uint8_t SERIAL_TIMEOUT_TIMER = 0;

void update_opt_flags(uint8_t opt_flag)
{
	OPTIONS_FLAG = opt_flag;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

uint8_t strlen_bound(char* str, uint8_t max)
{
	uint8_t len = 0;
	while(*str++ && len < max) ++len;
	return len; 
}

void delay_ms_f(uint16_t ms)
{	
	for(uint16_t i = 0; i < ms; ++i){
		_delay_ms(1);
	}
}