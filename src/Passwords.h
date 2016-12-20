#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include "Globals.h"
#include "Fram.h"

// Read the current password (CURRENT_PASSWORD_ID), from fram. This function decrypt the data and store the plain pwd in ram
void read_password();
// Set a new password for CURRENT_PASSWORD_ID. password should be an array of char, without encryption and without padding
void set_password(uint8_t* password, uint8_t pwd_len);

// Set a new password for CURRENT_PASSWORD_ID. usr_name should be an array of char, without encryption and without padding
void set_username(uint8_t* usr_name, uint8_t usr_len);

// Goto the first password, depending upon the sort method
void goto_first_pwd();
// Return the ID of the password before pwd_index. Loop if necessary
uint16_t prev_pwd(uint16_t pwd_index);
// Return the ID of the password after pwd_index. Loop if necessary
uint16_t next_pwd(uint16_t pwd_index);

// Increment the counter of utilisation of the current password. Should be called each time a password is used.
void increment_pwd_counter();

// Set a new name for the current password. pwd_name should be less than 32 byte long
void set_pwd_name(char* pwd_name);
// Return a 32 byte array with the name of the password.
void read_pwd_name(char* pwd_name, uint16_t pwd_index);

// Read, decrypt and store the current user name in ram.
void read_usr_name();
// Read the all password names.
void read_all_names();

// Return a 32 character random password. This function may be blockant.
void generate_password(char* output);

void change_password();
void change_username();
void delete_password();

// Add a new password
// Password name should be 32 bytes long
// Password data should be 32 bytes long
// User name     should be 64 bytes long
// Return 1 if there is no enough memory
// Return 0 if success.
uint8_t add_password(char* passwordName, char* passwordData, char* userName);



#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
