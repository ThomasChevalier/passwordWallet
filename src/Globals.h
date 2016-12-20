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

// The maximum number of password that the memory can handle.
// For 32KB fram it is 184. For test it is 35.
#define MAXIMUM_NUMBER_OF_PWD 35

#define START_OF_OLED_BUFFER (4)
#define SIZE_OF_PWD_BLOCK (166)
#define FIRST_PWD_OFFSET (2077)

#define OFFSET_ENTROPY_SIZE (1028)
#define OFFSET_ENTROPY_POOL (1030)
#define OFFSET_MEMORY_MAP (2054)
#define MEMORY_MAP_SIZE (23)

#define PWD_ADDR(pwdID, pwdField) (FIRST_PWD_OFFSET + SIZE_OF_PWD_BLOCK * pwdID + pwdField)
#define PWD_OFFSET_PREV_PWD_UTIL (0)
#define PWD_OFFSET_NEXT_PWD_UTIL (1)
#define PWD_OFFSET_PREV_PWD_ALPHA (2)
#define PWD_OFFSET_NEXT_PWD_ALPHA (3)
#define PWD_OFFSET_PWD_COUNT (4)
#define PWD_OFFSET_PWD_NAME (6)
#define PWD_OFFSET_PWD_IV (38)
#define PWD_OFFSET_PWD_DATA (54)
#define PWD_OFFSET_USR_IV (86)
#define PWD_OFFSET_USR_NAME (102)

// For test purpose key is 000102030405060708090A0B0C0D0E0F
extern uint8_t KEY[16]; // The main 128 AES key

extern uint8_t CURRENT_PASSWORD_ID;      // The current password id, start to 0, it is in the order of the memory
extern uint8_t CURRENT_PASSWORD_DATA[32]; // The current password decrypted

// The names of the three passwords displayed
extern char PWD_NAME_1[32];
extern char PWD_NAME_2[32];
extern char PWD_NAME_3[32];

// The decrypted user name of password displayed. Only the current username is necessary, since the others name are not displayed.
extern uint8_t CURRENT_USR_NAME[64];

// ////////////////////// //
// Options stored in FRAM //
// ////////////////////// //

extern uint8_t OPTIONS_FLAG;     // The options flag stored in fram

extern uint8_t FIRST_PWD_UTIL;  // bytes 2 and 3 of fram
extern uint8_t FIRST_PWD_ALPHA; // bytes 4 and 5 of fram
extern uint8_t NUM_PWD;         // bytes 6 of fram

extern uint8_t MEMORY_MAP[23]; // The memory map

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
