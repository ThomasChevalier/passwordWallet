#include "State_Option_Sort.h"

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Globals.h"

#include "../../Graphics/Drawing.h"

#include "../../Hardware/Fram.h"


static uint8_t currentChoice = 0;

static void do_no_sorting(void)
{
	OPTIONS_FLAG &= 0xF9;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

static void do_sort_usage(void)
{
	OPTIONS_FLAG &= 0xF9;
	OPTIONS_FLAG |= 0x02;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}
static void do_sort_alpha(void)
{
	OPTIONS_FLAG &= 0xF9;
	OPTIONS_FLAG |= 0x04;
	fram_write_byte(OFFSET_OPTIONS_FLAG, OPTIONS_FLAG);
}

void state_option_sort_begin (void)
{
	currentChoice = 0;
	draw_list(	currentChoice + OPTION_SORT_MENU_MIN_STR_INDEX,
				OPTION_SORT_MENU_MIN_STR_INDEX,
				OPTION_SORT_MENU_MIN_STR_INDEX + OPTION_SORT_MENU_MAX_CHOICE-1);
}

uint8_t state_option_sort_transition (uint8_t event)
{
	if(event & EVENT_BUTTON_1)
	{
		if(currentChoice == 0)
		{
			currentChoice = OPTION_SORT_MENU_MAX_CHOICE - 1;
		}
		else
		{
			--currentChoice;
		}
	}
	else if(event & EVENT_BUTTON_2)
	{
		// Note : a switch generate less code than if else for this case
		switch(currentChoice)
		{
			case 0:
				do_no_sorting();
				return STATE_MAIN;
			case 1:
				do_sort_usage();
				return STATE_MAIN;
			case 2:
				do_sort_alpha();
				return STATE_MAIN;
			default:
				// Impossible
				break;
		}
	}
	else if(event & EVENT_BUTTON_3)
	{
		++currentChoice;
		if(currentChoice == OPTION_SORT_MENU_MAX_CHOICE)
		{
			currentChoice = 0;
		}
	}
	else if(event & EVENT_BUTTON_4)
	{
		return STATE_OPTION;
	}

	if(event & EVENT_ALL_BUTTONS)
	{
		draw_list(	currentChoice + OPTION_SORT_MENU_MIN_STR_INDEX,
					OPTION_SORT_MENU_MIN_STR_INDEX,
					OPTION_SORT_MENU_MIN_STR_INDEX + OPTION_SORT_MENU_MAX_CHOICE-1);
	}

	return STATE_OPTION_SORT;
}

void state_option_sort_end (void)
{

}
