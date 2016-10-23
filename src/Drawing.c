#include "Drawing.h"

#include "String.h"
#include "Globals.h"
#include "Oled.h"


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

	uint8_t x = 0;
	uint8_t y = 0;
	for(; x < 7; ++x)
	{
		for(y = 0; y < 64; ++y)
		{
			oled_draw_pixel(x,y,BLACK);
		}
	}
	
	oled_v_line(7, 0, 64, WHITE);
	x = letter - 3;
	y = 0;
	for(; x <= letter + 3; ++x )
	{
		char c = x;
		if(x < '@')
			c += 27;
		if(x > 'Z')
			c-= 27;
		oled_draw_char(1, y*9, c);
		++y;
	}
	if(highlight)
	{
		for(y = 0; y < 7; ++y)
		{
			oled_h_line(0, 26+y, 6, INVERSE);
		}
	}
	
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

void draw_typing_column(uint8_t column)
{

}