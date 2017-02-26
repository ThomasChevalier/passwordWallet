#include <avr/io.h>
#include <util/delay.h>

void main () 
{
	DDRD |= _BV(DDD5); // portd 5 output
	
	while(1)
	{
		 PORTD |= _BV(PORTD5); // on
		 _delay_ms (500);
		 PORTD &= ~_BV(PORTD5); // off
		 _delay_ms (500);
	} // end of loop
}
