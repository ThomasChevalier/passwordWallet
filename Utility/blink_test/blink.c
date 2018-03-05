#include <avr/io.h>
#include <util/delay.h>

void main () 
{
	DDRC |= _BV(DDC7); // portd 5 output
	
	while(1)
	{
		 PORTC |= _BV(PORTC7); // on
		 _delay_ms (500);
		 PORTC &= ~_BV(PORTC7); // off
		 _delay_ms (500);
	} // end of loop
}
