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

#define gWDT_buffer_SIZE 32
static uint8_t gWDT_buffer[gWDT_buffer_SIZE];
static uint8_t gWDT_buffer_position;
static uint8_t gWDT_loop_counter;

/**
 * @brief 32 bits of pure random.
 */
static volatile uint32_t random_dword;

/**
 * @brief If the variable equal 0 then nothing has to be written to fram, else if the variable equal 1, random_dword should be written to fram
 */
static volatile uint8_t random_transfer;

void random_init(void)
{
    random_transfer = 0;
    gWDT_buffer_position=0;

    cli();                         // Temporarily turn off interrupts, until WDT configured
    MCUSR = 0;                     // Use the MCU status register to reset flags for WDR, BOR, EXTR, and POWR
    _WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (1<<WDE);
    // WDTCSR |= _BV(WDCE) | _BV(WDE);// WDT control register, This sets the Watchdog Change Enable (WDCE) flag, which is  needed to set the 
    _WD_CONTROL_REG = _BV(WDIE);            // Watchdog system reset (WDE) enable and the Watchdog interrupt enable (WDIE)
    sei();                         // Turn interupts on
}


//  
/**
 * @brief [This interrupt service routine is called every time the WDT interrupt is triggered.
 * With the default configuration that is approximately once every 16ms, producing 
 * approximately two 32-bit integer values every second.
 * 
 * @param val The Timer 1 low byte.
 */
static void isr_hardware_neutral(uint8_t val)
{
    gWDT_buffer[gWDT_buffer_position] = val;
    gWDT_buffer_position++;                     // every time the WDT interrupt is triggered
    if (gWDT_buffer_position >= gWDT_buffer_SIZE)
    {
        random_dword = 0;

        // The following code is an implementation of Jenkin's one at a time hash
        // This hash function has had preliminary testing to verify that it
        // produces reasonably uniform random results when using WDT jitter
        // on a variety of Arduino platforms
        for(gWDT_loop_counter = 0; gWDT_loop_counter < gWDT_buffer_SIZE; ++gWDT_loop_counter)
        {
            random_dword += gWDT_buffer[gWDT_loop_counter];
            random_dword += (random_dword << 10);
            random_dword ^= (random_dword >> 6);
        }
        random_dword += (random_dword << 3);
        random_dword ^= (random_dword >> 11);
        random_dword += (random_dword << 15);
        gWDT_buffer_position = 0; // Start collecting the next 32 bytes of Timer 1 counts
        random_transfer = 1;
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
            // Read how many size left and write the dword at the good position
            uint16_t entropyPoolSize = 0;
            fram_read_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
            if(entropyPoolSize > SIZE_ENTROPY_POOL-4)
            {
                return;
            }
            uint16_t pos = entropyPoolSize;//(entropyPoolSize > SIZE_ENTROPY_POOL-4) ? SIZE_ENTROPY_POOL-4 : entropyPoolSize;
            fram_write_bytes(OFFSET_ENTROPY_POOL + pos, (uint8_t*)(&random_dword), 4); // Write the random value in the pool
            entropyPoolSize += 4; // Increase of 4 bytes the size of the entropy pool
            fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2); // Write it to the fram
            random_transfer = 0; // random_dword has been transferred !
        }
    }
}

uint8_t random_request_byte(void)
{
    uint16_t entropyPoolSize = 0;
    // Read how many bytes are in fram
    fram_read_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
    
    // Wait for entropy available
    // /!\ Global interrupt must be activated !!
    while(entropyPoolSize == 0)
    {
        uint8_t cpTransfer = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            cpTransfer = random_transfer;
        }
        if(cpTransfer)
        {
            random_save_entropy();
            fram_read_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
        }
    }
    // Read the random byte and don't forget to clear it
    uint8_t random = fram_read_byte(OFFSET_ENTROPY_POOL + entropyPoolSize - 1);
    fram_write_byte(OFFSET_ENTROPY_POOL + entropyPoolSize - 1, 0);
    // Decrease pool size
    --entropyPoolSize;
    fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
    return random;
}

// This fonction use the request_byte implementation. random_min_max should not be used
// because it introduce bias by calling random_8 and random_16. More over random_max
// does not use the entropy pool.
uint8_t random_request_printable(void)
{
    // 126 is '~'
    uint8_t max = 127 - ' ';
    uint32_t slice;
    uint32_t retVal = WDT_MAX_32INT;

    slice = WDT_MAX_8INT / max;
    while (retVal >= max)
        retVal = random_request_byte() / slice;

    retVal += ' ';
    return retVal;
}
