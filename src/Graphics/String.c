#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card, "ERREUR : CARTE INCONNUE", 0);
DEFINE_STRING(str_error_auth, "ERREUR D'AUTHENTIFICATION", 1);
DEFINE_STRING(str_error_read, "ERREUR DE LECTURE", 2);
DEFINE_STRING(str_error_pwd, "MAUVAIS MOT DE PASSE", 3);

DEFINE_STRING(str_misc_approachCard, "APPROCHEZ VOTRE CARTE", 4);
DEFINE_STRING(str_misc_yes, "OUI", 5);
DEFINE_STRING(str_misc_no, "NON", 6);
DEFINE_STRING(str_misc_generateKey, "GENERATION DE LA CLEF", 7);
DEFINE_STRING(str_misc_updateEncryptVerif, "M.A.J. DE LA VALIDATION", 8);
DEFINE_STRING(str_misc_hereIsKey, "VOICI VOTRE CLEF", 9);

DEFINE_STRING(str_action_executing, "Action en cours ...", 10);
DEFINE_STRING(str_action_finished, "Fait !", 11);

DEFINE_STRING(str_usrsetup_welcome, "Il semble que c'est la premiere fois que vous utilisez cet appareil. Souhaitez vous le configurer ?", 12); 
DEFINE_STRING(str_usrsetup_note, "Ce message peut s'afficher si vous voulez formater l'appareil.", 13); 
DEFINE_STRING(str_usrsetup_warning, "ATTENTION : TOUTE LES DONNEES SERONT PERDUS !", 14); 
DEFINE_STRING(str_usrsetup_confirm, "Pressez successivement haut, bas, gauche, droite, pour continuer.", 15); 
DEFINE_STRING(str_usrsetup_eraseMem, "FORMATAGE MEMOIRE", 16);

DEFINE_STRING(str_order_typePwdName, "ENTREZ NOM MOT DE PASSE", 17); 
DEFINE_STRING(str_order_typePwdData, "ENTREZ MOT DE PASSE", 18); 
DEFINE_STRING(str_order_typeUsrName, "ENTREZ NOM UTILISATEUR", 19);

DEFINE_STRING(str_recovery_title, "MODE DE RECUPERATION", 20);
DEFINE_STRING(str_recovery_choice1, "1 - FORMATAGE COMPLET", 21);
DEFINE_STRING(str_recovery_choice2, "2 - ENTREZ LA CLEF", 22);

DEFINE_STRING(str_self_test_fram_ok, "MEMOIRE OK", 23);
DEFINE_STRING(str_self_test_fram_fail, "MEMOIRE HS", 24);
DEFINE_STRING(str_self_test_rfid_ok, "RFID OK", 25);
DEFINE_STRING(str_self_test_rfid_fail, "RFID HS", 26);
DEFINE_STRING(str_self_test_button_ok, "BOUTON OK", 27);
DEFINE_STRING(str_self_test_button_fail, "BOUTON HS", 28);

PGM_P const string_table[] PROGMEM = 
{
	str_error_card,
	str_error_auth,
	str_error_read,
	str_error_pwd,

	str_misc_approachCard,
	str_misc_yes,
	str_misc_no,
	str_misc_generateKey,
	str_misc_updateEncryptVerif,
	str_misc_hereIsKey,

	str_action_executing,
	str_action_finished,

	str_usrsetup_welcome,
	str_usrsetup_note,
	str_usrsetup_warning,
	str_usrsetup_confirm,
	str_usrsetup_eraseMem,

	str_order_typePwdName,
	str_order_typePwdData,
	str_order_typeUsrName,

	str_recovery_title,
	str_recovery_choice1,
	str_recovery_choice2,

	str_self_test_fram_ok,
	str_self_test_fram_fail,
	str_self_test_rfid_ok,
	str_self_test_rfid_fail,
	str_self_test_button_ok,
	str_self_test_button_fail
};

DEFINE_STRING(str_option_password, "MOT DE PASSE ...", 0);
DEFINE_STRING(str_option_sort	, "TRI ...", 1);
DEFINE_STRING(str_option_advanced, "AVANCEE ...", 2);
DEFINE_STRING(str_option_addPwd, "NOUVEAU MOT DE PASSE", 3);
DEFINE_STRING(str_option_changeOrientation, "RETOURNER L'AFFICHAGE", 4);

DEFINE_STRING(str_option_pwd_regenerate, "REGENERER", 5);
DEFINE_STRING(str_option_pwd_changePwd, "CHANGER MOT DE PASSE", 6);
DEFINE_STRING(str_option_pwd_changeUsrName, "CHANGER IDENTIFIANT", 7);
DEFINE_STRING(str_option_pwd_changePwdName, "CHANGER NOM", 8);
DEFINE_STRING(str_option_pwd_delete, "SUPPRIMER", 9);

DEFINE_STRING(str_option_sort_noSorting, "PAS DE TRI", 10);
DEFINE_STRING(str_option_sort_byUsage, "PAR UTILISATION", 11);
DEFINE_STRING(str_option_sort_alphabetical, "ALPHABETIQUE", 12);

DEFINE_STRING(str_option_advanced_changeKey, "CHANGER CLEF MAITRE", 13);
DEFINE_STRING(str_option_advanced_disableEncryption, "DESACTIVER CRYPTAGE", 14);
DEFINE_STRING(str_option_advanced_enableEncryption, "ACTIVER CRYPTAGE", 15);
DEFINE_STRING(str_option_advanced_showKey, "MONTRER CLEF MAITRE", 16);
DEFINE_STRING(str_option_advanced_selfTest, "SELF TEST", 17);
DEFINE_STRING(str_option_advanced_doFullReset, "FORMATER", 18);


PGM_P const string_options_table[] PROGMEM = 
{
	str_option_password,
	str_option_sort,
	str_option_advanced,
	str_option_addPwd,
	str_option_changeOrientation,

	str_option_pwd_regenerate,
	str_option_pwd_changePwd,
	str_option_pwd_changeUsrName,
	str_option_pwd_changePwdName,
	str_option_pwd_delete,

	str_option_sort_noSorting,
	str_option_sort_byUsage,
	str_option_sort_alphabetical,

	str_option_advanced_changeKey,
	str_option_advanced_disableEncryption,
	str_option_advanced_enableEncryption,
	str_option_advanced_showKey,
	str_option_advanced_selfTest,
	str_option_advanced_doFullReset
};

void str_option_to_buffer(uint8_t index)
{
	strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_options_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}

void str_to_buffer(uint8_t index)
{
	strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
