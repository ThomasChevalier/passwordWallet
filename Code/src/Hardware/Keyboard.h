#ifndef KEYBOARD_HEADER_THOMAS_CHEVALIER
#define KEYBOARD_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Globals.h" // For KEYBOARD_ENABLE define

#ifdef KEYBOARD_ENABLE

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void HID_Task								(void);
void EVENT_USB_Device_Connect				(void);
void EVENT_USB_Device_Disconnect			(void);
void EVENT_USB_Device_ConfigurationChanged	(void);
void EVENT_USB_Device_ControlRequest		(void);
void EVENT_USB_Device_StartOfFrame			(void);
void CreateKeyboardReport					(USB_KeyboardReport_Data_t* const ReportData);
void SendNextReport							(void);
void ReceiveNextReport						(void);

#endif // KEYBOARD_ENABLE

void keyboard_init	(void);
void keyboard_send	(char* data, unsigned char dataLen);
void keyboard_loop	(void);

#endif // KEYBOARD_HEADER_THOMAS_CHEVALIER
