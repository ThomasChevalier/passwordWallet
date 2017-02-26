#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];

DEFINE_STRING(str_error_card,				"ERREUR : carte inconnue",		0	);
DEFINE_STRING(str_error_auth,				"ERREUR d'authentification",	1	);
DEFINE_STRING(str_error_read,				"ERREUR de lecture",			2	);
DEFINE_STRING(str_error_pwd,				"Mauvais mot de passe",			3	);
DEFINE_STRING(str_error_not_enough_memory,	"Memoire insuffisante",			4	);
DEFINE_STRING(str_error_cannot_add_pwd,		"Ajout impossible",				5	);
	
DEFINE_STRING(str_misc_approachCard,		"Approchez votre carte", 		6	);
DEFINE_STRING(str_misc_generateKey,			"Generation de la clef",		7	);
DEFINE_STRING(str_misc_updateEncryptVerif,	"M.a.j. de la validation",		8	);
DEFINE_STRING(str_misc_updatePwd,			"M.a.j. des mots de passe", 	9	);
DEFINE_STRING(str_misc_hereIsKey,			"Voici votre clef",				10	);
		
DEFINE_STRING(str_order_typePwdName,		"ENTREZ NOM MOT DE PASSE",		11	); 
DEFINE_STRING(str_order_typePwdData,		"ENTREZ MOT DE PASSE",			12	); 
DEFINE_STRING(str_order_typeUsrName,		"ENTREZ NOM UTILISATEUR",		13	);
		
DEFINE_STRING(str_recovery_eraseMem,		"FORMATAGE MEMOIRE",			14	);
DEFINE_STRING(str_recovery_title,			"MODE DE RECUPERATION",			15	);
DEFINE_STRING(str_recovery_choice1,			"1 - FORMATAGE COMPLET",		16	);
DEFINE_STRING(str_recovery_choice2,			"2 - ENTREZ LA CLEF",			17	);
	
DEFINE_STRING(str_self_test_fram_ok,		"MEMOIRE OK",					18	);
DEFINE_STRING(str_self_test_fram_fail,		"MEMOIRE HS",					19	);
DEFINE_STRING(str_self_test_rfid_ok,		"RFID OK",						20	);
DEFINE_STRING(str_self_test_rfid_fail,		"RFID HS",						21	);
DEFINE_STRING(str_self_test_button_ok,		"BOUTON OK",					22	);
DEFINE_STRING(str_self_test_button_fail,	"BOUTON HS",					23	);
DEFINE_STRING(str_self_test_fail,			"ERREUR : appareil HS",			24	);

PGM_P const string_table[] PROGMEM = 
{
	str_error_card,
	str_error_auth,
	str_error_read,
	str_error_pwd,
	str_error_not_enough_memory,
	str_error_cannot_add_pwd,

	str_misc_approachCard,
	str_misc_generateKey,
	str_misc_updateEncryptVerif,
	str_misc_updatePwd,
	str_misc_hereIsKey,
	
	str_order_typePwdName,
	str_order_typePwdData,
	str_order_typeUsrName,

	str_recovery_eraseMem,
	str_recovery_title,
	str_recovery_choice1,
	str_recovery_choice2,

	str_self_test_fram_ok,
	str_self_test_fram_fail,
	str_self_test_rfid_ok,
	str_self_test_rfid_fail,
	str_self_test_button_ok,
	str_self_test_button_fail,
	str_self_test_fail
};

DEFINE_STRING(str_option_password, "Mot de passe ...", 0);
DEFINE_STRING(str_option_sort	, "Tri ...", 1);
DEFINE_STRING(str_option_advanced, "Plus d'options ...", 2);
DEFINE_STRING(str_option_addPwd, "Nouveau mot de passe", 3);
DEFINE_STRING(str_option_changeOrientation, "Retourner l'affichage", 4);

DEFINE_STRING(str_option_pwd_regenerate, "Regenerer", 5);
DEFINE_STRING(str_option_pwd_changePwd, "Changer mot de passe", 6);
DEFINE_STRING(str_option_pwd_changeUsrName, "Changer identifiant", 7);
DEFINE_STRING(str_option_pwd_changePwdName, "Changer nom", 8);
DEFINE_STRING(str_option_pwd_delete, "Supprimer", 9);

DEFINE_STRING(str_option_sort_noSorting, "Pas de tri", 10);
DEFINE_STRING(str_option_sort_byUsage, "Par utilisation", 11);
DEFINE_STRING(str_option_sort_alphabetical, "Alphabetique", 12);

DEFINE_STRING(str_option_advanced_changeKey, "Changer clef maitre", 13);
DEFINE_STRING(str_option_advanced_disableEncryption, "Desactiver cryptage", 14);
DEFINE_STRING(str_option_advanced_enableEncryption, "Activer cryptage", 15);
DEFINE_STRING(str_option_advanced_showKey, "Montrer clef maitre", 16);
DEFINE_STRING(str_option_advanced_selfTest, "Auto test", 17);
DEFINE_STRING(str_option_advanced_doFullReset, "FORMATER", 18);
DEFINE_STRING(str_option_advanced_communicate, "SERIAL", 19);


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
	str_option_advanced_doFullReset,
	str_option_advanced_communicate
};

void str_option_to_buffer(uint8_t index)
{
	strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_options_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}

void str_to_buffer(uint8_t index)
{
	strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(string_table[index]))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
