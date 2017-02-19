#include "SelfTest.h"

#include <util/delay.h>

#include "../Globals.h"

#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"

#include "Oled.h"
#include "Buttons.h"
#include "Rfid.h"
#include "Fram.h"
#include "Keyboard.h"

static void test_oled(void)
{
	blink(2);
	oled_init();
	// Test pixels
	for(uint8_t y = 0; y < SSD1306_LCDHEIGHT; ++y)
	{
		for(uint8_t x = 0; x < SSD1306_LCDWIDTH; ++x)
		{
			oled_draw_pixel(x, y, WHITE);
		}
	}
	oled_display();
	_delay_ms(2000);

	oled_dim(1);
	_delay_ms(2000);

	oled_dim(0);
	oled_clear_display();
	oled_display();
	_delay_ms(2000);
}

static void test_fram(void)
{
	uint16_t badByte = 0;
	for(uint16_t i = 0; i < FRAM_BYTE_SIZE; ++i)
	{
		uint8_t value = fram_read_byte(i);
		fram_write_byte(i, 0xAA);
		uint8_t verif = fram_read_byte(i);
		if(verif != 0xAA)
		{
			badByte = 1;
			break;
		}
		else
		{
			fram_write_byte(i, 0x55);
			verif = fram_read_byte(i);
			if(verif != 0x55)
			{
				badByte = 1;
				break;
			}
		}
		fram_write_byte(i, value);
	}
	if(badByte)
	{
		str_to_buffer(str_self_test_fram_fail_index);
		draw_text(0, 0, str_buffer, 0);
		draw_hex(100, 0, (uint8_t*)&badByte, 2);
		draw_update();
	}
	else
	{
		str_to_buffer(str_self_test_fram_ok_index);
		draw_text(0, 0, str_buffer, 0);
		draw_hex(100, 0, (uint8_t*)&badByte, 2);
		draw_update();
	}
	
	_delay_ms(5000);
}

static void test_rfid(void)
{
	blink(2);
	if(rfid_pcd_perform_self_test() == 0)
	{
		str_to_buffer(str_self_test_rfid_fail_index);
		draw_text(0, 10, str_buffer, 0);
		draw_update();
	}
	else
	{
		str_to_buffer(str_self_test_rfid_ok_index);
		draw_text(0, 10, str_buffer, 0);
		draw_update();
	}
	_delay_ms(5000);
}

static void test_buttons(void)
{
	blink(2);
	for(uint8_t j = 0; j < 4; ++j)
	{
		uint8_t ok = 0;
		for(uint8_t i = 0; i < 100; ++i)
		{
			if(buttons_pressed() & (1<<j))
			{
				str_to_buffer(str_self_test_button_ok_index);
				draw_text(0, 20 + 8 * j, str_buffer, 0);
				draw_update();
				ok = 1;
				break;
			}
			_delay_ms(50);
		}
		if(ok == 0)
		{
			str_to_buffer(str_self_test_button_fail_index);
			draw_text(0, 20 + 8 * j, str_buffer, 0);
			draw_update();
		}
	}
}

static void test_keyboard(void)
{
	blink(2);
	for(char c = ' '; c < '~'+1; ++c)
	{
		keyboard_send(&c, 1);
	}
}
void self_test_execute(void)
{
	blink(5);
	_delay_ms(5000);
	test_oled();
	test_fram();
	test_rfid();
	test_buttons();
	test_keyboard();
}