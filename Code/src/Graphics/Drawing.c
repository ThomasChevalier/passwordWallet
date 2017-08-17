#include "Drawing.h"

#include <string.h>

#include "../Globals.h"

#include "String.h"
#include "font.h"

#include "../Hardware/Oled.h"

#include "../Security/Password.h"
#include "../Security/Password_List.h"
#include "../Security/Security.h"

// For type string
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

// Le caractère nulle doit être transformé en INVALID_CHARACTER
// Retourne la longueur non écrite
uint8_t draw_char(uint8_t x, uint8_t y, uint8_t c)
{
	if(x + FONT_WIDTH >= 128 || y + FONT_HEIGHT >= 64)
	{
		return FONT_WIDTH;
	}
	// Special case for space character.
	if(c == ' ')
	{
		return FONT_WIDTH-2;
	}

	uint8_t i;
	uint8_t space = 0;
	uint8_t searchSpace = 1;
	for(i = FONT_WIDTH; i > 0; --i)
	{
		uint8_t font_byte = pgm_read_byte_near(font + ((c-' ') * FONT_WIDTH) + (i-1));
		if(font_byte != 0 || !searchSpace)
		{
			searchSpace = 0;
			uint8_t j = 0;
			for(j = 0; j < 8; ++j)
			{
				oled_draw_pixel(x+i-1, y+(7-j), (font_byte >> (7-j)) & 0x01);
			}
		}
		else
		{
			++space;
		}
	}
	return space;
}

void draw_flash_string(uint8_t x, uint8_t y, uint16_t str_index)
{
	uint8_t i = 0;
	while(1)
	{
		char c = pgm_read_byte(stringData + str_index + i);
		if(c == 0)
		{
			break;
		}
		uint8_t s = draw_char(x, y, c);
		x += FONT_WIDTH + 1 - s;
		if(x > 128-FONT_WIDTH+1)
		{
			return;
		}
		++i;
	}	
}

void draw_text(uint8_t x, uint8_t y, char *str, uint8_t max)
{
	uint8_t i = 0;
	for(i = 0; i < max; ++i)
	{
		char c = str[i];
		if(c == 0)
		{
			return;
		}
		uint8_t s = draw_char(x, y, str[i]);
		x += FONT_WIDTH + 1 - s;
		if(x > 128-FONT_WIDTH+1)
		{
			return;
		}
	}
}

void draw_num(uint8_t x, uint8_t y, uint16_t num)
{
	char strNum[5] = {0};
	char* ptr = strNum;

	uint16_t shifter = num;
	do{ //Move to where representation ends
		++ptr;
		shifter = shifter/10;
	}while(shifter);
	do{ //Move back, inserting digits as u go
		*--ptr = num%10 + '0';
		num = num/10;
	}while(num);

	draw_text(x, y, strNum, 5);
}


void draw_hex(uint8_t x, uint8_t y, uint8_t* hexBuffer, uint8_t hexBufferSize)
{
	uint8_t index = 0;
	char str[hexBufferSize * 2];
	for(index = hexBufferSize-1; index != (uint8_t)-1; --index)
	{
		uint8_t nibble = hexBuffer[index] >> 4;  // High
		str[index * 2] = (nibble < 10) ? nibble + '0' : (nibble-10) + 'A';
		nibble = hexBuffer[index] & 0x0F;  // Low
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
	draw_browse_dock(0, 0);

	draw_h_line(8, 16, 120, WHITE);
	draw_h_line(8, 48, 120, WHITE);

	if(NUM_PWD != 0)
	{
		char data[64];

		// Draw the previous password
		uint8_t pwd_id = pwd_list_get_prev_pwd_id(CURRENT_PASSWORD_ID);
		password_read_name(pwd_id, (uint8_t*)data);
		draw_text(12, 2 , data, 32);

		pwd_id = CURRENT_PASSWORD_ID;

		password_read_usr_name(pwd_id, (uint8_t*)data, KEY);
		if(data[0] != 0) // If an username is defined
		{
			draw_text(12, 35, data, 64);

			password_read_name(pwd_id, (uint8_t*)data);
			draw_text(12, 20, data, 32);
		}
		else
		{
			password_read_name(pwd_id, (uint8_t*)data);
			draw_text(12, 28, data, 32);
		}

		if(pwd_list_get_sorting_method() == PWD_SORTING_USE)
		{
			draw_h_line(99, 28, 28, WHITE);
			draw_v_line(99, 16, 12, WHITE);
			for(uint8_t i = 0; i < 10; ++i)
			{
				draw_h_line(100, 17+i, 27, BLACK);
			}

			uint16_t count = password_read_counter(pwd_id);
			draw_num(102, 18, count);
		}

		// Draw the next password
		pwd_id = pwd_list_get_next_pwd_id(CURRENT_PASSWORD_ID);
		password_read_name(pwd_id, (uint8_t*)data);
		draw_text(12, 52, data, 32);

		security_erase_data(data, 64);
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

	draw_char_column((highlight << 4) | (1 << 5), letter);
	draw_v_line(8, 0, 64, WHITE);
}

uint8_t type_string(char* string_typed, uint8_t maxLen)
{
	uint8_t pos = 0;
	uint8_t running = TRUE;
	uint8_t modified = FALSE;
	string_typed[strlen(string_typed)] = INVALID_CHARACTER;
	draw_typing_screen(string_typed, pos, maxLen);

	program_wait();

	while(running)
	{
		program_update();
		uint8_t event = events_get();  // Mask of events

		if(event & EVENT_BUTTON_1)
		{
			char c = string_typed[pos];
			if(c == ' ')
			{
				string_typed[pos] = INVALID_CHARACTER;
			}
			else
			{
				string_typed[pos] = c - 1;
			}
			modified = TRUE;
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
				if(string_typed[pos] == INVALID_CHARACTER)
				{
					// If there is nothing to erase, then quit
					// Access to pos+1 is safe because we have checked before
					// that the maximum size has not been reached.
					if(string_typed[pos+1] == 0)
					{
						running = 0;
					}
					// If there is something to erase then erase it
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
						string_typed[pos] = INVALID_CHARACTER;
					}
				}
			}
		}
		else if(event & EVENT_BUTTON_3)
		{
			char c = string_typed[pos];
			if(c == INVALID_CHARACTER)
			{
				string_typed[pos] = ' ';
			}
			else
			{
				string_typed[pos] = c + 1;
			}
			modified = TRUE;
		}
		else if(event & EVENT_BUTTON_4)
		{
			if(string_typed[pos] == INVALID_CHARACTER)
			{
				for(uint8_t i = pos+1; i < maxLen; ++i)
				{
					string_typed[i] = 0;
				}
			}

			if(pos > 0)
			{
				--pos;
			}
			else
			{
				running = 0;
			}
		}
		if(event)
		{
			draw_typing_screen(string_typed, pos, maxLen);
		}
		if(running)
		{
			program_wait();
		}
	}

	for(uint8_t i = 0; i < maxLen; ++i)
	{
		if(string_typed[i] == INVALID_CHARACTER)
		{
			string_typed[i] = 0;
		}
	}
	return modified;
}

void draw_typing_screen(char* str, uint8_t column, uint8_t max)
{
	uint8_t len = strlen_bound(str, max);
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
		draw_char(i * 7 + 1, 2*12, str[i]);
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
			oled_draw_pixel(x, y, BLACK);
		}
	}
	x = letter - 2;

	y = 0;
	for(; x <= letter + 2; ++x )
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
		// Full mode draw [' '-'~'[
		else
		{
			if(x < ' ')
			{
				c += INVALID_CHARACTER - ' ' + 1;
			}
			else if(x > INVALID_CHARACTER)
			{
				c-= INVALID_CHARACTER - ' ' + 1;
			}
		}

		draw_char(((column_and_flags&0xF)*7)+1, y*12, c);
		++y;
	}
	if((column_and_flags>>4) & 0x01)
	{
		for(y = 0; y < 11; ++y)
		{
			draw_h_line((column_and_flags&0xF)*7, 23+y, 7, INVERSE);
		}
	}
}
