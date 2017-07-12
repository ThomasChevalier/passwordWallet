#ifndef PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
#define PASSWORD_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

uint8_t	pwd_list_get_sorting_method				(void);
uint8_t	pwd_list_get_first_pwd_id				(void);
uint8_t	pwd_list_get_first_pwd_id_sort_none		(void);
uint8_t	pwd_list_get_first_pwd_id_sort_use		(void);
uint8_t	pwd_list_get_first_pwd_id_sort_alpha	(void);

uint8_t	pwd_list_get_prev_pwd_id				(uint8_t pwd_id);
uint8_t	pwd_list_get_prev_pwd_id_sort_none		(uint8_t pwd_id);
uint8_t	pwd_list_get_prev_pwd_id_sort_use		(uint8_t pwd_id);
uint8_t	pwd_list_get_prev_pwd_id_sort_alpha		(uint8_t pwd_id);

uint8_t	pwd_list_get_next_pwd_id				(uint8_t pwd_id);
uint8_t	pwd_list_get_next_pwd_id_sort_none		(uint8_t pwd_id);
uint8_t	pwd_list_get_next_pwd_id_sort_use		(uint8_t pwd_id);
uint8_t	pwd_list_get_next_pwd_id_sort_alpha		(uint8_t pwd_id);

void	pwd_list_delete_pwd						(uint8_t pwd_id);

// Add a new password
// Password name should be 32 bytes long
// Password data should be 32 bytes long
// User name     should be 64 bytes long
// Return MAXIMUM_NUMBER_OF_PWD if there is no enough memory
// Return the new password id if success.
uint8_t	pwd_list_add_pwd						(uint8_t* name, uint8_t* data, uint8_t* usrName);	// Return 1 if success, 0 if memory is full

void	pwd_list_sort_use						(void);
void	pwd_list_sort_alpha						(void);



#endif // PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
