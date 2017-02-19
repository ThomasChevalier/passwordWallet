#ifndef PASSWORDS_HEADER_THOMAS_CHEVALIER
#define PASSWORDS_HEADER_THOMAS_CHEVALIER

#include "../Globals.h"

// Only use it when the device is not encrypted, copy the content of the key stored in eeprom in ram
void no_encryption_copy_key_from_eeprom(void);

// Disable encryption of the device
void disable_encryption(void);

// Enable encryption of the device
// Don't generate a new key, change_master_key should be called first.
// This function erase the key from eeprom and update flags
void enable_encryption(void);

// Check the KEY buffer against a hash stored in eeprom, should return 1 if key is correct.
uint8_t check_key(void);

// Change the encryption key for all the passwords (decrypt with KEY and encrypt with new_key)
void update_encryption_with(uint8_t *new_key);

// Read the current password (CURRENT_PASSWORD_ID), from fram. This function decrypt the data and store the plain pwd in ram
void read_password(uint8_t* key);
// Set a new password for CURRENT_PASSWORD_ID. password should be an array of char, without encryption and without padding
void set_password(uint8_t* password, uint8_t pwd_len, uint8_t* key);

// Read, decrypt and store the current user name in ram.
void read_usr_name(uint8_t* key);

// Set a new password for CURRENT_PASSWORD_ID. usr_name should be an array of char, without encryption and without padding
void set_username(uint8_t* usr_name, uint8_t usr_len, uint8_t* key);

// Goto the first password, depending upon the sort method
void goto_first_pwd(void);
// Return the ID of the password before pwd_index. Loop if necessary
uint16_t prev_pwd(uint16_t pwd_index);
// Return the ID of the password after pwd_index. Loop if necessary
uint16_t next_pwd(uint16_t pwd_index);

// Increment the counter of utilisation of the current password. Should be called each time a password is used.
void increment_pwd_counter(void);

// Set a new name for the current password. pwd_name should be less than 32 byte long
void set_pwd_name(char* pwd_name);
// Return a 32 byte array with the name of the password.
void read_pwd_name(char* pwd_name, uint16_t pwd_index);

// Read the all password names.
void read_all_names(void);

// Return a 32 character random password. This function may be blockant.
void generate_password(char* output);

void delete_password(void);

// Add a new password
// Password name should be 32 bytes long
// Password data should be 32 bytes long
// User name     should be 64 bytes long
// Return 1 if there is no enough memory
// Return 0 if success.
uint8_t add_password(char* passwordName, char* passwordData, char* userName);



#endif // PASSWORDS_HEADER_THOMAS_CHEVALIER
