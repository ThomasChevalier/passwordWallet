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
uint16_t random_16();
uint32_t random_max(uint32_t max);
uint32_t random_min_max(uint32_t min, uint32_t max);
uint8_t random_available();

void random_save_entropy();
uint8_t random_request_byte();
void random_fill(uint8_t* buff, uint8_t len);

#endif // RANDOM_HEADER_THOMAS_CHEVALIER