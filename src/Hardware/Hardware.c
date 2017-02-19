#include "Hardware.h"

#include "Buttons.h"
#include "Fram.h"
#include "Keyboard.h"
#include "Oled.h"
#include "Rfid.h"
#include "Spi.h"

void init_hardware(void)
{
	spi_setup_hardware();
	fram_setup_hardware();
	oled_setup_hardware();
	rfid_setup_hardware();
	buttons_setup_hardware();

	oled_init();
	rfid_init();
	keyboard_init();
}