#ifndef STRING_HEADER_THOMAS_CHEVALIER
#define STRING_HEADER_THOMAS_CHEVALIER

#include <avr/pgmspace.h>

#define DECLARE_STRING(strName) extern const char strName [] PROGMEM; \
										  extern const uint8_t strName ## _index ;

#define DEFINE_STRING(strName, strValue, strIndex) const char strName [] PROGMEM = strValue; \
										 const uint8_t strName ## _index = strIndex;


DECLARE_STRING(str_error_card);
DECLARE_STRING(str_error_auth);
DECLARE_STRING(str_error_read);
DECLARE_STRING(str_error_pwd);

DECLARE_STRING(str_option_regen);
DECLARE_STRING(str_option_changePwd);
DECLARE_STRING(str_option_changeUsr);
DECLARE_STRING(str_option_delPwd);
DECLARE_STRING(str_option_addPwd);
DECLARE_STRING(str_option_changeMasterKey);
DECLARE_STRING(str_option_changeOrientation);

DECLARE_STRING(str_misc_approachCard);
DECLARE_STRING(str_misc_yes);
DECLARE_STRING(str_misc_no);
DECLARE_STRING(str_misc_generateKey);
DECLARE_STRING(str_misc_updateEncryptVerif);
DECLARE_STRING(str_misc_hereIsNewKey);

DECLARE_STRING(str_action_executing);
DECLARE_STRING(str_action_finished);

DECLARE_STRING(str_usrsetup_welcome); 
DECLARE_STRING(str_usrsetup_note); 
DECLARE_STRING(str_usrsetup_warning); 
DECLARE_STRING(str_usrsetup_confirm); 
DECLARE_STRING(str_usrsetup_eraseMem);

DECLARE_STRING(str_order_typePwdName);
DECLARE_STRING(str_order_typePwdData);
DECLARE_STRING(str_order_typeUsrName);

DECLARE_STRING(str_recovery_title);
DECLARE_STRING(str_recovery_choice1);
DECLARE_STRING(str_recovery_choice2);

extern PGM_P const string_table[] PROGMEM;

extern char str_buffer[26];

void str_to_buffer(uint8_t index);


#endif // STRING_HEADER_THOMAS_CHEVALIER
