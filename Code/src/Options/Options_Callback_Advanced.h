#ifndef OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER
#define OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Security/Authentification.h"
#include "../Hardware/SelfTest.h"

/**
 * @brief Change the master key.
 */
#define opt_callback_change_key user_change_key

/**
 * @brief Show the master key, encoded in ascii85.
 */
void opt_callback_show_key(void);

/**
 * @brief Inverse the screen.
 */
void opt_callback_inverse_screen(void);

/**
 * @brief Change the keyboard layout.
 */
void opt_callback_change_keyboard(void);

/**
 * @brief Procede to a self test.
 * @details Warning : it send keystrokes to the computer.
 */
#define opt_callback_self_test self_test_execute

/**
 * @brief Erase the device.
 * @details Warning : it cannot be undone.
 */
void opt_callback_full_reset(void);

/**
 * @brief Recover the key by asking the user to type the ascii85 encoded key.
 */
void opt_callback_enter_key(void);

void opt_callback_force_card(void);
void opt_callback_force_enter(void);

void opt_callback_system_info(void);

#endif // OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER
