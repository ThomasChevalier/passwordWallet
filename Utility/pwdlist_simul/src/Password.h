#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Read the plain password data.
 *  
 * @param pwd_id The id of the password.
 * @param dst An array of 32 bytes where the plain text will be stored.
 * @param key The key used to decrypt the memory.
 */
void		password_read_data				(uint8_t pwd_id, uint8_t* dst, uint8_t* key);

/**
 * @brief Read the user name of a password.
 * 
 * @param pwd_id The id of the password.
 * @param dst An array of 64 bytes where the plain text will be stored.
 * @param key The key used to decrypt the memory.
 */
void		password_read_usr_name			(uint8_t pwd_id, uint8_t* dst, uint8_t* key);

/**
 * @brief Read the name of a password.
 * 
 * @param pwd_id The id of the password.
 * @param dst A array of 32 bytes where the name of the password will be stored.
 */
void		password_read_name				(uint8_t pwd_id, uint8_t* dst);

/**
 * @brief Read the id of the previous password, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the previous password, sort by number of use.
 */
uint8_t		password_read_prev_pwd_use		(uint8_t pwd_id);

/**
 * @brief Read the id of the next password, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the next password, sort by number of use.
 */
uint8_t		password_read_next_pwd_use		(uint8_t pwd_id);

/**
 * @brief Read the id of the previous password, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the previous password, sort by alphabet.
 */
uint8_t		password_read_prev_pwd_alpha	(uint8_t pwd_id);

/**
 * @brief Read the id of the next password, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the next password, sort by alphabet.
 */
uint8_t		password_read_next_pwd_alpha	(uint8_t pwd_id);

/**
 * @brief Read the number of use of the password.
 * 
 * @param pwd_id The id of the password.
 * @return Return how many times the password has been entered.
 */
uint16_t	password_read_counter			(uint8_t pwd_id);

/**
 * @brief Set the password of an account, aka the most sensitive information.
 * @details Greate care should be taken that pwd is filled with 0 when it is not needed anymore.
 * The password is padded with random data if size is inferior to 32 bytes.
 * Add 21+(32-lenght) progress points.
 * 
 * @param pwd_id The id of the password.
 * @param pwd An array of byte that contains the password.
 * @param lenght The lenght of pwd, maximum 32 bytes.
 * @param key The key used to decrypt the memory.
 */
void		password_set_data				(uint8_t pwd_id, uint8_t* pwd, 	uint8_t lenght, uint8_t* key);

/**
 * @brief Set the user name of an account.
 * @details Greate care should be taken that usr is filled with 0 when it is not needed anymore.
 * The user name is padded with random data if size is inferior to 64 bytes.
 * Add 21+(64-lenght) progress points.
 * 
 * @param pwd_id The id of the password.
 * @param usr An array of byte that contains the user name.
 * @param lenght The lenght of usr, maximum 64 bytes.
 * @param key The key used to decrypt the memory.
 */
void		password_set_usr_name			(uint8_t pwd_id, uint8_t* usr,	uint8_t lenght, uint8_t* key);

/**
 * @brief Set the name of an account.
 * 
 * @param pwd_id The id of the password.
 * @param name An array of byte that contains the name.
 * @param lenght The lenght of name, maximum 32 bytes.
 */
void		password_set_name				(uint8_t pwd_id, uint8_t* name, uint8_t lenght);

/**
 * @brief Set the previous password id, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @param val The previous password id.
 */
void		password_set_prev_pwd_use		(uint8_t pwd_id, uint8_t  val);

/**
 * @brief Set the next password id, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @param val The next password id.
 */
void		password_set_next_pwd_use		(uint8_t pwd_id, uint8_t  val);

/**
 * @brief Set the previous password id, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @param val The previous password id.
 */
void		password_set_prev_pwd_alpha		(uint8_t pwd_id, uint8_t  val);

/**
 * @brief Set the next password id, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @param val The next password id.
 */
void		password_set_next_pwd_alpha		(uint8_t pwd_id, uint8_t  val);

/**
 * @brief Set the number of use of a password.
 * 
 * @param pwd_id The id of the password.
 * @param val How many times the password has been entered.
 */
void		password_set_counter			(uint8_t pwd_id, uint16_t val);

/**
 * @brief Add one to the password's number of use.
 * @details Same as password_set_counter(pwd_id, password_read_counter(pwd_id)+1);
 * 
 * @param pwd_id The id of the password.
 */
void		password_increment_counter		(uint8_t pwd_id);

/**
 * @brief Generate an array of 31 random bytes and set it as the password data of pwd_id.
 * @details Add 53 progress points.
 * 
 * @param pwd_id The id of the password.
 */
void		password_regenerate				(uint8_t pwd_id);

#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
