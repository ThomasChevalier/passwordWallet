#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "Erreur : carte inconnue", 23);
DEFINE_STRING(str_error_auth, "Erreur d'authentification", 25);
DEFINE_STRING(str_error_read, "Erreur de lecture", 17);
DEFINE_STRING(str_error_pwd, "Mauvais mot de passe", 20);

DEFINE_STRING(str_option_regen, "REGENERER", 9);
DEFINE_STRING(str_option_changePwd, "CHANGER LE MOT DE PASSE", 23);
DEFINE_STRING(str_option_changeUsr, "CHANGER L'IDENTIFIANT", 21);
DEFINE_STRING(str_option_delPwd, "SUPPRIMER", 9);
DEFINE_STRING(str_option_addPwd, "NOUVEAU MOT DE PASSE", 20);
DEFINE_STRING(str_option_changeMasterKey, "CHANGER LA CLEF MAITRE", 22);

DEFINE_STRING(str_misc_approachCard, "Approchez votre carte", 21);

PGM_P const string_table[] PROGMEM = 
{
	str_error_card,
	str_error_auth,
	str_error_read,
	str_error_pwd,

	str_option_regen,
	str_option_changePwd,
	str_option_changeUsr,
	str_option_delPwd,
	str_option_addPwd,
	str_option_changeMasterKey,

	str_misc_approachCard
};

void str_to_buffer(uint8_t index)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
