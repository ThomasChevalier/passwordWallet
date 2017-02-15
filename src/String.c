#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "ERREUR : CARTE INCONNUE", 0);
DEFINE_STRING(str_error_auth, "ERREUR D'AUTHENTIFICATION", 1);
DEFINE_STRING(str_error_read, "ERREUR DE LECTURE", 2);
DEFINE_STRING(str_error_pwd, "MAUVAIS MOT DE PASSE", 3);

DEFINE_STRING(str_option_regen, "REGENERER", 4);
DEFINE_STRING(str_option_changePwd	, "CHANGER LE MOT DE PASSE", 5);
DEFINE_STRING(str_option_changeUsr, "CHANGER L'IDENTIFIANT", 6);
DEFINE_STRING(str_option_delPwd, "SUPPRIMER", 7);
DEFINE_STRING(str_option_addPwd, "NOUVEAU MOT DE PASSE", 8);
DEFINE_STRING(str_option_changeMasterKey, "CHANGER LA CLEF MAITRE", 9);
DEFINE_STRING(str_option_changeOrientation, "RETOURNER L'AFFICHAGE", 10);

DEFINE_STRING(str_misc_approachCard, "APPROCHEZ VOTRE CARTE", 11);
DEFINE_STRING(str_misc_yes, "OUI", 12);
DEFINE_STRING(str_misc_no, "NON", 13);
DEFINE_STRING(str_misc_generateKey, "GENERATION DE LA CLEF", 14);
DEFINE_STRING(str_misc_updateEncryptVerif, "M.A.J. DE LA VALIDATION", 15);
DEFINE_STRING(str_misc_hereIsNewKey, "VOICI VOTRE NOUVELLE CLEF", 16);

DEFINE_STRING(str_action_executing, "Action en cours ...", 17);
DEFINE_STRING(str_action_finished, "Fait !", 18);

DEFINE_STRING(str_usrsetup_welcome, "Il semble que c'est la premiere fois que vous utilisez cet appareil. Souhaitez vous le configurer ?", 19); 
DEFINE_STRING(str_usrsetup_note, "Ce message peut s'afficher si vous voulez formater l'appareil.", 20); 
DEFINE_STRING(str_usrsetup_warning, "ATTENTION : TOUTE LES DONNEES SERONT PERDUS !", 21); 
DEFINE_STRING(str_usrsetup_confirm, "Pressez successivement haut, bas, gauche, droite, pour continuer.", 22); 
DEFINE_STRING(str_usrsetup_eraseMem, "FORMATAGE MEMOIRE", 23);

DEFINE_STRING(str_order_typePwdName, "ENTREZ NOM MOT DE PASSE", 24); 
DEFINE_STRING(str_order_typePwdData, "ENTREZ MOT DE PASSE", 25); 
DEFINE_STRING(str_order_typeUsrName, "ENTREZ NOM UTILISATEUR", 26); 

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
	str_misc_generateKey,
	str_misc_updateEncryptVerif,
	str_misc_hereIsNewKey,

	str_action_executing,
	str_action_finished,

	str_usrsetup_welcome,
	str_usrsetup_note,
	str_usrsetup_warning,
	str_usrsetup_confirm,
	str_usrsetup_eraseMem,

	str_order_typePwdName,
	str_order_typePwdData,
	str_order_typeUsrName
};

void str_to_buffer(uint8_t index)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
