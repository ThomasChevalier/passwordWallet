#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// This file should contain the global variables used in this project

// Define if this is the first pression on a button
extern uint8_t FIRST_PRESS;

// /////////////////////////////////////// //
//  Finite state machine related variables //
// /////////////////////////////////////// //


extern volatile uint8_t RUNNING; // Is the program running ?
extern uint8_t GLOBALS_EVENTS;   // Current events, not to be accessed by interrupt

// //////////////// //
// Progress Section //
// //////////////// //

extern uint8_t PROGRESS_COMPLEXITY;
extern uint8_t PROGRESS_ADVANCE;

// //////////////// //
// Password Section //
// //////////////// //

// The maximum number of password that the memory can handle.
// For 32KB fram it is 184. For test it is 35.
#define MAXIMUM_NUMBER_OF_PWD 35

#define START_OF_OLED_BUFFER (4)
#define OLED_BUFFER_SIZE (1024)

#define OFFSET_ENTROPY_SIZE (OLED_BUFFER_SIZE + START_OF_OLED_BUFFER)
#define OFFSET_ENTROPY_POOL (OFFSET_ENTROPY_SIZE + 2)
#define ENTROPY_POOL_SIZE (1024)
#define OFFSET_MEMORY_MAP (OFFSET_ENTROPY_POOL + ENTROPY_POOL_SIZE)
#define MEMORY_MAP_SIZE (23)

#define SIZE_OF_PWD_BLOCK (166)
#define FIRST_PWD_OFFSET (2077)

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

#undef FRAM_BUFFER

// Size of the fram in byte
#define FRAM_BYTE_SIZE (8192)
#define OFFSET_OPTIONS_FLAG (0)
#define OFFSET_OPTION_INITIALIZED (4)
#define OFFSET_FIRST_PWD_UTIL (1)
#define OFFSET_FIRST_PWD_ALPHA (2)
#define OFFSET_NUM_PWD (3)

extern uint8_t OPTIONS_FLAG;     // The options flag stored in fram

extern uint8_t FIRST_PWD_UTIL;  // bytes 2 and 3 of fram
extern uint8_t FIRST_PWD_ALPHA; // bytes 4 and 5 of fram
extern uint8_t NUM_PWD;         // bytes 6 of fram

extern uint8_t MEMORY_MAP[23]; // The memory map

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
