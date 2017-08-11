#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#include "Descriptors.h"

#include "Globals.h"

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN_NUM 5

#define BUTTON_1_PIN_NUM 7
#define BUTTON_1_DDR DDRB
#define BUTTON_1_PIN PINB
#define BUTTON_1_PORT PORTB

#define BUTTON_2_PIN_NUM 5
#define BUTTON_2_DDR DDRB
#define BUTTON_2_PIN PINB
#define BUTTON_2_PORT PORTB

// KEYBOARD

#ifdef KEYBOARD_ENABLE
/** Indicates what report mode the host has requested, true for normal HID reporting mode, \c false for special boot
 *  protocol reporting mode.
 */
static uint8_t UsingReportProtocol = 1;

/** Current Idle period. This is set by the host via a Set Idle HID class request to silence the device's reports
 *  for either the entire idle duration, or until the report status changes (e.g. the user presses a key).
 */
static uint16_t IdleCount = 500;

/** Current Idle period remaining. When the IdleCount value is set, this tracks the remaining number of idle
 *  milliseconds. This is separate to the IdleCount timer and is incremented and compared as the host may request
 *  the current idle period via a Get Idle HID class request, thus its value must be preserved.
 */

static uint16_t IdleMSRemaining = 0;
#endif // KEYBOARD_ENABLE

// SERIAL
#ifdef SERIAL_ENABLE
static CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 0,
                                           .CharFormat  = CDC_LINEENCODING_OneStopBit,
                                           .ParityType  = CDC_PARITY_None,
                                           .DataBits    = 8                            };
#endif // SERIAL_ENABLE

void EVENT_USB_Device_Connect (void);
void EVENT_USB_Device_Disconnect (void);
void EVENT_USB_Device_ConfigurationChanged (void);
void EVENT_USB_Device_ControlRequest (void);
void EVENT_USB_Device_StartOfFrame (void);

// Keyboard
void CreateKeyboardReport(USB_KeyboardReport_Data_t* const ReportData);
void SendNextReport(void);
void ReceiveNextReport(void);
void HID_Task(void);
void process_led(const uint8_t leds);

// Serial
void CDC_Task(void);

void EVENT_USB_Device_Connect (void)
{
	// Keyboard
#ifdef KEYBOARD_ENABLE
	UsingReportProtocol = 1;
#endif // KEYBOARD_ENABLE
}

void EVENT_USB_Device_Disconnect (void) {}

void EVENT_USB_Device_ConfigurationChanged (void)
{
#if defined(KEYBOARD_ENABLE) || defined(SERIAL_ENABLE)
	uint8_t ConfigSuccess = 1;
#endif

	// Keyboard
#ifdef KEYBOARD_ENABLE
	/* Setup HID Report Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);

	/* Turn on Start-of-Frame events for tracking HID report period expiry */
	USB_Device_EnableSOFEvents();
#endif

	// Serial
#ifdef SERIAL_ENABLE
	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);

	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;

	if(!ConfigSuccess)
	{
		while(1)
		{
			LED_DDR ^= (1<<LED_PIN_NUM);
			_delay_ms(500);
		}
	}
#endif
}

void EVENT_USB_Device_ControlRequest (void)
{
	/* Handle HID Class specific requests */
#ifdef KEYBOARD_ENABLE
	switch (USB_ControlRequest.bRequest)
	{
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				USB_KeyboardReport_Data_t KeyboardReportData;

				/* Create the next keyboard report for transmission to the host */
				CreateKeyboardReport(&KeyboardReportData);

				Endpoint_ClearSETUP();

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData));
				Endpoint_ClearOUT();
			}

			break;
		case HID_REQ_SetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Wait until the LED report has been sent by the host */
				while (!(Endpoint_IsOUTReceived()))
				{
					if (USB_DeviceState == DEVICE_STATE_Unattached)
					  return;
				}

				/* Read in the LED report from the host */
				uint8_t LEDStatus = Endpoint_Read_8();

				Endpoint_ClearOUT();
				Endpoint_ClearStatusStage();

				/* Process the incoming LED report */
				process_led(LEDStatus);
			}

			break;
		case HID_REQ_GetProtocol:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write the current protocol flag to the host */
				Endpoint_Write_8(UsingReportProtocol);

				Endpoint_ClearIN();
				Endpoint_ClearStatusStage();
			}

			break;
		case HID_REQ_SetProtocol:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				/* Set or clear the flag depending on what the host indicates that the current Protocol should be */
				UsingReportProtocol = (USB_ControlRequest.wValue != 0);
			}

			break;
		case HID_REQ_SetIdle:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				/* Get idle period in MSB, IdleCount must be multiplied by 4 to get number of milliseconds */
				IdleCount = ((USB_ControlRequest.wValue & 0xFF00) >> 6);
			}

			break;
		case HID_REQ_GetIdle:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write the current idle duration to the host, must be divided by 4 before sent to host */
				Endpoint_Write_8(IdleCount >> 2);

				Endpoint_ClearIN();
				Endpoint_ClearStatusStage();
			}

			break;
	}
#endif // KEYBOARD_ENABLE
	/* Process CDC specific control requests */
#ifdef SERIAL_ENABLE
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
			}

			break;
	}
#endif // SERIAL_ENABLE
}

void EVENT_USB_Device_StartOfFrame (void)
{
	// Keyboard
#ifdef KEYBOARD_ENABLE
	if (IdleMSRemaining)
	{
	  IdleMSRemaining--;
	}
#endif // KEYBOARD_ENABLE
}

// KEYBOARD
#ifdef KEYBOARD_ENABLE
void CreateKeyboardReport(USB_KeyboardReport_Data_t* const ReportData)
{
	//uint8_t UsedKeyCodes = 0;

	/* Clear the report contents */
	memset(ReportData, 0, sizeof(USB_KeyboardReport_Data_t));

	if (!(BUTTON_1_PIN & (1<<BUTTON_1_PIN_NUM)))
	{
		ReportData->KeyCode[0] = HID_KEYBOARD_SC_A;
		ReportData->Modifier = 0;
	}
}

/** Sends the next HID report to the host, via the keyboard data endpoint. */
void SendNextReport(void)
{
	static USB_KeyboardReport_Data_t PrevKeyboardReportData;
	USB_KeyboardReport_Data_t        KeyboardReportData;
	bool                             SendReport = false;

	/* Create the next keyboard report for transmission to the host */
	CreateKeyboardReport(&KeyboardReportData);

	/* Check if the idle period is set and has elapsed */
	if (IdleCount && (!(IdleMSRemaining)))
	{
		/* Reset the idle time remaining counter */
		IdleMSRemaining = IdleCount;

		/* Idle period is set and has elapsed, must send a report to the host */
		SendReport = true;
	}
	else
	{
		/* Check to see if the report data has changed - if so a report MUST be sent */
		SendReport = (memcmp(&PrevKeyboardReportData, &KeyboardReportData, sizeof(USB_KeyboardReport_Data_t)) != 0);
	}

	/* Select the Keyboard Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

	/* Check if Keyboard Endpoint Ready for Read/Write and if we should send a new report */
	if (Endpoint_IsReadWriteAllowed() && SendReport)
	{
		/* Save the current report data for later comparison to check for changes */
		PrevKeyboardReportData = KeyboardReportData;

		/* Write Keyboard Report Data */
		Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads the next LED status report from the host from the LED data endpoint, if one has been sent. */
void ReceiveNextReport(void)
{
	/* Select the Keyboard LED Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_OUT_EPADDR);

	/* Check if Keyboard LED Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in the LED report from the host */
			uint8_t LEDReport = Endpoint_Read_8();

			/* Process the read LED report from the host */
			process_led(LEDReport);
		}

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}
#endif // KEYBOARD_ENABLE

/** Function to manage HID report generation and transmission to the host, when in report mode. */
void HID_Task(void)
{
#ifdef KEYBOARD_ENABLE
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	{
	  return;
	}

	/* Send the next keypress report to the host */
	SendNextReport();

	/* Process the LED report sent from the host */
	ReceiveNextReport();
#endif // KEYBOARD_ENABLE
}

void process_led(const uint8_t leds)
{
	if(leds)
		LED_PORT |= (1<<LED_PIN_NUM);
	else
		LED_PORT &= ~(1<<LED_PIN_NUM);
}


// SERIAL

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
#ifdef SERIAL_ENABLE
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	if (!(BUTTON_2_PIN & (1<<BUTTON_2_PIN_NUM)) && LineEncoding.BaudRateBPS)
	{
		/* Select the Serial Tx Endpoint */
		Endpoint_SelectEndpoint(CDC_TX_EPADDR);

		/* Write the data to the Endpoint */
		char str[] = "azerty";
		Endpoint_Write_Stream_LE(str, 6, NULL);

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

		LED_PORT |= (1<<LED_PIN_NUM);
		_delay_ms(1000);
		LED_PORT &= ~(1<<LED_PIN_NUM);
	}
#endif // SERIAL_ENABLE
}

int main(void)
{
	sei();

	LED_DDR |= (1<<LED_PIN_NUM);
	LED_PORT &= ~(1<<LED_PIN_NUM);

	BUTTON_1_DDR &= ~(1 << BUTTON_1_PIN_NUM);
	BUTTON_1_PORT |= (1 << BUTTON_1_PIN_NUM);

	BUTTON_2_DDR &= ~(1 << BUTTON_2_PIN_NUM);
	BUTTON_2_PORT |= (1 << BUTTON_1_PIN_NUM);

#if defined(KEYBOARD_ENABLE) || defined(SERIAL_ENABLE)
	USB_Init();

	while(1)
	{
		HID_Task();
		CDC_Task();
		USB_USBTask();
	}
#else
	while(1);
#endif

}