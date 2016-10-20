#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  // for sei()
#include <util/delay.h>  // for _delay_ms()
#include <string.h>

#define sbi(x,y) x |= _BV(y) //set bit
#define cbi(x,y) x &= ~(_BV(y)) //clear bit
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) ((x & _BV(y)) == _BV(y)) //check if the input pin is high
#define is_low(x,y) ((x & _BV(y)) == 0) //check if the input pin is low


#include "Spi.h"
#include "Oled.h"
#include "Rfid.h"
#include "Fram.h"
#include "Keyboard.h"
#include "Buttons.h"

#include "States.h"
#include "Transitions.h"
#include "Events.h"

#include "Globals.h"
#include "Authentification.h"

void init_hardware()
{
  fram_setup_hardware();
  oled_setup_hardware();
    //rfid_setup_hardware();
  spi_setup_hardware();
  buttons_setup_hardware();
}

void init_software()
{
  oled_init();
  //rfid_init();
  //keyboard_init();
}

int main()
{
  State states[NUM_STATES];
  states[STATE_INIT]  =   (State)
  {
    EVENT_PASSWORD_ENTERED,                     TRANSITION(STATE_INIT)
  };
  states[STATE_MAIN]  =   (State)
  {
    EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED,  TRANSITION(STATE_MAIN)
  };
  states[STATE_BROWSE] = (State)
  {
      EVENT_BUTTON_1 | EVENT_BUTTON_2 | EVENT_BUTTON_3 | EVENT_USB_DISCONNECTED, TRANSITION(STATE_BROWSE)
  };
  states[STATE_OPTION]=   (State)
  {
    EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED,  TRANSITION(STATE_OPTION)
  };
  states[STATE_SAVE]  =   (State)
  {
    EVENT_USB_CONNECTED,                        TRANSITION(STATE_SAVE)
  };

  State* currentState = &states[0];

  init_hardware();
  init_software();

  oled_display();
  wait_for_valid_card();


  while(RUNNING)
  {
        uint8_t event = getEvents(); // Mask of events

        if(currentState->event_mask & event)
        {
          uint8_t newState = currentState->transition(event);
          currentState = &states[newState];
        }
  }
}

/*
Not working ...
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include "Spi.h"
#include "Oled.h"

#define false 0
#define true 1

volatile uint8_t sample = 0;
volatile uint8_t sample_waiting = false;
uint8_t current_bit = 0;
uint8_t result = 0;

uint8_t x = 0;
uint8_t y = 0;


 // Rotate bits to the left
 // https://en.wikipedia.org/wiki/Circular_shift#Implementing_circular_shifts
 uint8_t rotl(const uint8_t value, int shift) {
   if ((shift &= sizeof(value)*8 - 1) == 0)
     return value;
   return (value << shift) | (value >> (sizeof(value)*8 - shift));
 }

 // Setup of the watchdog timer.
 void wdtSetup() {
   cli();
   MCUSR = 0;

   // Start timed sequence
   WDTCSR |= _BV(WDCE) | _BV(WDE);

   // Put WDT into interrupt mode
   // Set shortest prescaler(time-out) value = 2048 cycles (~16 ms)
   WDTCSR = _BV(WDIE);

   sei();
 }

 // Watchdog Timer Interrupt Service Routine
 ISR(WDT_vect)
 {
   sample = TCNT1L; // Ignore higher bits
   sample_waiting = true;
 }

void binprint(int input) {
  unsigned int mask = 0x80;
  for (; mask; mask >>= 1) {
    oled_draw_char(x, y, mask&input?'1':'0');
    x += 5;
    if(x > 123)
    {
      x = 0;
      y += 8;
    }
    oled_display();
  }
}



void main(void)
{
  // sys_init
  DDRD = 0xC0;
  DDRB = 0xFF; // All pin or port B are output
  PORTB = 0x00;

  wdtSetup();

  spi_init_master();
  oled_init();

  oled_clear_display();
  while(1)
  {
     if (sample_waiting) {
       sample_waiting = false;

       result = rotl(result, 1); // Spread randomness around
       result ^= sample; // XOR preserves randomness

       current_bit++;
       if (current_bit > 7)
       {
         current_bit = 0;
         binprint(result);
       }
    }
  }
}*/
