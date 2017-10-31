#include "Commands.h"

#include "../USB/Serial.h"
#include "../Globals.h"
#include "../Hardware/Fram.h"
#include "../Hardware/Led.h"

#include "../System/Sleep.h"

#include "../Hardware/Oled.h"
#include "../Graphics/Drawing.h"

#include "../Security/Encryption.h"

static uint16_t COM_POS = 0;
static uint16_t COM_PARAMETER = 0;

Command CURRENT_COMMAND;

static void send_command(uint8_t id, uint16_t size, uint8_t *data)
{
	serial_send(&id, 1);
	serial_send((uint8_t*)(&size), 2);
	uint16_t i = 0;
	for(; i < size; i+=255)
	{
		size -= i;
		serial_send(data, size % 255);
	}
}
 
#define COM_LOGO_X (120)
#define COM_LOGO_Y (0)

static uint8_t save_logo[8];

static void show_com_logo(void)
{
	static const __flash uint8_t com_logo[] =
	{
		0x28, 0x2C, 0x2E, 0x28, 0x28, 0xE8, 0x68, 0x28
	};

	// The following lines save the column of pixel before update
	// This works because COM_LOGO_Y % 8 == 0
	uint8_t i = 0;
	uint16_t indice = (COM_LOGO_X /* +i */) + (COM_LOGO_Y/8)*SSD1306_LCDWIDTH;
	for(; i < 8; ++i)
	{
		save_logo[i] = oled_data[indice];
		oled_data[indice] = com_logo[i];
		++indice;
	}
	
	oled_display();
}

static void hide_com_logo(void)
{
	uint8_t i = 0;

	uint16_t indice = (COM_LOGO_X /* +i */) + (COM_LOGO_Y/8)*SSD1306_LCDWIDTH;
	for(; i < 8; ++i)
	{
		oled_data[indice] = save_logo[i];
		++indice;
	}
	
	oled_display();
}

void com_exec(void)
{
	const uint8_t id = CURRENT_COMMAND.id;

	// Special priority
	if(id == COM_GET_VERSION)
	{
		command_get_version();
		return;
	}

	if( (!(GLOBALS_EVENTS & EVENT_FLAG_COM)) && (id != COM_INIT))
	{
		send_command(COM_ERR_NOT_INIT, 0, 0);
		return;
	}

	show_com_logo();

	switch(id)
	{
	case COM_INIT:
		command_init();
		break;
	case COM_END:
		command_end();
		break;
	case COM_GET_FRAM :
		command_get_fram();
		break;
	case COM_GET_KEY :
		command_get_key();
		break;
	case COM_GET_PARAM :
		command_get_param();
		break;
	case COM_SET_FRAM :
		command_set_fram();
		break;
	case COM_SET_KEY :
		command_set_key();
		break;
	}

	hide_com_logo();
}

void com_abort(void)
{
	send_command(COM_TIMEOUT, 0, 0);
	GLOBALS_EVENTS &= ~EVENT_FLAG_COM;
	COM_POS = 0; 
	COM_PARAMETER = 0;
}

void command_init()
{
	events_happen(EVENT_FLAG_COM);
	send_command(COM_OK, 0, 0);
}

void command_end()
{
	GLOBALS_EVENTS &= ~EVENT_FLAG_COM;
	send_command(COM_END, 0, 0);
}

void command_get_fram()
{
	uint8_t id = COM_FRAM;
	serial_send(&id, 1);
	uint16_t size = FRAM_BYTE_SIZE;
	serial_send((uint8_t*)&size, 2);

	uint8_t buffer[16];
	for(uint16_t i = 0; i < FRAM_BYTE_SIZE / 16; ++i)
	{
		fram_read_bytes(i*16, buffer, 16);
		serial_send(buffer, 16);
	}
}

void command_get_key()
{
	if(GLOBALS_EVENTS & EVENT_KEY_ENTERED){
		send_command(COM_KEY, 16, KEY);
	}
	else{
		send_command(COM_ERR_UNAVAILABLE, 0, 0);
	}
}

void command_get_param()
{
	uint8_t id = COM_PARAM;
	serial_send(&id, 1);
	uint16_t size = 6;
	serial_send((uint8_t*)&size, 2);

	Fram_id fid = fram_read_id();
	uint8_t buffer[6];
	#if defined(SPI_FRAM)
	buffer[0] = fid.manufacturer_id;
	buffer[1] = fid.continuation_code;
	buffer[2] = fid.product_idL;
	buffer[3] = fid.product_idH;
	#elif defined(I2C_FRAM)
	buffer[0] = fid.manufacturer_id >> 8;
	buffer[1] = fid.manufacturer_id & 0xFF;
	buffer[2] = fid.product_id & 0xFF;
	buffer[3] = fid.product_id >> 8;
	#endif

	#if defined(STORE_SCREEN_BUFFER_IN_FRAM)
	buffer[4] = 0x01;
	#else
	buffer[4] = 0;
	#endif

	buffer[5] = SOFTWARE_VERSION;

	serial_send(buffer, size);
}

void command_get_version(void)
{
	uint8_t ver = SOFTWARE_VERSION;
	send_command(COM_VERSION, 1, &ver);
}

void command_set_fram()
{
	if(COM_POS == 0 && CURRENT_COMMAND.availableSize != 0){
		COM_PARAMETER = CURRENT_COMMAND.data[0];
		++CURRENT_COMMAND.data;
		--CURRENT_COMMAND.availableSize;
		++COM_POS;
	}
	if(COM_POS == 1 && CURRENT_COMMAND.availableSize != 0){
		COM_PARAMETER |= CURRENT_COMMAND.data[0] << 8;
		++CURRENT_COMMAND.data;
		--CURRENT_COMMAND.availableSize;
		++COM_POS;
	}
	if(COM_POS > 1){
		fram_write_bytes(COM_PARAMETER + COM_POS - 2, CURRENT_COMMAND.data, CURRENT_COMMAND.availableSize);
		COM_POS += CURRENT_COMMAND.availableSize;
		CURRENT_COMMAND.availableSize = 0;
	}

	if(COM_POS == CURRENT_COMMAND.totalSize)
	{
		COM_POS = 0;
		send_command(COM_OK, 0, 0);
	}
}

void command_set_key()
{
	for(uint8_t i = 0; i < CURRENT_COMMAND.availableSize; ++i){
		KEY[COM_POS + i] = CURRENT_COMMAND.data[i];
	}
	COM_POS += CURRENT_COMMAND.availableSize;
	CURRENT_COMMAND.availableSize = 0;
	if(COM_POS == CURRENT_COMMAND.totalSize){
		COM_POS = 0;
		if(encryption_check_key() == RETURN_SUCCESS){
			events_happen(EVENT_KEY_ENTERED);
			send_command(COM_OK, 0, 0);
		}
		else{
			send_command(COM_BAD_KEY, 0, 0);
		}
	}
}
