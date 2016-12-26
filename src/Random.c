#include "Random.h"

#include "Globals.h"
#include "Fram.h"

// Define save ram and prog mem
// Save 16 byte of progmem and 16 byte of ram
#define WDT_RETURN_BYTE (256)
#define WDT_RETURN_WORD (65536)
#define WDT_MAX_8INT (0xFF)
#define WDT_MAX_16INT (0xFFFF)
#define WDT_MAX_32INT (0xFFFFFFFF)

#define gWDT_buffer_SIZE 32
#define WDT_POOL_SIZE 8
static uint8_t gWDT_buffer[gWDT_buffer_SIZE];
static uint8_t gWDT_buffer_position;
static uint8_t gWDT_loop_counter;
static volatile uint8_t gWDT_pool_start;
static volatile uint8_t gWDT_pool_end;
static volatile uint8_t gWDT_pool_count;
static volatile uint32_t gWDT_entropy_pool[WDT_POOL_SIZE];
 

ENTROPY_LONG_WORD share_entropy;
uint32_t retVal;

// This function initializes the global variables needed to implement the circular entropy pool and
// the buffer that holds the raw Timer 1 values that are used to create the entropy pool.  It then
// Initializes the Watch Dog Timer (WDT) to perform an interrupt every 2048 clock cycles, (about 
// 16 ms) which is as fast as it can be set.
void random_init(void)
{
  gWDT_buffer_position=0;
  gWDT_pool_start = 0;
  gWDT_pool_end = 0;
  gWDT_pool_count = 0;

  cli();                         // Temporarily turn off interrupts, until WDT configured
  MCUSR = 0;                     // Use the MCU status register to reset flags for WDR, BOR, EXTR, and POWR
  _WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (1<<WDE);
  // WDTCSR |= _BV(WDCE) | _BV(WDE);// WDT control register, This sets the Watchdog Change Enable (WDCE) flag, which is  needed to set the 
  _WD_CONTROL_REG = _BV(WDIE);            // Watchdog system reset (WDE) enable and the Watchdog interrupt enable (WDIE)
  sei();                         // Turn interupts on
}

// This function returns a uniformly distributed random integer in the range
// of [0,0xFFFFFFFF] as long as some entropy exists in the pool and a 0
// otherwise.  To ensure a proper random return the available() function
// should be called first to ensure that entropy exists.
//
// The pool is implemented as an 8 value circular buffer
uint32_t random_32(void)
{
  uint8_t waiting;
  while (gWDT_pool_count < 1)
    waiting += 1;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    retVal = gWDT_entropy_pool[gWDT_pool_start];
    gWDT_pool_start = (gWDT_pool_start + 1) % WDT_POOL_SIZE;
    --gWDT_pool_count;
  }
  return(retVal);
}

// This function returns one byte of a single 32-bit entropy value, while preserving the remaining bytes to
// be returned upon successive calls to the method.  This makes best use of the available entropy pool when
// only bytes size chunks of entropy are needed.  Not available to public use since there is a method of using
// the default random method for the end-user to achieve the same results.  This internal method is for providing
// that capability to the random method, shown below
uint8_t random_8(void)
{
  static uint8_t byte_position=0;
  uint8_t retVal8;

  if (byte_position == 0)
    share_entropy.int32 = random_32();
  retVal8 = share_entropy.int8[byte_position++];
  byte_position = byte_position % 4;
  return(retVal8);
}

/*
// This function returns one word of a single 32-bit entropy value, while preserving the remaining word to
// be returned upon successive calls to the method.  This makes best use of the available entropy pool when
// only word sized chunks of entropy are needed.  Not available to public use since there is a method of using
// the default random method for the end-user to achieve the same results.  This internal method is for providing
// that capability to the random method, shown below
uint16_t random_16(void)
{
  static uint8_t word_position=0;
  uint16_t retVal16;

  if (word_position == 0)
    share_entropy.int32 = random_32();
  retVal16 = share_entropy.int16[word_position++];
  word_position = word_position % 2;
  return(retVal16);
}


// This function returns a uniformly distributed integer in the range of 
// of [0,max).  The added complexity of this function is required to ensure
// a uniform distribution since the naive modulus max (% max) introduces
// bias for all values of max that are not powers of two.
//
// The loops below are needed, because there is a small and non-uniform chance
// That the division below will yield an answer = max, so we just get
// the next random value until answer < max.  Which prevents the introduction
// of bias caused by the division process.  This is why we can't use the 
// simpler modulus operation which introduces significant bias for divisors
// that aren't a power of two
uint32_t random_max(uint32_t max)
{
  uint32_t slice;

  if (max < 2)
    retVal=0;
  else
    {
      retVal = WDT_MAX_32INT;
      if (max <= WDT_MAX_8INT) // If only byte values are needed, make best use of entropy
	{                      // by diving the long into four bytes and using individually
	  slice = WDT_MAX_8INT / max;
	  while (retVal >= max)
	    retVal = random_8() / slice;
	} 
      else if (max <= WDT_MAX_16INT) // If only word values are need, make best use of entropy
	{                            // by diving the long into two words and using individually
	  slice = WDT_MAX_16INT / max;
	  while (retVal >= max)
	    retVal = random_16() / slice;
	} 
      else 
	{
	  slice = WDT_MAX_32INT / max;
	  while (retVal >= max)           
	    retVal = random_32() / slice;
	}                                 
    }
  return(retVal);
}

// This function returns a uniformly distributed integer in the range of 
// of [min,max).  
uint32_t random_min_max(uint32_t min, uint32_t max)
{
  uint32_t tmp_random, tmax;

  tmax = max - min;
  if (tmax < 1)
    retVal=min;
  else
    {
      tmp_random = random_max(tmax);
      retVal = min + tmp_random;
    }
  return(retVal);
}
*/

// This function returns a unsigned char (8-bit) with the number of unsigned long values
// in the entropy pool
uint8_t random_available(void)
{
  return(gWDT_pool_count);
}

// This interrupt service routine is called every time the WDT interrupt is triggered.
// With the default configuration that is approximately once every 16ms, producing 
// approximately two 32-bit integer values every second. 
//
// The pool is implemented as an 8 value circular buffer
static void isr_hardware_neutral(uint8_t val)
{
  gWDT_buffer[gWDT_buffer_position] = val;
  gWDT_buffer_position++;                     // every time the WDT interrupt is triggered
  if (gWDT_buffer_position >= gWDT_buffer_SIZE)
  {
    gWDT_pool_end = (gWDT_pool_start + gWDT_pool_count) % WDT_POOL_SIZE;
    // The following code is an implementation of Jenkin's one at a time hash
    // This hash function has had preliminary testing to verify that it
    // produces reasonably uniform random results when using WDT jitter
    // on a variety of Arduino platforms
    for(gWDT_loop_counter = 0; gWDT_loop_counter < gWDT_buffer_SIZE; ++gWDT_loop_counter)
    {
	    gWDT_entropy_pool[gWDT_pool_end] += gWDT_buffer[gWDT_loop_counter];
    	gWDT_entropy_pool[gWDT_pool_end] += (gWDT_entropy_pool[gWDT_pool_end] << 10);
      gWDT_entropy_pool[gWDT_pool_end] ^= (gWDT_entropy_pool[gWDT_pool_end] >> 6);
    }
    gWDT_entropy_pool[gWDT_pool_end] += (gWDT_entropy_pool[gWDT_pool_end] << 3);
    gWDT_entropy_pool[gWDT_pool_end] ^= (gWDT_entropy_pool[gWDT_pool_end] >> 11);
    gWDT_entropy_pool[gWDT_pool_end] += (gWDT_entropy_pool[gWDT_pool_end] << 15);
    gWDT_entropy_pool[gWDT_pool_end] = gWDT_entropy_pool[gWDT_pool_end];
    gWDT_buffer_position = 0; // Start collecting the next 32 bytes of Timer 1 counts
    if (gWDT_pool_count == WDT_POOL_SIZE) // The entropy pool is full
    {
      gWDT_pool_start = (gWDT_pool_start + 1) % WDT_POOL_SIZE;  
    }
    else // Add another unsigned long (32 bits) to the entropy pool
    {
      ++gWDT_pool_count;
    }
  }
}

ISR(WDT_vect)
{
  isr_hardware_neutral(TCNT1L); // Record the Timer 1 low byte (only one needed) 
}

void random_save_entropy()
{
  // If there is 8 or more uint32_t available, there is at least 8*4=32 uint8_t availables
  if(random_available() >= 8)
  {
    uint16_t entropyPoolSize = 0;
    fram_read_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
    uint8_t max = 0;
    if(entropyPoolSize > 1024-32)
    {
        max = 1024 - entropyPoolSize;
    }
    else
    {
      max = 32;
    }
    for(uint8_t i = 0; i < max; ++i)
    {
      fram_write_byte(OFFSET_ENTROPY_POOL + entropyPoolSize + i, random_8());
    }
    entropyPoolSize += max;
    fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
  }
}

uint8_t random_request_byte()
{
  // If entropy available in ram, retrieve it
  if(random_available > 0)
  {
    return random_8();
  }
  else
  {
    // Else if entropy available in fram, retrieve it
    uint16_t entropyPoolSize = 0;
    uint8_t random = 0;
    // Read how many bytes are in fram
    fram_read_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
    if(entropyPoolSize > 0) // There is enough byte
    {
        // Read the random byte and don't forget to clear it
        random = fram_read_byte(OFFSET_ENTROPY_POOL + entropyPoolSize - 1);
        fram_write_byte(OFFSET_ENTROPY_POOL + entropyPoolSize - 1, 0);
        // Decrease pool size
        entropyPoolSize -= 1;
        fram_write_bytes(OFFSET_ENTROPY_SIZE, (uint8_t*)(&entropyPoolSize), 2);
        return random;
    }
    else
    {
        // Else, wait for entropy (random_8 is blockant)
        return random_8(); 
    }
  }
}

// This fonction use the request_byte implementation. random_min_max should not be used
// because it introduce bias by calling random_8 and random_16. More over random_max
// does not use the entrop pool.
uint8_t random_request_printable()
{
  // 126 is '~'
  uint8_t max = 126 - ' ';
  uint32_t slice;
  retVal = WDT_MAX_32INT;

  slice = WDT_MAX_8INT / max;
  while (retVal >= max)
    retVal = random_request_byte() / slice;

  retVal += ' ';
  return retVal;
}

void random_fill(uint8_t* buff, uint8_t len)
{
  for(uint8_t i = 0; i < len; ++i)
  {
    buff[i] = random_request_byte();
  }
}