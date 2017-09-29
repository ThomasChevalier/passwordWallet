#include "Random.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "../Globals.h"
#include "../Hardware/Fram.h"

// Define save ram and prog mem
// Save 16 byte of progmem and 16 byte of ram
#define WDT_RETURN_BYTE (256)
#define WDT_RETURN_WORD (65536)
#define WDT_MAX_8INT (0xFF)
#define WDT_MAX_16INT (0xFFFF)
#define WDT_MAX_32INT (0xFFFFFFFF)

#define NUM_SAMPLE (32)
static uint8_t sampleCount;

/**
 * @brief 32 bits of pure random.
 */
static volatile uint32_t random_dword;

/**
 * @brief If the variable equal FALSE then nothing has to be written to fram, else if the variable equal TRUE, random_dword should be written to fram
 */
static volatile uint8_t random_transfer;

/**
 * @brief The current size of the entropy pool stored in FRAM
 */
uint16_t entropy_pool_size;


// The following code is an implementation of Jenkin's one at a time hash
// This hash function has had preliminary testing to verify that it
// produces reasonably uniform random results when using WDT jitter
// on a variety of Arduino platforms
static void jenkin_one_at_a_time_step(volatile uint32_t* in, uint8_t val)
{
	(*in) += val;
	(*in) += ((*in) << 10);
	(*in) ^= ((*in) >> 6);
}

static void jenkin_one_at_a_time_final(volatile uint32_t* in)
{
	(*in) += ((*in) << 3);
	(*in) ^= ((*in) >> 11);
	(*in) += ((*in) << 15);
}

void random_init(void)
{
	random_transfer = TRUE;
	sampleCount = 0;
	entropy_pool_size = 0;
	random_dword = 0;

	cli();                         // Temporarily turn off interrupts, until WDT configured
	MCUSR = 0;                     // Use the MCU status register to reset flags for WDR, BOR, EXTR, and POWR
	_WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (1<<WDE);
	// WDTCSR |= _BV(WDCE) | _BV(WDE);// WDT control register, This sets the Watchdog Change Enable (WDCE) flag, which is  needed to set the 
	_WD_CONTROL_REG = _BV(WDIE);            // Watchdog system reset (WDE) enable and the Watchdog interrupt enable (WDIE)
	sei();                         // Turn interupts on
}


/**
 * @brief [This interrupt service routine is called every time the WDT interrupt is triggered.
 * With the default configuration that is approximately once every 16ms, producing 
 * approximately two 32-bit integer values every second.
 * 
 * @param val The Timer 1 low byte.
 */
static void isr_hardware_neutral(uint8_t val)
{
	if(random_transfer){
		return;
	}

	jenkin_one_at_a_time_step(&random_dword, val);
	++sampleCount;

	if (sampleCount >= NUM_SAMPLE)
	{
		jenkin_one_at_a_time_final(&random_dword);
		sampleCount = 0; // Start collecting the next 32 bytes of Timer 1 counts
		random_transfer = TRUE;
	}
}

ISR(WDT_vect)
{
	isr_hardware_neutral(TCNT1L); // Record the Timer 1 low byte (only one needed) 
}

void random_save_entropy(void)
{
	// If the random_dword should be transferred
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(random_transfer)
		{
			if(entropy_pool_size > SIZE_ENTROPY_POOL-4)
			{
				return;
			}
			fram_write_bytes(OFFSET_ENTROPY_POOL + entropy_pool_size, (uint8_t*)(&random_dword), 4); // Write the random value in the pool
			entropy_pool_size += 4; // Increase of 4 bytes the size of the entropy pool
			random_dword = 0;
			random_transfer = FALSE; // random_dword has been transferred !
		}
	}
}

uint8_t random_request_byte(void)
{	
	// Wait for entropy available
	// /!\ Global interrupt must be activated !!
	while(entropy_pool_size == 0)
	{
		random_save_entropy();
	}
	// Read the random byte and don't forget to clear it
	uint8_t random = fram_read_byte(OFFSET_ENTROPY_POOL + entropy_pool_size - 1);
	fram_write_byte(OFFSET_ENTROPY_POOL + entropy_pool_size - 1, 0);
	// Decrease pool size
	--entropy_pool_size;
	return random;
}

// This fonction use the request_byte implementation. random_min_max should not be used
// because it introduce bias by calling random_8 and random_16. More over random_max
// does not use the entropy pool.
uint8_t random_request_printable(void)
{
	// 126 is '~'
	uint8_t max = '~' - ' ';
	uint8_t slice;
	uint8_t retVal = WDT_MAX_8INT;

	slice = WDT_MAX_8INT / max;
	while (retVal >= max)
		retVal = random_request_byte() / slice;

	retVal += ' ';
	return retVal;
}

void random_reset(void)
{
	entropy_pool_size = 0;
}
