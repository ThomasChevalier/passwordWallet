#include "Hardware.h"

#include "Buttons.h"
#include "Fram.h"
#include "Oled.h"
#include "Rfid.h"
#include "Spi.h"
#include "Led.h"

void hardware_init(void)
{
	spi_setup_hardware();
	fram_setup_hardware();
	oled_setup_hardware();
	rfid_setup_hardware();
	buttons_setup_hardware();
	led_setup_harware();

	oled_init();
	rfid_init();
}
