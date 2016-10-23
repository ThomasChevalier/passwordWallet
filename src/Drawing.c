#include "Drawing.h"

#include <string.h>

#include <util/delay.h>

#include "String.h"
#include "Globals.h"
#include "Oled.h"


// For type string
#include "Buttons.h"
#include "Events.h"


void draw_main_menu()
{
	oled_clear_display();
	draw_browse_dock(0,0);

	oled_h_line(8, 20, 120, WHITE);

	oled_h_line(8, 43, 120, WHITE);
	//oled_draw_text(10, 2 , PWD_NAME_1, 0);
	//oled_draw_text(10, 23, PWD_NAME_2, 0);
	//oled_draw_text(10, 46, PWD_NAME_3, 0);
	oled_display();
}


void draw_browse_dock(char letter, uint8_t highlight)
{
	static char lastLetter = '@';
	if(letter == 0)
		letter = lastLetter;
	else
		lastLetter = letter;

	draw_char_column((highlight<<4) | (1<<5),letter);
	oled_v_line(7, 0, 64, WHITE);
}


void draw_option_menu(uint8_t currentChoice)
{
	oled_clear_display();
	uint8_t pos;
	if(currentChoice == 0)
		pos = 0;
	else if(currentChoice == 6)
		pos = 2;
	else
		pos = 1;

	if(currentChoice == 6)
		--currentChoice;
	if(currentChoice > 0)
		--currentChoice;

	currentChoice += 4;

	uint8_t i = 0;
	for(; i < 3; ++i)
	{
		str_to_buffer(currentChoice);
		++currentChoice;
		oled_draw_text(2, i*23+2, str_buffer, 0);
	}

	uint8_t l_table[3] = {0, 20, 43};

	for(i = 0; i < 21; ++i)
	{
		oled_h_line(0, l_table[pos] + i, 128, INVERSE);
	}
	if(pos == 1)
	{
		oled_h_line(0, 41, 128, INVERSE);
		oled_h_line(0, 42, 128, INVERSE);
	}
	oled_display();
}

// yesNo = 0 = no
// yesNo = 1 = yes
void draw_confirmation_screen(uint8_t choice, uint8_t yesNo)
{
	oled_clear_display();

	str_to_buffer(choice + 4);
	oled_draw_text(2, 2, str_buffer, 0);

	str_to_buffer(STRING_MISC_NO);
	oled_draw_text(56, 19, str_buffer, 0);

	str_to_buffer(STRING_MISC_YES);
	oled_draw_text(56, 29, str_buffer, 0);

	uint8_t y = yesNo ? 28 : 18;
	for(uint8_t i = 0; i < 7; ++i)
		oled_h_line(55, y+i, 16, INVERSE);

	oled_display();
}

void type_string(char* string_typed, uint8_t maxLen)
{
	uint8_t pos = 0;
	uint8_t have_erased = 0;
	uint8_t running = 1;
	string_typed[strlen(string_typed)] = 127;
	while(running)
	{
		 _delay_ms(100);
		buttons_update_event();
	    uint8_t event = getEvents(); // Mask of events

	    if(event & EVENT_BUTTON_1)
		{
			char c = string_typed[pos];
			if(c == ' ')
				string_typed[pos] = 127;
			else
				string_typed[pos] = c - 1;
		}
		else if(event & EVENT_BUTTON_2)
		{
			// Maximum size has been reached
			if(pos == maxLen-1)
			{
				running = 0;
			}
			else
			{
				// Erase functionnality
				if(string_typed[pos] == 127)
				{
					if(have_erased)
					{
						running = 0;
					}
					else
					{
						for(uint8_t i = pos+1; i < maxLen; ++i)
						{
							string_typed[i] = 0;
						}
						have_erased = 1;
					}
				}
				else
				{
					have_erased = 0;
					++pos;
					if(string_typed[pos] == 0)
						string_typed[pos] = 127;
				}
			}
		}
		else if(event & EVENT_BUTTON_3)
		{
			char c = string_typed[pos];
			if(c == 127)
				string_typed[pos] = ' ';
			else
				string_typed[pos] = c + 1;
		}
		else if(event & EVENT_BUTTON_4)
		{
			if(string_typed[pos] == 127)
			{
				for(uint8_t i = pos+1; i < maxLen; ++i)
				{
					string_typed[i] = 0;
				}
			}

			if(pos >= 1)
			{
				pos --;
			}
			else
			{
				running = 0;
			}
		}
		if(event)
		{
			draw_typing_screen(string_typed, pos);
		}
	}
	for(uint8_t i = 0; i < maxLen; ++i)
	{
		if(string_typed[i] == 127)
			string_typed[i] = 0;
	}

}

void draw_typing_screen(char* str, uint8_t column)
{
	uint8_t len = strlen(str);

	if(len > 17 && column > 16)
	{
		// column -= 9;
		// for(uint8_t i = 9; i < 18; ++i)
		// {
		// 	oled_draw_char(i * 7 + 1, 0, str[i])
		// }
		// draw_char_column(len-1 | (1<<4), str[len-1]);
	}
	else
	{
		oled_clear_display();
		for(uint8_t i = 0; i < len; ++i)
		{
			oled_draw_char(i * 7 + 1, 3*9, str[i]);
		}
		draw_char_column(column | (1<<4), str[column]);
	}
	oled_display();
}

void draw_char_column(uint8_t column_and_flags, char letter)
{
	uint8_t x = (column_and_flags&0xF)*7;
	uint8_t y = 0;
	for(; x < ((column_and_flags&0xF)+1)*7; ++x)
	{
		for(y = 0; y < 64; ++y)
		{
			oled_draw_pixel(x,y,BLACK);
		}
	}
	x = letter - 3;
	y = 0;
	for(; x <= letter + 3; ++x )
	{
		char c = x;

		// Dock mode draw [@-Z]
		if((column_and_flags>>5) & 0x01)
		{
			if(x < '@')
			{
				c += 27;
			}
				
			if(x > 'Z')
			{
				c-= 27;
			}
		}
		// Full mode draw [' '-'~']
		else
		{
			if(x < ' ')
			{
				c += 96;
			}
			if(x > '~'+1)
			{
				c-= 96;
			}
		}
		
		oled_draw_char(((column_and_flags&0xF)*7)+1, y*9, c);
		++y;
	}
	if((column_and_flags>>4) & 0x01)
	{
		for(y = 0; y < 7; ++y)
		{
			oled_h_line((column_and_flags&0xF)*7, 26+y, 6, INVERSE);
		}
	}
}