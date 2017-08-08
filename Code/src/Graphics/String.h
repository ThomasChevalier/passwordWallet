
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
DECLARE_STRING(str_error_not_enough_memory);
DECLARE_STRING(str_error_cannot_add_pwd);

DECLARE_STRING(str_misc_approachCard);
DECLARE_STRING(str_misc_generateKey);
DECLARE_STRING(str_misc_updateEncryptVerif);
DECLARE_STRING(str_misc_updatePwd);
DECLARE_STRING(str_misc_hereIsKey);

DECLARE_STRING(str_recovery_eraseMem);

DECLARE_STRING(str_order_typePwdName);
DECLARE_STRING(str_order_typePwdData);
DECLARE_STRING(str_order_typeUsrName);

DECLARE_STRING(str_recovery_title);
DECLARE_STRING(str_recovery_choice1);
DECLARE_STRING(str_recovery_choice2);

DECLARE_STRING(str_self_test_fram_ok);
DECLARE_STRING(str_self_test_fram_fail);
DECLARE_STRING(str_self_test_rfid_ok);
DECLARE_STRING(str_self_test_rfid_fail);
DECLARE_STRING(str_self_test_button_ok);
DECLARE_STRING(str_self_test_button_fail);
DECLARE_STRING(str_self_test_fail);

DECLARE_STRING(str_communication_what);
DECLARE_STRING(str_communication_dont_unplug);
DECLARE_STRING(str_communication_end);
DECLARE_STRING(str_communication_unplug);

DECLARE_STRING(str_reset_external);
DECLARE_STRING(str_reset_brownout);
DECLARE_STRING(str_reset_watchdog);
DECLARE_STRING(str_reset_jtag);

DECLARE_STRING(str_option_password);
DECLARE_STRING(str_option_sort);
DECLARE_STRING(str_option_advanced);
DECLARE_STRING(str_option_addPwd);
DECLARE_STRING(str_option_changeOrientation);

DECLARE_STRING(str_option_pwd_regenerate);
DECLARE_STRING(str_option_pwd_changePwd);
DECLARE_STRING(str_option_pwd_changeUsrName);
DECLARE_STRING(str_option_pwd_changePwdName);
DECLARE_STRING(str_option_pwd_delete);

DECLARE_STRING(str_option_sort_noSorting);
DECLARE_STRING(str_option_sort_byUsage);
DECLARE_STRING(str_option_sort_alphabetical);

DECLARE_STRING(str_option_advanced_changeKey);
DECLARE_STRING(str_option_advanced_disableEncryption);
DECLARE_STRING(str_option_advanced_enableEncryption);
DECLARE_STRING(str_option_advanced_showKey);
DECLARE_STRING(str_option_advanced_keyboardDisposition);
DECLARE_STRING(str_option_advanced_selfTest);
DECLARE_STRING(str_option_advanced_doFullReset);

extern PGM_P const string_table[] PROGMEM;

extern char str_buffer[26];

void str_to_buffer(uint8_t index);


#endif // STRING_HEADER_THOMAS_CHEVALIER
