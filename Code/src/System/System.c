#include "System.h"

#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()
#include <avr/wdt.h>
#include <avr/power.h>

#include "../Graphics/Drawing.h"
#include "../Graphics/String.h"

#include "../Program/Program.h"
#include "../FSM/Events.h"

#include "Timer.h"

#include "../Hardware/PinDefinition.h"

void system_init(void)
{
	// Activate interrupt
	sei();
	
	timer_init();

	// Turn on pin change interrupt
	PCICR = (1<<PCIE0); // Pin change interrupt enable
	// Set pin mask
	PCMSK0 = (1 << BUTTON_1_PIN_CHANGE_NUM) |
	         (1 << BUTTON_2_PIN_CHANGE_NUM) |
	         (1 << BUTTON_3_PIN_CHANGE_NUM) |
	         (1 << BUTTON_4_PIN_CHANGE_NUM);

	// If the i2c module is not needed
	#if !defined(I2C_FRAM)
	PRR0 |= (1<<PRTWI) | (1<<PRADC); // Disable i2c and adc
	#else
	PRR0 |= (1<<PRADC); // Make sure the adc is disabled first. Normally the adc is disabled at startup
	#endif
	PRR1 |= (1<<PRUSART1); // Disable usart
}

// Read the reset source and display it
void system_read_reset_source(uint8_t mcusr)
{
	uint16_t str_index = 0;
	// Ignore power on reset
	if((mcusr & (1<<PORF )) || (mcusr == 0)) {return;}

	if(mcusr & (1<<EXTRF)) {str_index = str_reset_external_index;}
	if(mcusr & (1<<BORF )) {str_index = str_reset_brownout_index;}
	if(mcusr & (1<<WDRF )) {str_index = str_reset_watchdog_index;}
	if(mcusr & (1<<JTRF )) {str_index = str_reset_jtag_index;}

	draw_flash_string(0, 55, str_index);
	draw_update();
}

uint16_t system_read_vcc(void)
{

	// Enable ADC
	PRR0 &= ~(1<<PRADC);
	ADCSRA = (1<<ADEN);

	delay_ms_f(2); // Wait for it

	// Read 1.1V reference against AVcc
	// set the reference to Vcc and the measurement to the internal 1.1V reference
	ADMUX = (1<<REFS0) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1);

	delay_ms_f(2); // Wait for Vref to settle

	ADCSRA |= (1<<ADSC); // Start conversion
	while (ADCSRA & (1<<ADSC)); // measuring

	uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
	uint8_t high = ADCH; // unlocks both

	uint16_t result = (high<<8) | low;

	result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000

	// Disable ADC
	ADMUX = 0;
	ADCSRA = 0;
	PRR0 |= (1<<PRADC);

	return result; // Vcc in millivolts
}


// These pragma directive are here to tell gcc to ignore the warning
// for this function only.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-local-addr"

uint16_t system_free_ram (void)
{
	extern uint16_t __heap_start, *__brkval;
	uint16_t v;
	return (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
}

#pragma GCC diagnostic pop

// void (*boot)(void)=(void*)0x7000;

// See http://www.fourwalledcubicle.com/files/LUFA/Doc/120730/html/_page__software_bootloader_start.html#Sec_SoftareBootAVR8
void system_reset(void)
{
	// #error Call the bootloader
	// jump to bootloader section
	
	// Disable interrupt
	cli();
	// Change watchdog config to reset mode

	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR  = (1<<WDE); // Reset mode and 16 ms timeout

	// Reset
	while(1);
	
    // boot();
	// asm volatile ("jmp 0x7000"); // don't work
	// asm volatile ("ijmp" ::"z" (0x3800)); // don't work
}
