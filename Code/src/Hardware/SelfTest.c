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

static uint8_t failOccured;

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
		draw_flash_str(0, 0, str_self_test_fram_fail);
		failOccured = TRUE;
		draw_update();
		led_blink(3);
	}
	else
	{
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
		uint8_t y = 0;
		if(failOccured){
			y = 10;
		}
		draw_flash_str(0, y, str_self_test_rfid_fail);
		failOccured = TRUE;
		draw_update();
		led_blink(3);
	}
	else
	{
		led_blink(2);
	}
	_delay_ms(500);
}

static void test_keyboard(void)
{
	for(char c = ' '; c < '~'; ++c)
	{
		// This copy is needed since keyboard_send can change the parameter value.
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
		draw_flash_str(0, 0, str_self_test_fail);
		draw_update();
		_delay_ms(3000);
		self_test_execute();
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

void self_test_execute(void)
{
	failOccured = FALSE;

	DISABLE_SLEEP();
	led_blink(5);
	test_oled();
	test_fram();
	test_rfid();
	if(failOccured)
	{
		draw_flash_str(0, 40, str_self_test_fail);
	}
	else
	{
		draw_flash_str(0, 0, str_self_test_good);
	}
	draw_update();
	test_keyboard();
	led_blink(5);
	ENABLE_SLEEP();
}
