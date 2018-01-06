#ifndef GLOBALS_HEADER_THOMAS_CHEVALIER
#define GLOBALS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/io.h>

// This file contain the global variables used in this project

#define SOFTWARE_VERSION (0x01)

#include "Memory/MemoryLayout.h"

// ///////// //
// PARAMETER //
// ///////// //

#define KEYBOARD_ENABLE  //< Enable or not the keyboard interface
#define SERIAL_ENABLE    //< Enable or not the serial interface

#undef VCC_GRAPH_ENABLE //< Optional feature, show the graph of vcc in function of the time. Just pretty. (200 bytes)

#if defined(KEYBOARD_ENABLE) || defined(SERIAL_ENABLE)
#define USB_ENABLE
#endif


#undef SPI_FRAM    //< Choose the connection type of the FRAM (here this is SPI)
#define I2C_FRAM   //< Choose the connection type of the FRAM (here this is I²C)

#if defined(SPI_FRAM) && defined(I2C_FRAM)
#error Only one type of connection can be used.
#elif !defined(SPI_FRAM) && !defined(I2C_FRAM)
#error A connection must be choosen
#endif

#if defined(I2C_FRAM)
#define I2C_REQUIRED
#endif

/**
 * @brief This define holds the duration of the delay between the led on and the led off, in milliseconds.
 */
#define LED_BLINK_DELAY (150)

/**
 * @brief How many milliseconds without activity the device is active before shutdown.
 * The value must not exceed the maximum value of uint16_t.
 */
#define SHUTDOWN_DELAY (15000)


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
extern p_addr CURRENT_PASSWORD_ID;

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
extern p_addr NUM_PWD;

/**
 * @brief The number of milliseconds since an event happen
 */
extern volatile uint16_t ACTIVITY_TIMER;

extern volatile uint8_t SERIAL_TIMEOUT_TIMER;
#define SERIAL_TIMEOUT (0xFF) // In milliseconds. Must not exceed 255 ms


// /////////// //
// RETURN CODE //
// /////////// //

#define RETURN_SUCCESS (1)
#define RETURN_ERROR (0)


// ///// //
// LOGIC //
// ///// //

#define FALSE (0)
#define TRUE (1)

// /////// //
// UTILITY //
// /////// //

uint8_t strlen_bound(char* str, uint8_t max);

#define STRINGFY_EXPANDED(s) #s
#define STRINGFY(s) STRINGFY_EXPANDED(s)

// /////// //
// KEYCODE //
// /////// //

#define KEYCODE_MODIFIER_SHIFT	(1<<7)
#define KEYCODE_MODIFIER_ALTGR	(1<<6)

void delay_ms_f(uint16_t ms);

#endif // GLOBALS_HEADER_THOMAS_CHEVALIER
