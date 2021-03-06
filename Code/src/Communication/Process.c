#include "Process.h"

#include "../Globals.h"

#include "Commands.h"


#define COM_ID    (0)             // 0
#define COM_SIZE1 (COM_ID + 1)    // 1
#define COM_SIZE2 (COM_SIZE1 + 1) // 2
#define COM_SIZE3 (COM_SIZE2 + 1) // 3
#define COM_DATA  (COM_SIZE3 + 1) // 4

static uint8_t com_state = COM_ID;


static uint8_t pick(uint8_t **data, uint8_t* lenght)
{
	--(*lenght);
	const uint8_t b = **data;
	++(*data);
	return b;
}

void com_process_data(uint8_t* data, uint8_t lenght)
{
	static uint32_t com_bytes_processed;

	// If we are waiting for a command we don't want to timeout
	if(SERIAL_TIMEOUT_TIMER == SERIAL_TIMEOUT && com_state != COM_ID){
		com_abort();
		com_state = COM_ID;
	}
	SERIAL_TIMEOUT_TIMER = 0;

	do
	{
		// Initial state read the command ID
		if(com_state == COM_ID)
		{
			CURRENT_COMMAND.id = pick(&data, &lenght);
			++com_state;
		}
		// Read the first byte of the command size.
		if(com_state == COM_SIZE1 && lenght)
		{
			CURRENT_COMMAND.totalSize = pick(&data, &lenght);
			++com_state;
		}
		// Read the second byte of the command size.
		if(com_state == COM_SIZE2 && lenght)
		{
			CURRENT_COMMAND.totalSize |= (uint32_t)pick(&data, &lenght) << 8;
			++com_state;
		}
		// Read the third byte of the command size
		if(com_state == COM_SIZE3 && lenght)
		{
			CURRENT_COMMAND.totalSize |= (uint32_t)pick(&data, &lenght) << 16;
			com_bytes_processed = 0;
			++com_state;
		}

		// Process the data of the command
		if(com_state == COM_DATA)
		{
			CURRENT_COMMAND.data = data;

			// Data available from serial belong completely to the current command :
			if(lenght < CURRENT_COMMAND.totalSize - com_bytes_processed){
				CURRENT_COMMAND.availableSize = lenght;
				com_bytes_processed += lenght;
				lenght = 0;
			}
			// A fraction of the data available from the serial belong to the current command :
			else
			{
				// The available size contains the remaining bytes
				CURRENT_COMMAND.availableSize = (CURRENT_COMMAND.totalSize - com_bytes_processed);
				lenght -= CURRENT_COMMAND.availableSize;
				// We have processed the full command
				com_bytes_processed = CURRENT_COMMAND.totalSize;

				com_state = COM_ID; // Look for a new command
			}

			// This executes the current command even if data are not received entirely.
			// It will be executed as many times as needed, as if several commands were sent, 
			// each with partial data.
			com_exec();
		}
	}while(lenght);
}
