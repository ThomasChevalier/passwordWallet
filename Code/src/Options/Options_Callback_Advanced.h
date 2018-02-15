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
 * @brief Reset the device.
 * @details Warning : it cannot be undone.
 */
void opt_callback_full_reset(void);

/**
 * @brief Recover the key by asking the user to type the ascii85 encoded key.
 */
void opt_callback_enter_key(void);

/**
 * @brief Read the key of a card and replace the current key.
 * @details This function does not re-encrypt all the passwords.
 * In normal use case this function shouldn't be called.
 */
void opt_callback_force_card(void);

/**
 * @brief Tells the user to enter (manually) the key and replace the current key.
 * @details This function does not re-encrypt all the passwords.
 */
void opt_callback_force_enter(void);

/**
 * @brief Shows a screen with several indicators of the system health
 * @details Shows how many ram is used, the number of passwords stored in the device,
 * the device's voltage, the number of random bytes availables and the version of the product.
 * The screen disappear when a button is pressed.
 */
void opt_callback_system_info(void);

#endif // OPTIONS_CALLBACK_ADVANCED_HEADER_THOMAS_CHEVALIER
