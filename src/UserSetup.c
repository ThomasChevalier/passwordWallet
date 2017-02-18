#include "UserSetup.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Globals.h"

#include "Hardware/Fram.h"
#include "Hardware/Buttons.h"
#include "Hardware/Rfid.h"

#include "Graphics/String.h"
#include "Graphics/ProgressBar.h"
#include "Graphics/Drawing.h"
#include "Graphics/Ascii85.h"

#include "Security/Authentification.h"

#include "FSM/Events.h"



uint8_t usr_setup_is_initialized(void)
{
	const uint8_t optFlag = fram_read_byte(OFFSET_OPTIONS_FLAG);
	if((optFlag & (1<<OPTIONS_FLAG_OFFSET_INITIALIZED)) == 0)
	{
		return 0;
	}
	if(buttons_pressed() == 0x0F)
	{
		return 0;
	}
	return 1;
}

uint8_t usr_setup_do_initialization(void)
{
	{
		draw_clear();
		char str_buf[100];
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_welcome_index])));
		draw_text(0, 2, str_buf, 0);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_note_index])));
		draw_text(90, 18, str_buf, 62);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_warning_index])));
		draw_text(54, 34, str_buf, 45);
		strcpy_P(str_buf, (PGM_P)pgm_read_word(&(string_table[str_usrsetup_confirm_index])));
		draw_text(0, 48, str_buf, 65);
		draw_update();

		_delay_ms(5000);

		// uint8_t buttList[4] = {0x01, 0x04, 0x08, 0x02};
		// while(buttons_pressed() == 0);
		// if(buttons_pressed() != 0x01)
		// 	return 0;
		// for(uint8_t i = 1; i < 4; ++i)
		// {
		// 	while(buttons_pressed() == 0x00 || buttons_pressed() == buttList[i-1]);
		// 	if(!(buttons_pressed() & buttList[i]))
		// 		return 0;
		// }
		
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
	    draw_clear();
	    str_to_buffer(str_misc_approachCard_index);
	    draw_text(18, 30, str_buffer, 0);
	    draw_update();
	}
	

	// Erase all fram memory
	draw_clear();
	str_to_buffer(str_usrsetup_eraseMem_index);
	draw_text(30, 40, str_buffer, 0);
	draw_update();

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
  	fram_write_byte(OFFSET_OPTIONS_FLAG, (1<<OPTIONS_FLAG_OFFSET_INITIALIZED));

  	// Force the next state
  	events_happen(EVENT_PASSWORD_ENTERED);
  	return 1;

}

uint8_t usr_setup_check_card_lost(void)
{
	// Button up and down
	return (buttons_pressed() == 0x05) ? 1 : 0;
}

uint8_t usr_setup_recover_key(void)
{
	_delay_ms(1000);
	char usrKey[21] = {0};
	type_string(usrKey, 20);
	decode_16B(usrKey, KEY);
	draw_clear();
	draw_update();
	_delay_ms(1000);
	draw_text(0,0, usrKey, 0);
	draw_update();
	_delay_ms(10000);
	if(check_key())
	{
		events_happen(EVENT_PASSWORD_ENTERED);
		return 1;
	}
	return 0;
}
