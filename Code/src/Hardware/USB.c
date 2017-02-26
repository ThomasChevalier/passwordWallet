#include "USB.h"


#include <avr/power.h>

#include "../Globals.h"

#include "Keyboard.h"
#include "Serial.h"

void USB_init (void)
{
	#ifdef USB_ENABLE
	/* Disable clock division */
	clock_prescale_set(clock_div_1);
	USB_Init();
	#endif

	keyboard_init();
	serial_init();
}

void USB_loop (void)
{
	keyboard_loop();
	serial_loop();

	#ifdef USB_ENABLE
	USB_USBTask();
	#endif
}

void EVENT_USB_Device_Connect (void)
{
	keyboard_on_device_connect();
	serial_on_device_connect();
}

void EVENT_USB_Device_Disconnect (void)
{
	RUNNING = 0;
	keyboard_on_device_disconnect();
	serial_on_device_disconnect();
}

void EVENT_USB_Device_ConfigurationChanged (void)
{
	uint8_t ConfigSuccess = 1;
	ConfigSuccess &= keyboard_on_configuration_changed();
	ConfigSuccess &= serial_on_configuration_changed();
}

void EVENT_USB_Device_ControlRequest (void)
{
	keyboard_on_control_request();
	serial_on_control_request();
}

void EVENT_USB_Device_StartOfFrame (void)
{
	keyboard_on_start_of_frame();
	serial_on_start_of_frame();
}