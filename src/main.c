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
    EVENT_PASSWORD_ENTERED,                    TRANSITION(STATE_INIT)
  };
  states[STATE_MAIN]  =   (State)
  {
    EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_MAIN)
  };
  states[STATE_BROWSE] = (State)
  {
    EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_BROWSE)
  };
  states[STATE_OPTION]=   (State)
  {
    EVENT_ALL_BUTTON | EVENT_USB_DISCONNECTED, TRANSITION(STATE_OPTION)
  };
  states[STATE_SAVE]  =   (State)
  {
    EVENT_USB_CONNECTED,                       TRANSITION(STATE_SAVE)
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