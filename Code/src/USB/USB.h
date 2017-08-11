#ifndef USB_HEADER_THOMAS_CHEVALIER
#define USB_HEADER_THOMAS_CHEVALIER

#include "../Globals.h" // For USB_ENABLE define

/**
 * @brief Init the USB LUFA library
 * @details Init lufa library and the keyboard and the serial
 */
void USB_init								(void);

/**
 * @brief Update the USB code.
 * @details Must be called regularly.
 */
void USB_loop								(void);

#ifdef USB_ENABLE

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void EVENT_USB_Device_Connect				(void);
void EVENT_USB_Device_Disconnect			(void);
void EVENT_USB_Device_ConfigurationChanged	(void);
void EVENT_USB_Device_ControlRequest		(void);
void EVENT_USB_Device_StartOfFrame			(void);

#endif // USB_ENABLE


#endif // USB_HEADER_THOMAS_CHEVALIER
