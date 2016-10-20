#ifndef BUTTONS_HEADER_THOMAS_CHEVALIER
#define BUTTONS_HEADER_THOMAS_CHEVALIER

#include <avr/io.h>

#define BUTTON_1_PIN_NUM 4
#define BUTTON_2_PIN_NUM 4
#define BUTTON_3_PIN_NUM 7

#define BUTTON_1_DDR DDRD
#define BUTTON_2_DDR DDRB
#define BUTTON_3_DDR DDRC

#define BUTTON_1_PIN PIND
#define BUTTON_2_PIN PINB
#define BUTTON_3_PIN PINC

#define BUTTON_1_PORT PORTD
#define BUTTON_2_PORT PORTB
#define BUTTON_3_PORT PORTC

void buttons_setup_hardware();

void buttons_update_event(); // Update event flag for buttons, should be call regularly
unsigned char buttons_pressed(); // Bit 0 for button 1, bit 2 for button 1, etc ... 1 <=> pressed / 0 <=> released

// Not needed since the buttons are hardware debounced
//void debounce(unsigned char* butt); // Modify butt
//void _debounce_one(unsigned char bitMask, unsigned char *butt, unsigned char* lastState, unsigned char* counter);


#endif