#ifndef OPTIONS_CALLBACK_PASSWORD_HEADER_THOMAS_CHEVALIER
#define OPTIONS_CALLBACK_PASSWORD_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Add a password.
 */
void opt_callback_add_pwd(void);

/**
 * @brief Regenerate the current password.
 * @details Warning : the old data is erased.
 */
void opt_callback_regenerate_pwd(void);

/**
 * @brief Change the current password.
 */
void opt_callback_change_pwd(void);

/**
 * @brief Change the current password user name.
 */
void opt_callback_change_usr_name(void);

/**
 * @brief Change the current password name.
 */
void opt_callback_change_name(void);

/**
 * @brief Delete the current password.
 * @details Warning : this cannot be undone.
 */
void opt_callback_delete_pwd(void);

/**
 * @brief Send the password of CURRENT_PASSWORD_ID
 */
void opt_callback_send_pwd(void);

/**
 * @brief Send the user name of CURRENT_PASSWORD_ID
 */
void opt_callback_send_usr(void);

/**
 * @brief Send the password and the user name of CURRENT_PASSWORD_ID
 */
void opt_callback_send_both(void);

#endif // OPTIONS_CALLBACK_PASSWORD_HEADER_THOMAS_CHEVALIER
