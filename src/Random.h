#ifndef RANDOM_HEADER_THOMAS_CHEVALIER
#define RANDOM_HEADER_THOMAS_CHEVALIER

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

typedef union 
{
  uint32_t int32;
  uint16_t int16[2];
  uint8_t int8[4];
}ENTROPY_LONG_WORD;

extern ENTROPY_LONG_WORD share_entropy;
extern uint32_t retVal;
  
void random_init();

uint32_t random_32();
uint8_t random_8();
// uint16_t random_16();
// uint32_t random_max(uint32_t max);
// uint32_t random_min_max(uint32_t min, uint32_t max);
uint8_t random_available();

// Save entropy to the fram
void random_save_entropy();
// Return a value in [0; 255] range, make the best use of entropy pool
uint8_t random_request_byte();
// Return a value in [' '; '~'] range
uint8_t random_request_printable();
// Fill a buffer of len byte of random byte
void random_fill(uint8_t* buff, uint8_t len);

#endif // RANDOM_HEADER_THOMAS_CHEVALIER