#include "Random.h"

#include <random>
#include <chrono>

std::default_random_engine generator;

#define WDT_RETURN_BYTE (256)
#define WDT_RETURN_WORD (65536)
#define WDT_MAX_8INT (0xFF)
#define WDT_MAX_16INT (0xFFFF)
#define WDT_MAX_32INT (0xFFFFFFFF)

void random_init(void)
{
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
  
/**
 * @brief [This interrupt service routine is called every time the WDT interrupt is triggered.
 * With the default configuration that is approximately once every 16ms, producing 
 * approximately two 32-bit integer values every second.
 * 
 * @param val The Timer 1 low byte.
 */


void random_save_entropy(void)
{

}

uint8_t random_request_byte(void)
{
    std::uniform_int_distribution<int> distribution(0,255);
    return distribution(generator);
}

// This fonction use the request_byte implementation. random_min_max should not be used
// because it introduce bias by calling random_8 and random_16. More over random_max
// does not use the entropy pool.
uint8_t random_request_printable(void)
{
    // 126 is '~'
    uint8_t max = 126 - ' ';
    uint32_t slice;
    uint32_t retVal = WDT_MAX_32INT;

    slice = WDT_MAX_8INT / max;
    while (retVal >= max)
        retVal = random_request_byte() / slice;

    retVal += ' ';
    return retVal;
}
