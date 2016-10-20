#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// This file should contain the global variables used in this project

// Is the program running ?
extern volatile uint8_t RUNNING;

// Current events
extern volatile uint8_t GLOBALS_EVENTS;

// The main 128 AES key
extern uint8_t KEY[16];
// For test purpose key is 000102030405060708090A0B0C0D0E0F

// //////////////// //
// Password Section //
// //////////////// //
#define SIZE_OF_PWD_BLOCK (170)
#define FIRST_PWD_OFFSET (1031)

// The current password id, start to 0, it is in the order of the memory
extern uint16_t CURRENT_PASSWORD_ID;
// The current password decrypted
extern uint8_t CURRENT_PASSWORD_DATA[32];
// The names of the three password displayed
extern char PWD_NAME_1[32];
extern char PWD_NAME_2[32];
extern char PWD_NAME_3[32];

// Fram data
// The options flag stored in fram
extern uint8_t OPTIONS_FLAG;
extern uint16_t FIRST_PWD_UTIL;
extern uint16_t FIRST_PWD_ALPHA;
extern uint16_t NUM_PWD;

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
