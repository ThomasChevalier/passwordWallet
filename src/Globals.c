#include "Globals.h"

uint8_t ORIENTATION = 0;

volatile uint8_t RUNNING = 1;
uint8_t GLOBALS_EVENTS = 0;

uint8_t KEY[16];
uint8_t CURRENT_PASSWORD_ID = 0;
uint8_t CURRENT_PASSWORD_DATA[32];
char PWD_NAME_1[32];
char PWD_NAME_2[32];
char PWD_NAME_3[32];

uint8_t CURRENT_USR_NAME[64];

uint8_t OPTIONS_FLAG = 0;
uint8_t FIRST_PWD_UTIL = 0;
uint8_t FIRST_PWD_ALPHA = 0;
uint8_t NUM_PWD = 0;

uint8_t MEMORY_MAP[23];