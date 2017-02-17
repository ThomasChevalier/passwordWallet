#include "Buttons.h"

#include "Globals.h"
#include "Events.h"
#include "PinDefinition.h"

void buttons_update_event()
{
    static uint8_t last_butts = 0;

    uint8_t butts = buttons_pressed();
    if(butts != last_butts)
    {
        FIRST_PRESS = 1;
        last_butts = butts;
    }
    else
    {
        FIRST_PRESS = 0;
    }
    GLOBALS_EVENTS |= butts;
}

void buttons_setup_hardware()
{
    // Buttons' pins defined as input
    BUTTON_1_DDR &= ~(1 << BUTTON_1_PIN_NUM);
    BUTTON_2_DDR &= ~(1 << BUTTON_2_PIN_NUM);
    BUTTON_3_DDR &= ~(1 << BUTTON_3_PIN_NUM);
    BUTTON_4_DDR &= ~(1 << BUTTON_4_PIN_NUM);

    // Active pull-up resistors
    BUTTON_1_PORT |= (1 << BUTTON_1_PIN_NUM);
    BUTTON_2_PORT |= (1 << BUTTON_2_PIN_NUM);
    BUTTON_3_PORT |= (1 << BUTTON_3_PIN_NUM);
    BUTTON_4_PORT |= (1 << BUTTON_4_PIN_NUM);
}

unsigned char buttons_pressed()
{
    uint8_t rep = 0;
    // Check the 4 buttons and write corresponding byte
    if (BUTTON_1_PIN & (1<<BUTTON_1_PIN_NUM))
    {
        rep |= ( OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION) ) ? EVENT_BUTTON_3 : EVENT_BUTTON_1;
    }
    if (BUTTON_2_PIN & (1<<BUTTON_2_PIN_NUM))
    {
        rep |= ( OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION) ) ? EVENT_BUTTON_4 : EVENT_BUTTON_2;
    }
    if (BUTTON_3_PIN & (1<<BUTTON_3_PIN_NUM))
    {
        rep |= ( OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION) ) ? EVENT_BUTTON_1 : EVENT_BUTTON_3;
    }
    if (BUTTON_4_PIN & (1<<BUTTON_4_PIN_NUM))
    {
        rep |= ( OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_ORIENTATION) ) ? EVENT_BUTTON_2 : EVENT_BUTTON_4;
    }

    rep ^= 0x0F; // Inverse result because here 1 <=> released (input pullup) and we want 1 <=> pressed

    return rep;
}