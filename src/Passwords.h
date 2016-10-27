#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include "Globals.h"
#include "Fram.h"


void read_password();
void set_password(uint8_t* password, uint8_t pwd_len);

void goto_first_pwd();
uint16_t prev_pwd(uint16_t pwd_index);
uint16_t next_pwd(uint16_t pwd_index);

void increment_pwd_counter();

void set_pwd_name(char* pwd_name);
void read_pwd_name(char* pwd_name, uint16_t pwd_index);

void read_usr_name();
void read_all_names();

void generate_password(char* output);
void change_password();
void change_username();
void delete_password();
void add_password();



#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
