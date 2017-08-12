#include "State_Communication.h"

#include <string.h>

#include "../StatesDefine.h"
#include "../Events.h"

#include "../../Graphics/Drawing.h"
#include "../../Graphics/String.h"

#include "../../USB/Serial.h"

#include "../../Hardware/Fram.h"

#include "../../Program/Program.h"
#include "../../Security/Encryption.h"
#include "../Security/Password_List.h"

// Flag for communication state
#define FLAG_INITED (1<<0)

#define FLAG_SEND_FRAM_DATA (1<<1)
#define FLAG_WAIT_FRAM_DATA (1<<2)

#define FLAG_SEND_KEY_DATA (1<<3)
#define FLAG_WAIT_KEY_DATA (1<<4)

#define FLAG_SEND_FRAM_ID (1<<5)

#define FLAG_END_COMMUNICATION (1<<6)

static uint8_t communication_flag = 0;

static void do_fram_dump(void)
{
	uint8_t buffer[16];
	for(uint16_t i = 0; i < FRAM_BYTE_SIZE / 16 && (communication_flag & FLAG_END_COMMUNICATION) == 0; ++i)
	{
		fram_read_bytes(i*16, buffer, 16);
		serial_send(buffer, 16);
	}
}

static void do_send_key(void)
{
	serial_send(KEY, 16);
}

static void do_send_framId(void)
{
	Fram_id id = fram_read_id();
	uint8_t buffer[5];
	#if defined(SPI_FRAM)
	buffer[0] = id.manufacturer_id;
	buffer[1] = id.continuation_code;
	buffer[2] = id.product_idL;
	buffer[3] = id.product_idH;
	#elif defined(I2C_FRAM)
	buffer[0] = id.manufacturer_id >> 8;
	buffer[1] = id.manufacturer_id & 0xFF;
	buffer[2] = id.product_id & 0xFF;
	buffer[3] = id.product_id >> 8;
	#endif

	#if defined(STORE_SCREEN_BUFFER_IN_FRAM)
	buffer[4] = 0x01;
	#else
	buffer[4] = 0;
	#endif
	serial_send(buffer, 5);
}

void state_communication_process_data(uint8_t* buffer, uint8_t lenght)
{
	static uint16_t data_received = 0;

	if(lenght == 1 && buffer[0] == 0xA5 && (communication_flag & FLAG_INITED) == 0)
	{
		events_happen(EVENT_INIT_COMMUNICATION);
	}
	if((communication_flag & FLAG_INITED) == 0 || (communication_flag & FLAG_END_COMMUNICATION)==FLAG_END_COMMUNICATION)
		return;

	// Command
	if((communication_flag&FLAG_WAIT_FRAM_DATA) == 0 && (communication_flag&FLAG_WAIT_KEY_DATA) == 0 )
	{
		uint8_t byte = buffer[0];
		if(byte == 0x5A && (communication_flag & FLAG_INITED) == 1)
		{
			communication_flag |= FLAG_END_COMMUNICATION;
		}
		else if(byte == 0x01)
		{
			communication_flag |= FLAG_SEND_FRAM_DATA;
		}
		else if(byte == 0x02)
		{
			communication_flag |= FLAG_WAIT_FRAM_DATA;
			data_received = 0;
		}
		else if(byte == 0x03)
		{
			communication_flag |= FLAG_SEND_KEY_DATA;
		}
		else if(byte == 0x04)
		{
			communication_flag |= FLAG_WAIT_KEY_DATA;
			data_received = 0;
		}
		else if(byte == 0x05)
		{
			communication_flag |= FLAG_SEND_FRAM_ID;
		}
		else
		{
			// Unknown command
		}
		++buffer; 
		--lenght;
	}
	if(lenght == 0)
	{
		return;
	}

	// We are waiting for fram data (set fram command)
	if(communication_flag & FLAG_WAIT_FRAM_DATA)
	{
		if(data_received + lenght <= FRAM_BYTE_SIZE)
		{
			fram_write_bytes(data_received, buffer, lenght);
			data_received += lenght;
		}
		else
		{
			fram_write_bytes(data_received, buffer, FRAM_BYTE_SIZE - data_received);
			data_received = FRAM_BYTE_SIZE;
		}
		// End of data
		if(data_received == FRAM_BYTE_SIZE)
		{
			communication_flag &= ~FLAG_WAIT_FRAM_DATA;
			program_init(); // Read the variable stored in fram
		}
		return;
	}

	// We are waiting key data
	if(communication_flag & FLAG_WAIT_KEY_DATA)
	{
		if(data_received + lenght <= 16)
		{
			memcpy(KEY+data_received, buffer, lenght);
			data_received += lenght;
		}
		else
		{
			memcpy(KEY+data_received, buffer, 16-data_received);
			data_received = 16;
		}
		if(data_received == 16)
		{
			communication_flag &= ~FLAG_WAIT_KEY_DATA;
		}
		return;
	}

}

void state_communication_begin(void)
{
	DISABLE_SLEEP();

	communication_flag = FLAG_INITED;

	draw_clear();
	draw_text_index(5, 0, str_communication_what_index);
	draw_text_index(10, 10, str_communication_dont_unplug_index);
	draw_update();
}

uint8_t state_communication_transition(uint8_t event)
{
	if(communication_flag & FLAG_END_COMMUNICATION)
	{
		draw_clear();
		program_init();
		if(encryption_check_key())
		{
			// If the fram has been modified, the list must be resorted
			pwd_list_sort_use();
			pwd_list_sort_alpha();
			return STATE_MAIN;
		}
		else
		{
			return STATE_WAIT_CARD;
		}
	}

	if(communication_flag & FLAG_SEND_FRAM_DATA)
	{
		do_fram_dump();
		communication_flag &= ~FLAG_SEND_FRAM_DATA;
	}
	else if(communication_flag & FLAG_SEND_KEY_DATA)
	{
		do_send_key();
		communication_flag &= ~FLAG_SEND_KEY_DATA;
	}
	else if(communication_flag & FLAG_SEND_FRAM_ID)
	{
		do_send_framId();
		communication_flag &= ~FLAG_SEND_FRAM_ID;
	}


	return STATE_COMMUNICATION;
}

void state_communication_end(void)
{
	communication_flag &= ~FLAG_INITED;
	ENABLE_SLEEP();
}
