#ifndef STRING_HEADER_THOMAS_CHEVALIER
#define STRING_HEADER_THOMAS_CHEVALIER

#include <avr/pgmspace.h>

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef DEFINE_VARIABLES
#define EXTERN
#define EQUAL(v) = v
#else
#define EXTERN extern
#define EQUAL(v)
#endif /* DEFINE_VARIABLE */

#define REGISTER_STRING(strName, strValue, strLenght) EXTERN const char strName [strLenght] PROGMEM EQUAL(strValue)

REGISTER_STRING(str_error_card, "Erreur : carte inconnue", 23);
REGISTER_STRING(str_error_auth, "Erreur d'authentification", 25);
REGISTER_STRING(str_error_read, "Erreur de lecture", 17);
REGISTER_STRING(str_error_pwd, "Mauvais mot de passe", 20);

REGISTER_STRING(str_option_genNew, "Regenerer", 9);
REGISTER_STRING(str_option_changePwd, "Changer le mot de passe", 23);
REGISTER_STRING(str_option_changeUsr, "Changer l'identifiant", 21);
REGISTER_STRING(str_option_delPwd, "Supprimer", 9);
REGISTER_STRING(str_option_addPwd, "Nouveau mot de passe", 20);
REGISTER_STRING(str_option_changeMasterKey, "Changer la clef maitre", 22);

REGISTER_STRING(str_misc_changePwd, "Approchez votre carte", 21);

extern char str_buffer[26];

void str_to_buffer(const char* str_id);


#endif // STRING_HEADER_THOMAS_CHEVALIER
