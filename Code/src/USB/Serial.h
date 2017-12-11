#ifndef SERIAL_HEADER_THOMAS_CHEVALIER
#define SERIAL_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Globals.h"

#ifdef SERIAL_ENABLE

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void CDC_Task(void);

#endif // SERIAL_ENABLE
/**
 * @brief Init the serial communication.
 * @details This function does nothing yet, because there is nothing to do.
 */
void serial_init (void);

/**
 * @brief Update the serial communication.
 * @details Should be called regularly.
 */
void serial_loop (void);

/**
 * @brief Send an array of bytes to the pc through the serial port.
 * @details The user should take care that the buffer is not too long.
 * 
 * @param buffer The array of bytes to send.
 * @param lenght The lenght of the array.
 */
void serial_send (uint8_t* buffer, uint8_t lenght);


/** 
 * @brief Event handler for the USB_Connect event.
 * @details This indicates that the device is enumerating and starts the library USB task to begin the enumeration and USB management process.
 */
void serial_on_device_connect (void);

/**
 * @brief Event handler for the USB_Disconnect event.
 * @details This indicates that the device is no longer connected to an host.
 */
void serial_on_device_disconnect (void);

/**
 * @brief Event handler for the USB_ConfigurationChanged event.
 * @details This is fired when the host sets the current configuration
 * of the USB device after enumeration, and configures the serial device endpoints.
 * 
 * @return Return 1 if no error occured, 0 otherwise.
 */
uint8_t serial_on_configuration_changed (void);

/**
 * @brief Event handler for the USB_ControlRequest event.
 * @details This is used to catch and process control requests sent to
 * the device from the USB host before passing along unhandled control requests to the library for processing
 * internally.
 */
void serial_on_control_request (void);

/**
 * @brief Event handler for the USB device Start Of Frame event. 
 */
void serial_on_start_of_frame (void);


#endif // SERIAL_HEADER_THOMAS_CHEVALIER
