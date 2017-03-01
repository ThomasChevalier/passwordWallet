#ifndef KEYBOARD_HEADER_THOMAS_CHEVALIER
#define KEYBOARD_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Globals.h" // For KEYBOARD_ENABLE define

#ifdef KEYBOARD_ENABLE

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void HID_Task								(void);
void CreateKeyboardReport					(USB_KeyboardReport_Data_t* const ReportData);
void SendNextReport							(void);
void ReceiveNextReport						(void);

#endif // KEYBOARD_ENABLE

void keyboard_init	(void);
void keyboard_send	(char* data, uint8_t dataLen);
void keyboard_loop	(void);

void	keyboard_on_device_connect			(void);
void	keyboard_on_device_disconnect		(void);
uint8_t	keyboard_on_configuration_changed	(void);
void	keyboard_on_control_request			(void);
void	keyboard_on_start_of_frame			(void);

#endif // KEYBOARD_HEADER_THOMAS_CHEVALIER
