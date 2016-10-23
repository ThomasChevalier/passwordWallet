#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// This file should contain the global variables used in this project


// /////////////////////////////////////// //
//  Finite state machine related variables //
// /////////////////////////////////////// //


extern volatile uint8_t RUNNING; // Is the program running ?
extern uint8_t GLOBALS_EVENTS;   // Current events, not to be accessed by interrupt

// //////////////// //
// Password Section //
// //////////////// //

#define START_OF_OLED_BUFFER (7)
#define SIZE_OF_PWD_BLOCK (170)
#define FIRST_PWD_OFFSET (1031)

// For test purpose key is 000102030405060708090A0B0C0D0E0F
extern uint8_t KEY[16]; // The main 128 AES key

extern uint16_t CURRENT_PASSWORD_ID;      // The current password id, start to 0, it is in the order of the memory
extern uint8_t CURRENT_PASSWORD_DATA[32]; // The current password decrypted

// The names of the three passwords displayed
extern char PWD_NAME_1[32];
extern char PWD_NAME_2[32];
extern char PWD_NAME_3[32];

// The decrypted user name of the three passwords displayed
extern char USR_NAME_1[32];
extern char USR_NAME_2[32];
extern char USR_NAME_3[32];

// ////////////////////// //
// Options stored in FRAM //
// ////////////////////// //

extern uint8_t OPTIONS_FLAG;     // The options flag stored in fram

extern uint16_t FIRST_PWD_UTIL;  // bytes 2 and 3 of fram
extern uint16_t FIRST_PWD_ALPHA; // bytes 4 and 5 of fram
extern uint16_t NUM_PWD;         // bytes 6 and 7 of fram

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
