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

void serial_init	(void);
void serial_loop	(void);
void serial_send	(uint8_t* buffer, uint8_t lenght);

void	serial_on_device_connect			(void);
void	serial_on_device_disconnect			(void);
uint8_t	serial_on_configuration_changed		(void);
void	serial_on_control_request			(void);
void	serial_on_start_of_frame			(void);


#endif // SERIAL_HEADER_THOMAS_CHEVALIER
