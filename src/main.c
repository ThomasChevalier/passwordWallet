#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  // for sei()
#include <util/delay.h>  // for _delay_ms()
#include <string.h>

#include "Spi.h"
#include "Oled.h"
#include "Rfid.h"
#include "Fram.h"
#include "Keyboard.h"
#include "Buttons.h"
// 
#include "States.h"
#include "Transitions.h"
#include "Events.h"

#include "Globals.h"
#include "Authentification.h"
#include "Passwords.h"

void init_hardware()
{
  // Because spi is master mode, PB0 (aka Slave Select pin) should be an output, or, if it is an output, should be in high state
  DDRB |= 0x01;
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

  State* currentState = &states[STATE_INIT];

  init_hardware();
  init_software();

  // For test
  oled_clear_display();
  oled_display();

  // For test
  //wait_for_valid_card();
  eventHappen(EVENT_PASSWORD_ENTERED);
  while(RUNNING)
  {
    buttons_update_event();
    uint8_t event = getEvents(); // Mask of events

    if(currentState->event_mask & event)
    {
       uint8_t newState = currentState->transition(event);
       currentState = &states[newState];
    }
    _delay_ms(100);
  }
}