#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "Erreur : carte inconnue", 0);
DEFINE_STRING(str_error_auth, "Erreur d'authentification", 1);
DEFINE_STRING(str_error_read, "Erreur de lecture", 2);
DEFINE_STRING(str_error_pwd, "Mauvais mot de passe", 3);

DEFINE_STRING(str_option_regen, "REGENERER", 4);
DEFINE_STRING(str_option_changePwd, "CHANGER LE MOT DE PASSE", 5);
DEFINE_STRING(str_option_changeUsr, "CHANGER L'IDENTIFIANT", 6);
DEFINE_STRING(str_option_delPwd, "SUPPRIMER", 7);
DEFINE_STRING(str_option_addPwd, "NOUVEAU MOT DE PASSE", 8);
DEFINE_STRING(str_option_changeMasterKey, "CHANGER LA CLEF MAITRE", 9);
DEFINE_STRING(str_option_changeOrientation, "RETOURNER L'AFFICHAGE", 10);

DEFINE_STRING(str_misc_approachCard, "Approchez votre carte", 11);
DEFINE_STRING(str_misc_yes, "OUI", 12);
DEFINE_STRING(str_misc_no, "NON", 13);

DEFINE_STRING(str_action_executing, "Action en cours ...", 14);
DEFINE_STRING(str_action_finished, "Fait !", 15);

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
	str_misc_no,

	str_action_executing,
	str_action_finished
};

void str_to_buffer(uint8_t index)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
