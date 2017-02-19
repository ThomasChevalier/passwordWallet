#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void 		password_read_data				(uint8_t pwd_id, uint8_t* dst, uint8_t* key);
void		password_read_usr_name			(uint8_t pwd_id, uint8_t* dst, uint8_t* key);
void 		password_read_name				(uint8_t pwd_id, uint8_t* dst);
uint8_t  	password_read_prev_pwd_util		(uint8_t pwd_id);
uint8_t  	password_read_next_pwd_util		(uint8_t pwd_id);
uint8_t  	password_read_prev_pwd_alpha	(uint8_t pwd_id);
uint8_t  	password_read_next_pwd_alpha	(uint8_t pwd_id);
uint16_t 	password_read_counter			(uint8_t pwd_id);

void		password_set_data				(uint8_t pwd_id, uint8_t* pwd, 	uint8_t lenght, uint8_t* key);
void		password_set_usr_name			(uint8_t pwd_id, uint8_t* usr,	uint8_t lenght, uint8_t* key);
void		password_set_name				(uint8_t pwd_id, uint8_t* name, uint8_t lenght);
void		password_set_prev_pwd_util		(uint8_t pwd_id, uint8_t  val);
void		password_set_next_pwd_util		(uint8_t pwd_id, uint8_t  val);
void		password_set_prev_pwd_alpha		(uint8_t pwd_id, uint8_t  val);
void		password_set_next_pwd_alpha		(uint8_t pwd_id, uint8_t  val);
void		password_set_counter			(uint8_t pwd_id, uint16_t val);

void 		password_increment_counter		(uint8_t pwd_id);
void 		password_regenerate				(uint8_t pwd_id);

#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
