#include "Security.h"

#include <util/delay.h>

#include "../Globals.h"
#include "Random.h"
#include "../Graphics/Drawing.h"

void security_init(void)
{
	random_init();
}


uint8_t security_memcmp(const void *data1, const void* data2, uint8_t sz)
{
	
	//C equivalent code
	// uint8_t i = 0;
	// uint8_t result = 0;
	// do
	// {
	// 	uint8_t a = ((uint8_t*)(data1))[i];
	// 	uint8_t b = ((uint8_t*)(data2))[i];
	// 	result = result | (a ^ b);
	// 	++i;
	// 	--sz;
	// }while(sz != 0);

	// uint8_t ret = RETURN_ERROR;
	// if(result == 0)
	// {
	// 	ret = RETURN_SUCCESS;
	// }

	// return ret;
	
	

	// Function “call-used” registers are r18-r27, and r30-r31
	// r24 is the ret register
	// __tmp_reg__ is the result
	// r18 is the size (normally)
	// r19 is a
	// r20 is b
	// X = (r26, r27)
	// Y = (r28, r29) -> cannot use because not a "call-used" register
	// Z = (r30, r31)
	asm volatile (
		"movw X, %[data1]"				"\n\t"
		"movw Z, %[data2]"				"\n\t"
		"mov __tmp_reg__, __zero_reg__"	"\n\t"
		"loop%=:"						"\n\t"
		"ld r19, X+"					"\n\t"
		"ld r20, Z+"					"\n\t"
		"eor r20, r19"					"\n\t"
		"or __tmp_reg__, r20"			"\n\t"
		"dec r18"					"\n\t"
		"brne loop%="					"\n\t"
		"ldi r24, %[error]"				"\n\t"
		"cp __tmp_reg__, __zero_reg__"	"\n\t"
		"breq no_error%="				"\n\t"
		"rjmp error%="					"\n\t"
		"no_error%=:"					"\n\t"
		"ldi r24, %[success]"			"\n\t"
		"error%=:"						"\n\t"
		"ret"							"\n\t"
		: 
		: [data1] "r" (data1), [data2] "r" (data2),  [size] "r" (sz), [error] "M" (RETURN_ERROR), [success] "M" (RETURN_SUCCESS)
		: "r0", "r19", "r20", "r26", "r27", "r30", "r31");

	// breq : 2 ticks if branching, 1 otherwise
	// rjmp : 2 ticks
	// nop : 1 tick
	// ldi : 1 tick
	// Total : 3 ticks on each branch (after the loop)
}
