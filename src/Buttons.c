#include "Buttons.h"

#include <avr/interrupt.h>
#include "Events.h"

void buttons_update_event()
{
    uint8_t butts = buttons_pressed();
    cli();
    GLOBALS_EVENTS |= butts;
    sei();
}

void buttons_setup_hardware()
{
	// Buttons' pins defined as input
	BUTTON_1_DDR &= ~(1 << BUTTON_1_PIN_NUM);
	BUTTON_2_DDR &= ~(1 << BUTTON_2_PIN_NUM);
	BUTTON_3_DDR &= ~(1 << BUTTON_3_PIN_NUM);

    // Active pull-up resistors
	BUTTON_1_PORT |= (1 << BUTTON_1_PIN_NUM);
	BUTTON_2_PORT |= (1 << BUTTON_2_PIN_NUM);
	BUTTON_3_PORT |= (1 << BUTTON_3_PIN_NUM);
}

unsigned char buttons_pressed()
{
    uint8_t rep = 0;
    // Check the 3 buttons and write corresponding byte
    if (BUTTON_1_PIN & (1<<BUTTON_1_PIN_NUM))
    {
    	rep |= EVENT_BUTTON_1;
    }
    if (BUTTON_2_PIN & (1<<BUTTON_2_PIN_NUM))
    {
    	rep |= EVENT_BUTTON_2;
    }
    if (BUTTON_3_PIN & (1<<BUTTON_3_PIN_NUM))
    {
    	rep |= EVENT_BUTTON_3;
    }

    rep ^= 0x07; // Inverse result because here 1 <=> released (input pullup) and we want 1 <=> pressed

    return rep;
}

/*
void debounce(unsigned char* butt)
{
    static unsigned char counter1 = 0;
    static unsigned char counter2 = 0;
    static unsigned char counter3 = 0;
    static unsigned char lastState = 0x00; // Default case

    _debounce_one(0x01, butt, &lastState, &counter1);
    _debounce_one(0x02, butt, &lastState, &counter2);
    _debounce_one(0x04, butt, &lastState, &counter3);

}

void _debounce_one(unsigned char bitMask, unsigned char *butt, unsigned char* lastState, unsigned char* counter)
{
    unsigned char reading = (*butt) & bitMask;
    unsigned char last = (*lastState) & bitMask;
    *butt = (last) ? (*butt) | bitMask : (*butt) & (0xFF ^ bitMask); // By default, ignore changement and set the state to the last state

    if(reading != last)
    {
        ++(*counter);
        if((*counter) > 10)
        {
            --(*counter);
            // State changed
            // Last become actual read and read become ... read
            *lastState = (reading) ? (*lastState) | bitMask : (*lastState) & (0xFF ^ bitMask);
            *butt = (reading) ? (*butt) | bitMask : (*butt) & (0xFF ^ bitMask);
            //(*counter) = 0; // Long press = multiple press
        }
    }
    else
    {
        (*counter) = 0;
    }

}
//*/