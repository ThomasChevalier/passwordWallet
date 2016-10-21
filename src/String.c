#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "Erreur : carte inconnue", 23);
DEFINE_STRING(str_error_auth, "Erreur d'authentification", 25);
DEFINE_STRING(str_error_read, "Erreur de lecture", 17);
DEFINE_STRING(str_error_pwd, "Mauvais mot de passe", 20);

DEFINE_STRING(str_option_genNew, "Regenerer", 9);
DEFINE_STRING(str_option_changePwd, "Changer le mot de passe", 23);
DEFINE_STRING(str_option_changeUsr, "Changer l'identifiant", 21);
DEFINE_STRING(str_option_delPwd, "Supprimer", 9);
DEFINE_STRING(str_option_addPwd, "Nouveau mot de passe", 20);
DEFINE_STRING(str_option_changeMasterKey, "Changer la clef maitre", 22);

DEFINE_STRING(str_misc_changePwd, "Approchez votre carte", 21);


void str_to_buffer(const char* str_id)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(str_id))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
