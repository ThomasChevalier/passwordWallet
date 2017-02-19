#ifndef PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
#define PASSWORD_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Goto the first password, depending upon the sort method
void goto_first_pwd(void);
// Return the ID of the password before pwd_index. Loop if necessary
uint16_t prev_pwd(uint16_t pwd_index);
// Return the ID of the password after pwd_index. Loop if necessary
uint16_t next_pwd(uint16_t pwd_index);

// Read the all password names.
void read_all_names(void);

void delete_password(void);

// Add a new password
// Password name should be 32 bytes long
// Password data should be 32 bytes long
// User name     should be 64 bytes long
// Return 1 if there is no enough memory
// Return 0 if success.
uint8_t add_password(char* passwordName, char* passwordData, char* userName);



#endif // PASSWORD_LIST_HEADER_THOMAS_CHEVALIER
