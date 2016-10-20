#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// This file should contain the global variables used in this project

// Is the program running ?
extern volatile uint8_t RUNNING;

// Current events
extern volatile uint8_t GLOBALS_EVENTS;

// The main 128 AES key
extern unsigned char KEY[16];
// For test purpose key is 000102030405060708090A0B0C0D0E0F

// //////////////// //
// Password Section //
// //////////////// //
#define SIZE_OF_PWD_BLOCK 136
#define FIRST_PWD_OFFSET 1028


extern unsigned char password_id;
extern unsigned char pwd1[32];
//unsigned char pwd2[32];
//unsigned char pwd3[32];

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
