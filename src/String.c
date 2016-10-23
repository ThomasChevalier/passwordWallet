#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "Erreur : carte inconnue");
DEFINE_STRING(str_error_auth, "Erreur d'authentification");
DEFINE_STRING(str_error_read, "Erreur de lecture");
DEFINE_STRING(str_error_pwd, "Mauvais mot de passe");

DEFINE_STRING(str_option_regen, "REGENERER");
DEFINE_STRING(str_option_changePwd, "CHANGER LE MOT DE PASSE");
DEFINE_STRING(str_option_changeUsr, "CHANGER L'IDENTIFIANT");
DEFINE_STRING(str_option_delPwd, "SUPPRIMER");
DEFINE_STRING(str_option_addPwd, "NOUVEAU MOT DE PASSE");
DEFINE_STRING(str_option_changeMasterKey, "CHANGER LA CLEF MAITRE");
DEFINE_STRING(str_option_changeOrientation, "RETOURNER L'AFFICHAGE");

DEFINE_STRING(str_misc_approachCard, "Approchez votre carte");
DEFINE_STRING(str_misc_yes, "OUI");
DEFINE_STRING(str_misc_no, "NON");

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
	str_option_changeOrientation,

	str_misc_approachCard,
	str_misc_yes,
	str_misc_no
};

void str_to_buffer(uint8_t index)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
