#include "Hardware.h"

#include "Buttons.h"
#include "Fram.h"
#include "Oled.h"
#include "Rfid.h"
#include "Spi.h"
#include "Led.h"

#include "PinDefinition.h"

void hardware_init(void)
{
	// For i2c
	fram_setup_hardware();

	INIT_ALL_DDR
	INIT_ALL_PORT

	spi_init();

	oled_init();
	
	// Note : the rfid module don't need to be initialized
	// The module is initialized when necessary and then power down
}
