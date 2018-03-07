#ifndef PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
#define PASSWORD_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#include "../Memory/MemoryLayout.h"

#include "../Hardware/Fram.h"

/**
 * @brief Return the current sorting method.
 * @return The sorting method PWD_SORTING_NONE, PWD_SORTING_USE or PWD_SORTING_ALPHA.
 * These constants are declared in Globals.h.
 */
uint8_t pwd_list_get_sorting_method             (void);

/**
 * @brief Get the id of the first password according to the current sorting method.
 * @return The id of the first password.
 */
p_addr pwd_list_get_first_pwd_id               (void);

/**
 * @brief Get the id of the first password in memory, it means the first chunk of memory used to store a password.
 * @return The id of the first password.
 */
p_addr pwd_list_get_first_pwd_id_sort_none     (void);

/**
 * @brief Get the id of the first password, sort by number of use.
 * @return The id of the first password.
 */
p_addr pwd_list_get_first_pwd_id_sort_use      (void);

/**
 * @brief Get the id of the first password, sort by name.
 * @return The id of the first password.
 */
p_addr pwd_list_get_first_pwd_id_sort_alpha    (void);

/**
 * @brief Get the previous password id according to the current sorting method.
 * @details If pwd_id is the first password then the function return the last password.
 * @param pwd_id The id of the next password of the id we want.
 * @return The id of the password before pwd_id.
 */
p_addr pwd_list_get_prev_pwd_id                (p_addr pwd_id);

/**
 * @brief Get the previous password of pwd_id in memory.
 * @details If pwd_id is the first password then the function scan the memory from top to bottom and return
 * the first chunk of memory used that the function find.
 * @param pwd_id The id of the next password of the id we want.
 * @return The id of the password before pwd_id.
 */
p_addr pwd_list_get_prev_pwd_id_sort_none      (p_addr pwd_id);

/**
 * @brief Get the previous password of pwd_id, sort by number of use.
 * @details If pwd_id is the first password then the function start from pwd_id and read all the next
 * password until the next password is invalid. It returns the end of the list.
 * @param pwd_id The id of the next password of the id we want.
 * @return The id of the password before pwd_id.
 */
p_addr pwd_list_get_prev_pwd_id_sort_use       (p_addr pwd_id);

/**
 * @brief Get the previous password of pwd_id, sort by name.
 * @details If pwd_id is the first password then the function start from pwd_id and read all the next
 * password until the next password is invalid. It returns the end of the list.
 * @param pwd_id The id of the next password of the id we want.
 * @return The id of the password before pwd_id.
 */
p_addr pwd_list_get_prev_pwd_id_sort_alpha     (p_addr pwd_id);

/**
 * @brief Get the password after pwd_id according to the current sorting method.
 * @details If pwd_id is the last password, then the function return the first password.
 * 
 * @param pwd_id The password before the password we want.
 * @return The id of the password after pwd_id.
 */
p_addr pwd_list_get_next_pwd_id                (p_addr pwd_id);

/**
 * @brief Get the password after pwd_id in memory.
 * @details If pwd_id is the last password, then the function return the first chunk of memory
 * used from chunk 0 (including).
 * 
 * @param pwd_id The password before the password we want.
 * @return The id of the password after pwd_id.
 */
p_addr pwd_list_get_next_pwd_id_sort_none      (p_addr pwd_id);

/**
 * @brief Get the password after pwd_id , sort by use.
 * @details If pwd_id is the last password, then the function return the same value as pwd_list_get_first_pwd_id_sort_use.
 * 
 * @param pwd_id The password before the password we want.
 * @return The id of the password after pwd_id.
 */
p_addr pwd_list_get_next_pwd_id_sort_use       (p_addr pwd_id);

/**
 * @brief Get the password after pwd_id , sort by name.
 * @details If pwd_id is the last password, then the function return the same value as pwd_list_get_first_pwd_id_sort_alpha.
 * 
 * @param pwd_id The password before the password we want.
 * @return The id of the password after pwd_id.
 */
p_addr pwd_list_get_next_pwd_id_sort_alpha     (p_addr pwd_id);

/**
 * @brief Delete the password pwd_id.
 * @details The function  fill the memory chunk with 0. It takes care to update the lists.
 * 
 * @param pwd_id The id of the password to erase.
 */
void    pwd_list_delete_pwd                     (p_addr pwd_id);

/**
 * @brief Add a new password.
 * @details This function search the memory for a free chunk, if there is such a chunk it stores the password in and update the list.
 * The function encrypts data and usrName.
 * 
 * @param name The password name. Must be a 32 bytes long array.
 * @param data The plain password. Must be a 32 bytes long array.
 * @param usrName The plain user name. Must be a 64 bytes long array.
 * @return Returns RETURN_SUCCESS if the password has been successfully added to the memory and RETURN_ERROR if the memory is full.
 */
uint8_t pwd_list_add_pwd                        (uint8_t* name, uint8_t* data, uint8_t* usrName);

/**
 * @brief Sort passwords by use.
 * @details It uses an insertion sort algorithm.
 */
void    pwd_list_sort_use                       (void);

/**
 * @brief Sort passwords by name.
 * @details Name are uppercased before being sorted. It uses an insertion sort algorithm.
 */
void    pwd_list_sort_alpha                     (void);



#endif // PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
