#include "Serial.h"

#include "../Globals.h"
#include "../FSM/Events.h"

#include "../Graphics/Drawing.h"

#include "../FSM/States/State_Communication.h"

#ifdef SERIAL_ENABLE
/** Contains the current baud rate and other settings of the virtual serial port. While this demo does not use
 *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
 *  upon request or the host will assume the device is non-functional.
 *
 *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
 *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
 *  serial link characteristics and instead sends and receives data in endpoint streams.
 */
static CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 0,
                                           .CharFormat  = CDC_LINEENCODING_OneStopBit,
                                           .ParityType  = CDC_PARITY_None,
                                           .DataBits    = 8                            };

static uint8_t *send_buffer_ptr;
static uint8_t send_buffer_lenght;

#endif

void serial_init (void)
{

}

void serial_loop (void)
{
	#ifdef SERIAL_ENABLE
	CDC_Task();
	#endif
}

void serial_send (uint8_t* buffer, uint8_t lenght)
{
	#ifdef SERIAL_ENABLE
	send_buffer_ptr = buffer;
	send_buffer_lenght = lenght;
	while(send_buffer_ptr != NULL)
	{
		serial_loop();
		USB_USBTask();
	}
	#endif
}

void serial_on_device_connect (void)
{

}

void serial_on_device_disconnect (void)
{

}

uint8_t serial_on_configuration_changed (void)
{
	#ifdef SERIAL_ENABLE

	uint8_t ConfigSuccess = 1;
	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);

	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;

	return ConfigSuccess;
	#else
	return 1;
	#endif
}

void serial_on_control_request (void)
{
	#ifdef SERIAL_ENABLE
/* Process CDC specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		case CDC_REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearOUT();
			}

			break;
		case CDC_REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearIN();
			}

			break;
		case CDC_REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				/* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
				         lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
						 CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
				*/
			}

			break;
	}
	#endif
}

void serial_on_start_of_frame (void)
{

}


#ifdef SERIAL_ENABLE

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	if (send_buffer_ptr != NULL && LineEncoding.BaudRateBPS)
	{

		/* Select the Serial Tx Endpoint */
		Endpoint_SelectEndpoint(CDC_TX_EPADDR);

		/* Write the data to the Endpoint */
		Endpoint_Write_Stream_LE(send_buffer_ptr, send_buffer_lenght, NULL);

		send_buffer_ptr = NULL;
		send_buffer_lenght = 0;

		/* Remember if the packet to send completely fills the endpoint */
		bool IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* If the last packet filled the endpoint, send an empty packet to release the buffer on
		 * the receiver (otherwise all data will be cached until a non-full packet is received) */
		if (IsFull)
		{
			/* Wait until the endpoint is ready for another packet */
			Endpoint_WaitUntilReady();

			/* Send an empty packet to ensure that the host does not buffer data sent to it */
			Endpoint_ClearIN();
		}
	}

	/* Select the Serial Rx Endpoint */
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	/* Get data from the host */
	if (Endpoint_IsOUTReceived())
	{
		/* Create a temp buffer big enough to hold the incoming endpoint packet */
		uint8_t  buffer[Endpoint_BytesInEndpoint()];

		/* Remember how large the incoming packet is */
		uint16_t dataLength = Endpoint_BytesInEndpoint();

		/* Read in the incoming packet into the buffer */
		Endpoint_Read_Stream_LE(&buffer, dataLength, NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearOUT();

		state_communication_process_data(buffer, dataLength);
	}
}



#endif // SERIAL_ENABLE