#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/io.h>

// This file contain the global variables used in this project

// ///////// //
// PARAMETER //
// ///////// //


//< Wether or not the internal buffer of pixel for the oled
//< should be stored in FRAM
#undef STORE_SCREEN_BUFFER_IN_FRAM

#define KEYBOARD_ENABLE  //< Enable or not the keyboard interface
#define SERIAL_ENABLE    //< Enable or not the serial interface

#if defined(KEYBOARD_ENABLE) || defined(SERIAL_ENABLE)
#define USB_ENABLE
#endif


#undef SPI_FRAM    //< Choose the connection type of the FRAM (here this is SPI)
#define I2C_FRAM   //< Choose the connection type of the FRAM (here this is I²C)

#if defined(SPI_FRAM) && defined(I2C_FRAM)
#error Only one type of connection can be used.
#elif !defined(SPI_FRAM) && !defined(I2C_FRAM)
#error A connection should be choosen
#endif

#if defined(I2C_FRAM)
#define I2C_REQUIRED
#endif

// #define FRAM_BYTE_SIZE (8192) //< Size of the fram in byte
#define FRAM_BYTE_SIZE (32768)  //< Size of the fram in byte

/**
 * @brief This define holds the duration of the delay between the led on and the led off, in milliseconds.
 */
#define LED_BLINK_DELAY (150)

/**
 * @brief How many milliseconds without activity the device is active before shutdown.
 * The value must not exceed the maximum value of uint16_t.
 */
#define SHUTDOWN_DELAY (15000)

// ///////////// //
// Fram Section  //
// ///////////// //

#define OFFSET_OPTIONS_FLAG (0)
#define SIZE_OPTION_FLAG (1)
#define OPTIONS_FLAG_OFFSET_SORTING_METHOD_L (0)
#define OPTIONS_FLAG_OFFSET_SORTING_METHOD_H (1)
#define OPTIONS_FLAG_OFFSET_ORIENTATION (2)
#define OPTIONS_FLAG_OFFSET_INITIALIZED (3)
#define OPTIONS_FLAG_OFFSET_QWERTY (4)

#define OFFSET_FIRST_PWD_USE (OFFSET_OPTIONS_FLAG + SIZE_OPTION_FLAG) /* 1 */
#define SIZE_FIRST_PWD_USE (1)

#define OFFSET_FIRST_PWD_ALPHA (OFFSET_FIRST_PWD_USE + SIZE_FIRST_PWD_USE) /* 2 */
#define SIZE_FIRST_PWD_ALPHA (1)

#define OFFSET_NUM_PWD (OFFSET_FIRST_PWD_ALPHA + SIZE_FIRST_PWD_ALPHA) /* 3 */
#define SIZE_NUM_PWD (1)

#define OFFSET_OLED_BUFFER (OFFSET_NUM_PWD + SIZE_NUM_PWD) /* 3 */
#ifdef STORE_SCREEN_BUFFER_IN_FRAM
#define SIZE_OLED_BUFFER (1024)
#else
#define SIZE_OLED_BUFFER (0)
#endif

#define OFFSET_ENTROPY_POOL (OFFSET_OLED_BUFFER + SIZE_OLED_BUFFER) /* 4 */
#define SIZE_ENTROPY_POOL (256)

#define OFFSET_MEMORY_MAP (OFFSET_ENTROPY_POOL + SIZE_ENTROPY_POOL) /* 260 */
#if FRAM_BYTE_SIZE == 8192
#define SIZE_MEMORY_MAP (6)
#elif FRAM_BYTE_SIZE == 32768
#define SIZE_MEMORY_MAP (25)
#else
#error Not standart size
#endif

#define OFFSET_FIRST_PWD (OFFSET_MEMORY_MAP + SIZE_MEMORY_MAP) /* 266 or 285*/

// //////////////// //
// Password Section //
// //////////////// //

/**
 * @brief The maximum number of password that the memory can handle.
 * For 8KB memory it is 47
 */
#if FRAM_BYTE_SIZE == 8192
#define MAXIMUM_NUMBER_OF_PWD (47)
#elif FRAM_BYTE_SIZE == 32768
#define MAXIMUM_NUMBER_OF_PWD (195)
#else
#error Not standart size
#endif

#define PWD_ADDR(pwdID, pwdField) (OFFSET_FIRST_PWD + SIZE_OF_PWD_BLOCK * pwdID + pwdField)

#define PWD_OFFSET_PREV_PWD_USE (0)
#define PWD_SIZE_PREV_PWD_USE (1)

#define PWD_OFFSET_NEXT_PWD_USE (PWD_OFFSET_PREV_PWD_USE + PWD_SIZE_PREV_PWD_USE) /* 1 */
#define PWD_SIZE_NEXT_PWD_USE (1)

#define PWD_OFFSET_PREV_PWD_ALPHA (PWD_OFFSET_NEXT_PWD_USE + PWD_SIZE_NEXT_PWD_USE) /* 2 */
#define PWD_SIZE_PREV_PWD_ALPHA (1)

#define PWD_OFFSET_NEXT_PWD_ALPHA (PWD_OFFSET_PREV_PWD_ALPHA + PWD_SIZE_PREV_PWD_ALPHA) /* 3 */
#define PWD_SIZE_NEXT_PWD_ALPHA (1)

#define PWD_OFFSET_PWD_COUNT (PWD_OFFSET_NEXT_PWD_ALPHA + PWD_SIZE_NEXT_PWD_ALPHA) /* 4 */
#define PWD_SIZE_PWD_COUNT (2)

#define PWD_OFFSET_PWD_NAME (PWD_OFFSET_PWD_COUNT + PWD_SIZE_PWD_COUNT) /* 6 */
#define PWD_SIZE_PWD_NAME (32)

#define PWD_OFFSET_PWD_IV (PWD_OFFSET_PWD_NAME + PWD_SIZE_PWD_NAME) /* 38 */
#define PWD_SIZE_PWD_IV (16)

#define PWD_OFFSET_PWD_DATA (PWD_OFFSET_PWD_IV + PWD_SIZE_PWD_IV) /* 54 */
#define PWD_SIZE_PWD_DATA (32)

#define PWD_OFFSET_USR_IV (PWD_OFFSET_PWD_DATA + PWD_SIZE_PWD_DATA) /* 86 */
#define PWD_SIZE_USR_IV (16)

#define PWD_OFFSET_USR_NAME (PWD_OFFSET_USR_IV + PWD_SIZE_USR_IV) /* 102 */
#define PWD_SIZE_USR_NAME (64)

#define SIZE_OF_PWD_BLOCK (PWD_OFFSET_USR_NAME + PWD_SIZE_USR_NAME) /* 166 */


#define PWD_SORTING_NONE (0)
#define PWD_SORTING_USE (1)
#define PWD_SORTING_ALPHA (2)

#define KEY_SIZE (16)
/**
 * @brief The main 128 AES key. It should be the only place where the key is stored for a long period
 */
extern uint8_t KEY[KEY_SIZE];

/**
 * @brief Current events. Can be accessed within interrupts.
 */
#define GLOBALS_EVENTS GPIOR0

/**
 * @brief The options flag stored in fram
 */
#define OPTIONS_FLAG GPIOR1

/**
 * @brief The current password id, start to 0, it is in the order of the memory
 */
//#define CURRENT_PASSWORD_ID GPIOR2
extern uint8_t CURRENT_PASSWORD_ID;

/**
 * @brief Write to OPTIONS_FLAG and send it to FRAM.
 * @details Should be the only way to write to OPTIONS_FLAG.
 * First set OPTIONS_FLAG to opt_flag then write OPTIONS_FLAG to FRAM.
 * 
 * @param opt_flag The new value of OPTIONS_FLAG.
 */
void update_opt_flags(uint8_t opt_flag);

/**
 * @brief The number of password stored in memory now.
 */
extern uint8_t NUM_PWD;

/**
 * @brief The number of milliseconds since an event happen
 */
extern volatile uint16_t ACTIVITY_TIMER;

// ////// //
// EEPROM //
// ////// //

#define EEPROM_OFFSET_RANDSEQ (0)
#define EEPROM_RANDSEQ_SIZE (16)

#define EEPROM_OFFSET_VALIDATION (EEPROM_OFFSET_RANDSEQ + EEPROM_RANDSEQ_SIZE)
#define EEPROM_VALIDATION_SIZE (16)

/* *********** */
/* RETURN CODE */
/* *********** */

#define RETURN_SUCCESS (1)
#define RETURN_ERROR (0)


/* ***** */
/* LOGIC */
/* ***** */

#define FALSE (0)
#define TRUE (1)

/* ******* */
/* UTILITY */
/* ******* */

uint8_t strlen_bound(char* str, uint8_t max);

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
