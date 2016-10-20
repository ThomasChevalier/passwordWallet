#include "Transitions.h"

#include <string.h>

#include "Authentification.h"
#include "Events.h"
#include "States.h"

#include "Oled.h"
#include "String.h"


DECLARE_TRANSITION(STATE_INIT)
{
    if(!check_key())
    {
        wait_for_valid_card();
        return STATE_INIT;
    }
    return STATE_MAIN;
}

DECLARE_TRANSITION(STATE_MAIN)
{
    if(event & EVENT_USB_DISCONNECTED)
        return STATE_SAVE;

    if(event & EVENT_BUTTON_1)
    {

    }
    else if(event & EVENT_BUTTON_2)
    {
    }
    else if(event & EVENT_BUTTON_3)
    {

    }

    return STATE_MAIN;
}

DECLARE_TRANSITION(STATE_BROWSE)
{

    return STATE_BROWSE;
}

void drawOptionMenu(uint8_t currentChoice)
{
    const char* strTable[3] = {str_option_genNew, str_option_changeMasterKey, str_option_back};
    uint8_t i = 0;
    for(; i < 3; ++i)
    {
        str_to_buffer(strTable[i]);
        oled_draw_text(0, i*8, str_buffer, 0);
    }
    for(i = 0; i < 8; ++i)
    {
        oled_h_line(0, currentChoice * 8 + i, 128, INVERSE);
    }
}

DECLARE_TRANSITION(STATE_OPTION)
{
    const uint8_t numberOfChoice = 3;
    static uint8_t currentChoice = 0;

    if(event & EVENT_USB_DISCONNECTED)
        return STATE_SAVE;

    oled_clear_display();
    drawOptionMenu(currentChoice);
    oled_display();

    if(event & EVENT_BUTTON_1)
    {
        // Looping
        currentChoice = (currentChoice == 0) ? numberOfChoice-1 : currentChoice - 1;
        return STATE_OPTION;
    }
    else if(event & EVENT_BUTTON_2)
    {
        switch(currentChoice)
        {
        case 0:
            //pwd_generateNew();
            break;
        case 1:
            //changeMainPassword();
            break;
        case 2:
            // Nothing
            break;
        }
        return STATE_MAIN;
    }
    else if(event & EVENT_BUTTON_3)
    {
        // Looping
        currentChoice = (currentChoice == numberOfChoice-1) ? 0 : currentChoice + 1;
        return STATE_OPTION;
    }

    return STATE_OPTION;
}

DECLARE_TRANSITION(STATE_SAVE) 
{
    return STATE_SAVE;
}
