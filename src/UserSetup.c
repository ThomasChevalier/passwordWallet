#include "UserSetup.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Globals.h"
#include "Fram.h"
#include "Buttons.h"
#include "Oled.h"
#include "String.h"
#include "ProgressBar.h"
#include "Authentification.h"
#include "Events.h"
#include "Events.h"
#include "Drawing.h"
#include "Ascii85.h"
#include "Rfid.h"

uint8_t usr_setup_is_initialized()
{
	const uint8_t optFlag = fram_read_byte(OFFSET_OPTIONS_FLAG);
	if((optFlag & (1<<OFFSET_OPTION_INITIALIZED)) == 0)
		return 0;
	if(buttons_pressed() == 0x0F)
	{
		return 0;
	}
	return 1;
}

uint8_t usr_setup_do_initialization()
{
	{
		oled_clear_display();
		char str_buf[100];
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_welcome_index])));
		oled_draw_text(0, 2, str_buf, 0);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_note_index])));
		oled_draw_text(90, 18, str_buf, 62);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_warning_index])));
		oled_draw_text(54, 34, str_buf, 45);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_confirm_index])));
		oled_draw_text(0, 48, str_buf, 65);
		oled_display();

		_delay_ms(5000);

		while(buttons_pressed() == 0);
		if(buttons_pressed() != 0x01)
			return 0;
		while(buttons_pressed() == 0x01 ||buttons_pressed() == 0x00);
		if(!(buttons_pressed() & 0x04))
			return 0;
		while(buttons_pressed() == 0x04 ||buttons_pressed() == 0x00);
		if(!(buttons_pressed() & 0x08))
			return 0;
		while(buttons_pressed() == 0x08 ||buttons_pressed() == 0x00);
		if(!(buttons_pressed() & 0x02))
			return 0;

		 // Ask for card
	    oled_clear_display();
	    str_to_buffer(str_misc_approachCard_index);
	    oled_draw_text(18, 30, str_buffer, 0);
	    oled_display();

	    // rfid may be power down
    	rfid_init();
	    // Waiting for the user to present his card
	    wait_for_valid_card();
	    rfid_power_down();
		
	}
	

	// Erase all fram memory
	oled_clear_display();
	str_to_buffer(str_usrsetup_eraseMem_index);
	oled_draw_text(30, 40, str_buffer, 0);
	oled_display();

	progress_begin(FRAM_BYTE_SIZE/64);
	uint8_t zeroBuf[64] = {0};

	for(uint16_t i = 0; i < FRAM_BYTE_SIZE/64; ++i)
	{
		fram_write_bytes(i*64, zeroBuf, 64);
		progress_add(1);
	}
	progress_end();

  	// Get a master key
  	change_master_key();

  	// Device is now initialized !
  	fram_write_byte(OFFSET_OPTIONS_FLAG, (1<<OFFSET_OPTION_INITIALIZED));

  	// Force the next state
  	eventHappen(EVENT_PASSWORD_ENTERED);
  	return 1;

}

uint8_t usr_setup_check_card_lost()
{
	// Button up and down
	return (buttons_pressed() == 0x05) ? 1 : 0;
}

uint8_t usr_setup_recover_key()
{
	_delay_ms(1000);
	char usrKey[21] = {0};
	type_string(usrKey, 20);
	decode_16B(usrKey, KEY);
	oled_clear_display();
	oled_display();
	_delay_ms(1000);
	oled_draw_text(0,0, usrKey, 0);
	oled_display();
	_delay_ms(10000);
	if(check_key())
	{
		eventHappen(EVENT_PASSWORD_ENTERED);
		return 1;
	}
	return 0;
}
