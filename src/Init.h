#ifndef INIT_HEADER_THOMAS_CHEVALIER
#define INIT_HEADER_THOMAS_CHEVALIER

// Init the avr (register, clock, etc ...)
void init_system(void);

// Set the hardware to work with other components (fram, rfid, oled ...)
void init_hardware(void);

// Initialize the code to work with other components
void init_software(void);

#endif // INIT_HEADER_THOMAS_CHEVALIER