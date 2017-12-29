#include "Keyboard.h"

#include "Ascii2Keycode.h"
#include "../Hardware/PinDefinition.h"

#include "../Hardware/Led.h"

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


static char keycodeToSend;
static uint8_t needToSend;
static uint8_t capsEnabled;

#endif // KEYBOARD_ENABLE

static void process_led(const uint8_t leds)
{
		//capsEnabled = (LEDReport & HID_KEYBOARD_LED_CAPSLOCK)? TRUE : FALSE;
		if(leds)
		{
			LED_PORT |= (1<<LED_PIN_NUM);
		}
		else
		{
			LED_PORT &= ~(1<<LED_PIN_NUM);
		}
}

void keyboard_init(void)
{
	#ifdef KEYBOARD_ENABLE
	//memset(keyboard_text_buffer, 0, 6);
	keycodeToSend = 0;
	needToSend = FALSE;
	capsEnabled = FALSE;
	#endif // KEYBOARD_ENABLE
}

/**
 * @brief Wait until the keycode is sent
 */
static void wait_for_sent(void)
{
	needToSend = TRUE;
	do
	{
		keyboard_loop();
		USB_USBTask();
		_delay_ms(5); // A delay is needed
	}while(needToSend);
}

void keyboard_send(char* data, uint8_t dataLen)
{
	#ifdef KEYBOARD_ENABLE
		uint8_t sendSpace = FALSE;
		for(uint8_t i = 0; i < dataLen*2; ++i)
		{
			if(i%2 == 0)
			{
				keycodeToSend = data[i/2];
				// Handle special ascii character
				if(keycodeToSend == '^' || keycodeToSend == '`')
				{
					sendSpace = TRUE;
					data[i/2] = ' ';
				}
			}
			else
			{
				keycodeToSend = '~'+1;

				if(sendSpace)
				{
					i -= 2;
					sendSpace = FALSE;
				}
			}

			keycodeToSend = ascii_to_keycode(keycodeToSend);
			wait_for_sent();
		}
	#endif // KEYBOARD_ENABLE
}

void keyboard_send_key_next(void)
{
	keycodeToSend = HID_KEYBOARD_SC_TAB;
	wait_for_sent();
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

	if(needToSend)
	{
		const uint8_t modifier = keycodeToSend & (KEYCODE_MODIFIER_SHIFT | KEYCODE_MODIFIER_ALTGR); // Read modifiers
		keycodeToSend &= ~(KEYCODE_MODIFIER_SHIFT | KEYCODE_MODIFIER_ALTGR); // Clear modifiers
		ReportData->KeyCode[0] = keycodeToSend;

		ReportData->Modifier = 0;
		if( ((!capsEnabled) && (modifier & KEYCODE_MODIFIER_SHIFT)) ||
			(capsEnabled && !(modifier & KEYCODE_MODIFIER_SHIFT)) )
		{
			ReportData->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
		}
		if(modifier & KEYCODE_MODIFIER_ALTGR)
		{
			ReportData->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTALT;
		}

		needToSend = FALSE;
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

/** Function to manage HID report generation and transmission to the host, when in report mode. */
void HID_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	{
		return; // Be careful : an unplugged wire can be the cause of this return
	}

	/* Send the next keypress report to the host */
	SendNextReport();

	/* Process the LED report sent from the host */
	ReceiveNextReport();
}

#endif // KEYBOARD_ENABLE
