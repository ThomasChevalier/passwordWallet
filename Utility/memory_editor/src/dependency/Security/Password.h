#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Memory/MemoryLayout.h"
#include "../../Fram.h"

/**
 * @brief Write a password id to fram.
 * @details This function take cares of the size of the p_addr type.
 * 
 * @param framAddr The address where to write the id
 * @param id The id to write
 */
void password_write_id(f_addr_t framAddr, p_addr id);

/**
 * @brief Read a password id from fram.
 * @details This function take cares of the size of the p_addr type.
 * 
 * @param framAddr The address from where to read the id
 */
#if p_addr_sz == 1 // Inline the function
#define password_read_id(addr) fram_read_byte(addr)
#else
p_addr password_read_id(f_addr_t framAddr);
#endif


/**
 * @brief Read the plain password data.
 *  
 * @param pwd_id The id of the password.
 * @param dst An array of 32 bytes where the plain text will be stored.
 * @param key The key used to decrypt the memory.
 */
void        password_read_data              (p_addr pwd_id, uint8_t* dst, uint8_t* key);

/**
 * @brief Read the user name of a password.
 * 
 * @param pwd_id The id of the password.
 * @param dst An array of 64 bytes where the plain text will be stored.
 * @param key The key used to decrypt the memory.
 */
void        password_read_usr_name          (p_addr pwd_id, uint8_t* dst, uint8_t* key);

/**
 * @brief Read the name of a password.
 * 
 * @param pwd_id The id of the password.
 * @param dst A array of 32 bytes where the name of the password will be stored.
 */
void        password_read_name              (p_addr pwd_id, uint8_t* dst);

/**
 * @brief Read the id of the previous password, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the previous password, sort by number of use.
 */
p_addr     password_read_prev_pwd_use      (p_addr pwd_id);

/**
 * @brief Read the id of the next password, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the next password, sort by number of use.
 */
p_addr     password_read_next_pwd_use      (p_addr pwd_id);

/**
 * @brief Read the id of the previous password, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the previous password, sort by alphabet.
 */
p_addr     password_read_prev_pwd_alpha    (p_addr pwd_id);

/**
 * @brief Read the id of the next password, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @return The id of the next password, sort by alphabet.
 */
p_addr     password_read_next_pwd_alpha    (p_addr pwd_id);

/**
 * @brief Read the number of use of the password.
 * 
 * @param pwd_id The id of the password.
 * @return Return how many times the password has been entered.
 */
uint16_t    password_read_counter           (p_addr pwd_id);

/**
 * @brief Set the password of an account, aka the most sensitive information.
 * @details Greate care should be taken that pwd is filled with 0 when it is not needed anymore.
 * Add 21 progress points.
 * 
 * @param pwd_id The id of the password.
 * @param pwd An array of byte that contains the password, size must be 32 bytes and padded with 0 if necessary
 * @param key The key used to decrypt the memory.
 */
void        password_set_data               (p_addr pwd_id, uint8_t* pwd, uint8_t* key);

/**
 * @brief Set the user name of an account.
 * @details Greate care should be taken that usr is filled with 0 when it is not needed anymore.
 * Add 21 progress points.
 * 
 * @param pwd_id The id of the password.
 * @param usr An array of byte that contains the user name, size must be 64 bytes and padded with 0 if necessary
 * @param key The key used to decrypt the memory.
 */
void        password_set_usr_name           (p_addr pwd_id, uint8_t* usr,  uint8_t* key);

/**
 * @brief Set the name of an account.
 * 
 * @param pwd_id The id of the password.
 * @param name An array of byte that contains the name, size must be 32 bytes and padded with 0 if necessary
 */
void        password_set_name               (p_addr pwd_id, uint8_t* name);

/**
 * @brief Set the previous password id, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @param val The previous password id.
 */
void        password_set_prev_pwd_use       (p_addr pwd_id, p_addr  val);

/**
 * @brief Set the next password id, sort by number of use.
 * 
 * @param pwd_id The id of the password.
 * @param val The next password id.
 */
void        password_set_next_pwd_use       (p_addr pwd_id, p_addr  val);

/**
 * @brief Set the previous password id, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @param val The previous password id.
 */
void        password_set_prev_pwd_alpha     (p_addr pwd_id, p_addr  val);

/**
 * @brief Set the next password id, sort by alphabet.
 * 
 * @param pwd_id The id of the password.
 * @param val The next password id.
 */
void        password_set_next_pwd_alpha     (p_addr pwd_id, p_addr  val);

/**
 * @brief Set the number of use of a password.
 * 
 * @param pwd_id The id of the password.
 * @param val How many times the password has been entered.
 */
void        password_set_counter            (p_addr pwd_id, uint16_t val);

/**
 * @brief Add one to the password's number of use.
 * @details Same as password_set_counter(pwd_id, password_read_counter(pwd_id)+1);
 * 
 * @param pwd_id The id of the password.
 */
void        password_increment_counter      (p_addr pwd_id);

/**
 * @brief Generate an array of 31 random bytes and set it as the password data of pwd_id.
 * @details Add 52 progress points.
 * 
 * @param pwd_id The id of the password.
 */
void        password_regenerate             (p_addr pwd_id);

#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
