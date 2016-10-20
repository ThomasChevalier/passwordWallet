#include "Globals.h"

volatile uint8_t RUNNING = 1;

volatile uint8_t GLOBALS_EVENTS = 0;

uint8_t KEY[16];

uint16_t CURRENT_PASSWORD_ID = 0;
uint8_t CURRENT_PASSWORD_DATA[32];

char PWD_NAME_1[32];
char PWD_NAME_2[32];
char PWD_NAME_3[32];

uint8_t OPTIONS_FLAG = 0;
uint16_t FIRST_PWD_UTIL = 0;
uint16_t FIRST_PWD_ALPHA = 0;
uint16_t NUM_PWD = 0;
