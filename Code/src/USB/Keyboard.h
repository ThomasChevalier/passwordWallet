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

/**
 * @brief Initialize statics variables for keyboard.
 */
void keyboard_init	(void);

/**
 * @brief Send a string to the computer as if it was typed.
 * 
 * @param data The string to send to the computer.
 * @param dataLen The lenght of the string.
 */
void keyboard_send	(char* data, uint8_t dataLen);

/**
 * @brief Update the keyboard related stuff.
 * @details Should be called regularly.
 */
void keyboard_loop	(void);

/** 
 * @brief Event handler for the USB_Connect event.
 * @details This indicates that the device is enumerating and starts the library USB task to begin the enumeration and USB management process.
 */
void	keyboard_on_device_connect			(void);

/**
 * @brief Event handler for the USB_Disconnect event.
 * @details This indicates that the device is no longer connected to an host.
 */
void	keyboard_on_device_disconnect		(void);

/**
 * @brief Event handler for the USB_ConfigurationChanged event.
 * @details This is fired when the host sets the current configuration
 * of the USB device after enumeration, and configures the keyboard device endpoints.
 * 
 * @return Return 1 if no error occured, 0 otherwise.
 */
uint8_t	keyboard_on_configuration_changed	(void);

/**
 * @brief Event handler for the USB_ControlRequest event.
 * @details This is used to catch and process control requests sent to
 * the device from the USB host before passing along unhandled control requests to the library for processing
 * internally.
 */
void	keyboard_on_control_request			(void);

/**
 * @brief Event handler for the USB device Start Of Frame event. 
 */
void	keyboard_on_start_of_frame			(void);

#endif // KEYBOARD_HEADER_THOMAS_CHEVALIER
