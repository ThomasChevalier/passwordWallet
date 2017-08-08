#include "Globals.h"

#include <avr/io.h>
#include <util/delay.h>

#include "Hardware/Fram.h"

uint8_t KEY[KEY_SIZE];
uint8_t NUM_PWD = 0;

void update_opt_flags(uint8_t opt_flag)
{
	OPTIONS_FLAG = opt_flag;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}