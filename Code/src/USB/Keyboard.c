#include "Keyboard.h"

#include <avr/io.h>
#include <string.h>

#include "Ascii2Keycode.h"

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


static char letterToSend;
static uint8_t needToSend;

#endif // KEYBOARD_ENABLE

void keyboard_init(void)
{
	#ifdef KEYBOARD_ENABLE
	//memset(keyboard_text_buffer, 0, 6);
	letterToSend = 0;
	needToSend = 0;
	#endif // KEYBOARD_ENABLE
}


void keyboard_send(char* data, uint8_t dataLen)
{
	#ifdef KEYBOARD_ENABLE
	// { (dataLen + 5)/ 6 } is used to around the result of (dataLen/6) to the upper integer
	// We divide data in slice of 6 character because the maximum number of character that the keyboard can send at one time is 6.
	/*for(uint8_t slice = 0; slice < (dataLen + 5)/ 6; ++slice)
	{
		// Clear buffer (previous text may be present)
		memset(keyboard_text_buffer, 0, 6);
		// Handle a slice of 6 characters
		uint8_t i = 0;
		for(i = 0; i < dataLen - slice * 6; ++i)
		{
			//const uint8_t keycode = pgm_read_byte_near(ascii_to_keycode_map + (data[i+ (slice*6)]-' '));
			keyboard_text_buffer[i] = data[i+ (slice*6)];
		}
		keyboard_buffer_lenght = i;

		// Until all the characters have been sent, we retry to send them.
		while(keyboard_buffer_lenght != 0)
		{
			keyboard_loop();
			USB_USBTask(); // Needed here
		}*/

		for(uint8_t i = 0; i < dataLen; ++i)
		{
			letterToSend = data[i];
			needToSend = 1;
			do
			{
				keyboard_loop();
				USB_USBTask();
				_delay_ms(5); // A delay is needed
			}while(needToSend);
		}
		
	//}
	#endif // KEYBOARD_ENABLE
}

void keyboard_loop(void)
{
#ifdef KEYBOARD_ENABLE
	HID_Task();
#endif // KEYBOARD_ENABLE
}

void keyboard_on_device_connect(void)
{
	#ifdef KEYBOARD_ENABLE
	/* Default to report protocol on connect */
	UsingReportProtocol = 1;
	#endif
}


void keyboard_on_device_disconnect(void)
{

}


uint8_t keyboard_on_configuration_changed(void)
{
	#ifdef KEYBOARD_ENABLE
	uint8_t ConfigSuccess = 1;

	/* Setup HID Report Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);

	/* Turn on Start-of-Frame events for tracking HID report period expiry */
	USB_Device_EnableSOFEvents();
	return ConfigSuccess;

	#else
	return 1;
	#endif
}

void keyboard_on_control_request(void)
{
	#ifdef KEYBOARD_ENABLE
	/* Handle HID Class specific requests */
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
				//uint8_t LEDStatus = Endpoint_Read_8();

				Endpoint_ClearOUT();
				Endpoint_ClearStatusStage();

				/* Process the incoming LED report */
				//ProcessLEDReport(LEDStatus);
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
	#endif
}


void keyboard_on_start_of_frame(void)
{
	#ifdef KEYBOARD_ENABLE
	/* One millisecond has elapsed, decrement the idle time remaining counter if it has not already elapsed */
	if (IdleMSRemaining)
	{
	  IdleMSRemaining--;
	}
	#endif
}

#ifdef KEYBOARD_ENABLE
/** Fills the given HID report data structure with the next HID report to send to the host.
 *
 *  \param[out] ReportData  Pointer to a HID report data structure to be filled
 */
void CreateKeyboardReport(USB_KeyboardReport_Data_t* const ReportData)
{
	//uint8_t UsedKeyCodes = 0;

	/* Clear the report contents */
	memset(ReportData, 0, sizeof(USB_KeyboardReport_Data_t));

	// Loop through the buffer of keycode
	/*
	uint8_t i = 0;
	for(i = 0; i < keyboard_buffer_lenght; ++i)
	{
		// Read key and get the right keycode
		const char key = keyboard_text_buffer[i];
		uint8_t keycode = pgm_read_byte_near(ascii_to_keycode_map + (key-' '));
		const uint8_t modifier = keycode & (1<<7); // Read MSB
		keycode &= 0x7F; // Clear MSB
		// If there is no modifier we can send multiple key
		if(!modifier)
		{
			ReportData->KeyCode[UsedKeyCodes++] = keycode;
		}
		else // There is a modifier to enable
		{
			// If this is the first key
			if(i == 0)
			{
				// Send it and set modifier
				ReportData->KeyCode[UsedKeyCodes++] = keycode;
				ReportData->Modifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;
			}
			else
			{
				// Keep data for an other report
				memmove(keyboard_text_buffer, keyboard_text_buffer + i, keyboard_buffer_lenght - i);
				break;
			}
		}
	}
	keyboard_buffer_lenght -= i;*/

	if(needToSend)
	{
		uint8_t keycode = pgm_read_byte_near(ascii_to_keycode(letterToSend));
		const uint8_t modifier = keycode & 0XC0; // Read modifiers
		keycode &= 0XC0; // Clear modifiers
		ReportData->KeyCode[0] = keycode;

		ReportData->Modifier = 0;
		if(modifier & KEYCODE_MODIFIER_SHIFT)
		{
			ReportData->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
		}
		if(modifier & KEYCODE_MODIFIER_ALTGR)
		{
			ReportData->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTALT;
		}

		needToSend = 0;
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
			// /* Read in the LED report from the host */
			// uint8_t LEDReport = Endpoint_Read_8();

			// /* Process the read LED report from the host */
			// ProcessLEDReport(LEDReport);
		}

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

/** Function to manage HID report generation and transmission to the host, when in report mode. */
void HID_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	{
	  return;
	}

	/* Send the next keypress report to the host */
	SendNextReport();

	/* Process the LED report sent from the host */
	ReceiveNextReport();
}

#endif // KEYBOARD_ENABLE