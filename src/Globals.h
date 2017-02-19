#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// This file should contain the global variables used in this project

// Define if this is the first pression on a button
extern uint8_t FIRST_PRESS;

// /////////////////// //
// Option Menu Section //
// /////////////////// //

#define OPTION_MENU_MIN_STR_INDEX (0)
#define OPTION_MENU_MAX_CHOICE (5)

#define OPTION_PASSWORD_MENU_MIN_STR_INDEX (OPTION_MENU_MAX_CHOICE + OPTION_MENU_MIN_STR_INDEX)
#define OPTION_PASSWORD_MENU_MAX_CHOICE (5)

#define OPTION_SORT_MENU_MIN_STR_INDEX (OPTION_PASSWORD_MENU_MAX_CHOICE + OPTION_PASSWORD_MENU_MIN_STR_INDEX)
#define OPTION_SORT_MENU_MAX_CHOICE (3)

#define OPTION_ADVANCED_MENU_MIN_STR_INDEX (OPTION_SORT_MENU_MAX_CHOICE + OPTION_SORT_MENU_MIN_STR_INDEX)
#define OPTION_ADVANCED_MENU_MAX_CHOICE (6)


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

// ///////////// //
// Fram Section  //
// ///////////// //

// Size of the fram in byte
#define FRAM_BYTE_SIZE (8192)

#define OFFSET_OPTIONS_FLAG (0)
#define SIZE_OPTION_FLAG (1)
#define OPTIONS_FLAG_OFFSET_NO_ENCRYPTION (0)
#define OPTIONS_FLAG_OFFSET_SORTING_METHOD_L (1)
#define OPTIONS_FLAG_OFFSET_SORTING_METHOD_H (2)
#define OPTIONS_FLAG_OFFSET_ORIENTATION (3)
#define OPTIONS_FLAG_OFFSET_INITIALIZED (4)

#define OFFSET_FIRST_PWD_UTIL (OFFSET_OPTIONS_FLAG + SIZE_OPTION_FLAG) /* 1 */
#define SIZE_FIRST_PWD_UTIL (1)

#define OFFSET_FIRST_PWD_ALPHA (OFFSET_FIRST_PWD_UTIL + SIZE_FIRST_PWD_UTIL) /* 2 */
#define SIZE_FIRST_PWD_ALPHA (1)

#define OFFSET_NUM_PWD (OFFSET_FIRST_PWD_ALPHA + SIZE_FIRST_PWD_ALPHA) /* 3 */
#define SIZE_NUM_PWD (1)

#define OFFSET_ENTROPY_SIZE (OFFSET_NUM_PWD + SIZE_NUM_PWD) /* 4 */
#define SIZE_ENTROPY_SIZE (2)

#define OFFSET_ENTROPY_POOL (OFFSET_ENTROPY_SIZE + SIZE_ENTROPY_SIZE) /* 6 */
#define SIZE_ENTROPY_POOL (256)

#define OFFSET_MEMORY_MAP (OFFSET_ENTROPY_POOL + SIZE_ENTROPY_POOL) /* 262 */
#define SIZE_MEMORY_MAP (6)

#define OFFSET_FIRST_PWD (OFFSET_MEMORY_MAP + SIZE_MEMORY_MAP) /* 268 */

// //////////////// //
// Password Section //
// //////////////// //

// The maximum number of password that the memory can handle.
// For 8KB fram it is 47
#define MAXIMUM_NUMBER_OF_PWD 47


#define SIZE_OF_PWD_BLOCK (166)

#define PWD_ADDR(pwdID, pwdField) (OFFSET_FIRST_PWD + SIZE_OF_PWD_BLOCK * pwdID + pwdField)
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

extern uint8_t OPTIONS_FLAG;     // The options flag stored in fram

extern uint8_t FIRST_PWD_UTIL;  // bytes 2 and 3 of fram
extern uint8_t FIRST_PWD_ALPHA; // bytes 4 and 5 of fram
extern uint8_t NUM_PWD;         // bytes 6 of fram

extern uint8_t MEMORY_MAP[6]; // The memory map

#define KEYBOARD_ENABLE

void blink(uint8_t times);

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
