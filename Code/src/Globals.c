#include "Globals.h"

#include <avr/io.h>
#include <util/delay.h>

#include "Hardware/Fram.h"

volatile uint8_t RUNNING = 1;
uint8_t GLOBALS_EVENTS = 0;

uint8_t KEY[KEY_SIZE];
uint8_t CURRENT_PASSWORD_ID = 0;

uint8_t OPTIONS_FLAG = 0;
uint8_t NUM_PWD = 0;

void update_opt_flags(uint8_t opt_flag)
{
	OPTIONS_FLAG = opt_flag;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}