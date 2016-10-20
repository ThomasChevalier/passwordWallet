#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include "Globals.h"
#include "Fram.h"


void read_password();
void goto_first_password();
void goto_prev_pwd();
void goto_next_pwd();
void increment_pwd_counter();

void generate_password(char* output);
void change_password();
void change_username();
void delete_password();
void add_password();


#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
