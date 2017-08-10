#include "SelfTest.h"

#include <util/delay.h>

#include "../Globals.h"

#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"

#include "../System/Sleep.h"

#include "Oled.h"
#include "Buttons.h"
#include "Rfid.h"
#include "Fram.h"
#include "../USB/Keyboard.h"
#include "Led.h"

static void test_oled(void)
{
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

	oled_clear_display();
	oled_display();
}

static void test_fram(void)
{
	if(fram_test() == RETURN_ERROR)
	{
		draw_text_index(0, 0, str_self_test_fram_fail_index);
		draw_update();
		led_blink(3);
	}
	else
	{
		draw_text_index(0, 0, str_self_test_fram_ok_index);
		draw_update();
		led_blink(2);
	}
	_delay_ms(500);
}

static void test_rfid(void)
{
	rfid_init();
	uint8_t ret = rfid_pcd_perform_self_test();
	rfid_power_down();
	if(ret == RETURN_ERROR)
	{
		draw_text_index(0, 10, str_self_test_rfid_fail_index);
		draw_update();
		led_blink(3);
	}
	else
	{
		draw_text_index(0, 10, str_self_test_rfid_ok_index);
		draw_update();
		led_blink(2);
	}
	_delay_ms(500);
}

static void test_buttons(void)
{
	led_blink(1);
	for(uint8_t j = 0; j < 4; ++j)
	{
		uint8_t ok = 0;
		for(uint8_t i = 0; i < 100; ++i)
		{
			if(buttons_pressed() & (1<<j))
			{
				draw_text_index(0, 20 + 8 * j, str_self_test_button_ok_index);
				draw_update();
				led_blink(2);
				ok = 1;
				break;
			}
			_delay_ms(50);
		}
		if(ok == 0)
		{
			draw_text_index(0, 20 + 8 * j, str_self_test_button_fail_index);
			draw_update();
			led_blink(3);
		}
	}
}

static void test_keyboard(void)
{
	for(char c = ' '; c < '~'+1; ++c)
	{
		char d = c;
		keyboard_send(&d, 1);
	}
}

// Assume that all is initialized
uint8_t self_test_check (void)
{

	// There is someting wrong, that prevent the device to work normally
	if(fram_test() == RETURN_ERROR || rfid_pcd_perform_self_test() == RETURN_ERROR)
	{
		DISABLE_SLEEP();
		draw_clear();
		draw_text_index(0, 0, str_self_test_fail_index);
		draw_update();
		_delay_ms(3000);
		self_test_execute();
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

void self_test_execute(void)
{
	DISABLE_SLEEP();
	led_blink(5);
	test_oled();
	test_fram();
	test_rfid();
	test_buttons();
	test_keyboard();
	led_blink(5);
	ENABLE_SLEEP();
}
