#ifndef STRING_HEADER_THOMAS_CHEVALIER
#define STRING_HEADER_THOMAS_CHEVALIER

#include <avr/pgmspace.h>

#define DECLARE_STRING(strName, strValue) extern const char strName [] PROGMEM
#define DEFINE_STRING(strName, strValue) const char strName [] PROGMEM = strValue

#define STRING_ERROR_CARD 0
DECLARE_STRING(str_error_card, "Erreur : carte inconnue");
#define STRING_ERROR_AUTH 1
DECLARE_STRING(str_error_auth, "Erreur d'authentification");
#define STRING_ERROR_READ 2
DECLARE_STRING(str_error_read, "Erreur de lecture");
#define STRING_ERROR_PWD 3
DECLARE_STRING(str_error_pwd, "Mauvais mot de passe");

#define STRING_OPTION_REGEN 4
DECLARE_STRING(str_option_regen, "Regenerer");
#define STRING_OPTION_CHANGE_PWD 5
DECLARE_STRING(str_option_changePwd, "Changer le mot de passe");
#define STRING_OPTION_CHANGE_USR 6
DECLARE_STRING(str_option_changeUsr, "Changer l'identifiant");
#define STRING_OPTION_DEL 7
DECLARE_STRING(str_option_delPwd, "Supprimer");
#define STRING_OPTION_ADD 8
DECLARE_STRING(str_option_addPwd, "Nouveau mot de passe");
#define STRING_OPTION_CHANGE_MASTER_KEY 9
DECLARE_STRING(str_option_changeMasterKey, "Changer la clef maitre");
#define STRING_OPTION_ROTATION 10
DECLARE_STRING(str_option_changeOrientation, "Retourner l'affichage");

#define STRING_MISC_APPROACH_CARD 11
DECLARE_STRING(str_misc_approachCard, "Approchez votre carte");
#define STRING_MISC_YES 12
DECLARE_STRING(str_misc_yes, "OUI");
#define STRING_MISC_NO 13
DECLARE_STRING(str_misc_no, "NON");

extern PGM_P const string_table[] PROGMEM;

extern char str_buffer[26];

void str_to_buffer(uint8_t index);


#endif // STRING_HEADER_THOMAS_CHEVALIER
