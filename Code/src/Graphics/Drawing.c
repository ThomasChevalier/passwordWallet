#include "Drawing.h"

#include <string.h>

#include <util/delay.h>

#include "../Globals.h"

#include "String.h"
#include "font.h"

#include "../Hardware/Oled.h"

#include "../Security/Password.h"
#include "../Security/Password_List.h"

// For type string
#include "../Security/Random.h"
#include "../Hardware/Keyboard.h"
#include "../Hardware/Buttons.h"
#include "../FSM/Events.h"
#include "../Program/Program.h"

void draw_clear(void)
{
	oled_clear_display();
}

void draw_update(void)
{
	oled_display();
}

// Le caractère nulle doit être transformé en 127
// Retourne la longueur non écrite
uint8_t draw_char(uint8_t x, uint8_t y, uint8_t c)
{
	if(x + FONT_WIDTH >= 128 || y + FONT_HEIGHT >= 64)
	{
		return FONT_WIDTH;
	}

	uint8_t i;
	for(i = 0; i < FONT_WIDTH; ++i)
	{
		uint8_t font_byte = pgm_read_byte_near(font + ((c-' ') * FONT_WIDTH) + i);
		if(font_byte == 0)
		{
			return FONT_WIDTH - i;
		}
		uint8_t j = 0;
		for(j=0; j < 8; ++j)
		{
			oled_draw_pixel(x+i, y+(7-j), (font_byte >> (7-j)) & 0x01);
		}


	}
	return 0;
}


void draw_text(uint8_t x, uint8_t y, char *str, uint8_t str_len)
{
	if(str_len == 0)
		str_len = strlen(str);

	uint8_t i = 0;
	for(i = 0; i < str_len; ++i)
	{
		uint8_t s = draw_char(x, y, str[i]);
		x += FONT_WIDTH + 1 - s;
		if(x > 128-FONT_WIDTH+1)
		{
			x = 0;
			y += FONT_HEIGHT+3;
		}
	}
}


void draw_hex(uint8_t x, uint8_t y, uint8_t* hexBuffer, uint8_t hexBufferSize)
{
	uint8_t index = 0;
	char str[hexBufferSize * 2];
	for(index = hexBufferSize-1; index != (uint8_t)-1; --index)
	{
		uint8_t nibble = hexBuffer[index] >> 4;// High
		str[index * 2] = (nibble < 10) ? nibble + '0' : (nibble-10) + 'A';
		nibble = hexBuffer[index] & 0x0F; // Low
		str[index * 2 + 1] = (nibble < 10) ? nibble + '0' : (nibble-10) + 'A';
	}
	draw_text(x, y, str, hexBufferSize * 2);
}

void draw_h_line(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
	uint8_t i = 0;
	for(; i < w; ++i)
	{
		oled_draw_pixel(x+i, y, color);
	}
}

void draw_v_line(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{
	uint8_t i = 0;
	for(; i < h; ++i)
	{
		oled_draw_pixel(x, y+i, color);
	}
}



void draw_main_menu(void)
{
	draw_clear();
	draw_browse_dock(0,0);

	draw_h_line(8, 20, 120, WHITE);
	draw_h_line(8, 43, 120, WHITE);

	if(NUM_PWD != 0)
	{
		char pwdName[32];

		uint8_t pwd_id = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
		password_read_name(pwd_id, (uint8_t*)pwdName);
		draw_text(10, 2 , pwdName, 0);
		draw_hex(50, 2, &pwd_id, 1);

		pwd_id = CURRENT_PASSWORD_ID;
		password_read_name(pwd_id, (uint8_t*)pwdName);
		draw_text(10, 23, pwdName, 0);
		draw_hex(50, 23, &pwd_id, 1);

		pwd_id = pwd_list_get_next_pwd_id(CURRENT_PASSWORD_ID);
		password_read_name(pwd_id, (uint8_t*)pwdName);
		draw_text(10, 46, pwdName, 0);
		draw_hex(50, 46, &pwd_id, 1);
	}
	
	draw_update();
}

void draw_browse_dock(char letter, uint8_t highlight)
{
	static char lastLetter = '@';
	if(letter == 0)
	{
		letter = lastLetter;
	}
	else
	{
		lastLetter = letter;
	}

	draw_char_column((highlight<<4) | (1<<5),letter);
	draw_v_line(7, 0, 64, WHITE);
}

// Index is in the range [minIndex; maxIndex]
// Draw a list of string from str_option_table
void draw_list(uint8_t index, uint8_t minIndex, uint8_t maxIndex)
{
	draw_clear();

	// The position of the selected text (0 is the top, 1 the middle, 2 the bottom)
	uint8_t pos = 1;
	if(index == minIndex)
	{
		pos = 0;
	}
	else if(index == maxIndex)
	{
		pos = 2;
	}

	if(index == maxIndex)
	{
		--index;
	}
	if(index != minIndex)
	{
		--index;
	}

	uint8_t i = 0;
	for(; i < 3; ++i)
	{
		str_option_to_buffer(index + i);
		draw_text(2, i*23+2, str_buffer, 0);
	}

	uint8_t l_table[3] = {0, 20, 43};

	for(i = 0; i < 21; ++i)
	{
		draw_h_line(0, l_table[pos] + i, 128, INVERSE);
	}
	if(pos == 1)
	{
		draw_h_line(0, 41, 128, INVERSE);
		draw_h_line(0, 42, 128, INVERSE);
	}
	draw_update();
}

uint8_t type_string(char* string_typed, uint8_t maxLen)
{
	uint8_t pos = 0;
	uint8_t running = 1;
	uint8_t modified = 0;
	string_typed[strlen(string_typed)] = 127;
	draw_typing_screen(string_typed, pos);

	program_wait();

	while(running)
	{
		program_update();
		
		uint8_t event = events_get(); // Mask of events
		if(event & EVENT_BUTTON_1)
		{
			char c = string_typed[pos];
			if(c == ' ')
			{
				string_typed[pos] = 127;
			}
			else
			{
				string_typed[pos] = c - 1;
			}
			modified = 1;
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
					// If there is nothing to erase, then quit
					if(string_typed[pos+1] == 0)
					{
						running = 0;
					}
					else
					{
						for(uint8_t i = pos+1; i < maxLen; ++i)
						{
							string_typed[i] = 0;
						}
					}
				}
				else
				{
					++pos;
					if(string_typed[pos] == 0)
					{
						string_typed[pos] = 127;
					}
				}
			}
		}
		else if(event & EVENT_BUTTON_3)
		{
			char c = string_typed[pos];
			if(c == 127)
			{
				string_typed[pos] = ' ';
			}
			else
			{
				string_typed[pos] = c + 1;
			}
			modified = 1;
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
		if(running)
		{
			program_wait();
		}
	}
	for(uint8_t i = 0; i < maxLen; ++i)
	{
		if(string_typed[i] == 127)
		{
			string_typed[i] = 0;
		}
	}
	return modified;
}

void draw_typing_screen(char* str, uint8_t column)
{
	uint8_t len = strlen(str);
	// Scrolling
	if(column > 15)
	{
		str += column - 15;
		len -= column - 15;
		column = 15;
	}

	draw_clear();
	for(uint8_t i = 0; i < len && (i * 7 + 1) < 128; ++i)
	{
		draw_char(i * 7 + 1,
		#if defined(BIG_FONT)
			2*12,
		#elif defined(SMALL_FONT)
			3*9,
		#endif
		str[i]);
	}
	draw_char_column(column | (1<<4), str[column]);
	draw_update();
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
	#if defined(BIG_FONT)
		x = letter - 2;
	#elif defined(SMALL_FONT)
		x = letter - 3;
	#endif

	y = 0;
	for(;
		#if defined(BIG_FONT)
			x <= letter + 2;
		#elif defined(SMALL_FONT)
			x <= letter + 3;
		#endif
		++x )
	{
		char c = x;

		// Dock mode draw [@-Z]
		if((column_and_flags>>5) & 0x01)
		{
			if(x < '@')
			{
				c += 27;
			}

			else if(x > 'Z')
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
			else if(x > '~'+1)
			{
				c-= 96;
			}
		}
		
		draw_char(((column_and_flags&0xF)*7)+1,
			#if defined(BIG_FONT)
				y*12,
			#elif defined(SMALL_FONT)
				y*9,
			#endif
			 c);
		++y;
	}
	if((column_and_flags>>4) & 0x01)
	{
		for(y = 0; y < 
				#if defined(BIG_FONT)
					11;
				#elif defined(SMALL_FONT)
					 7;
				#endif
			++y)
		{
			draw_h_line((column_and_flags&0xF)*7,
				#if defined(BIG_FONT)
					23+y, 7,
				#elif defined(SMALL_FONT)
					 26+y, 6,
				#endif
				INVERSE);
		}
	}
}
