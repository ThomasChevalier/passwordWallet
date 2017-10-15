#include "eeprom.h"

// For keycodes
#include <LUFA/Drivers/USB/USB.h>

#include "../Globals.h"


Eeprom_init EEPROM_VARS = {


	// Version 2.0 (0x92)
	// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 self-test
	.rfid_firmware =  {
	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
	},


	#define SHIFT KEYCODE_MODIFIER_SHIFT
	#define ALTGR KEYCODE_MODIFIER_ALTGR
	// MDB is set to 1 if modifier is needed and to 0 otherwhise
	// Modifier == (1<<7) means that the left shift is pressed
	// Modifier == (1<<6) means that the right alt is pressed
	.qwerty_to_keycode_map=
	{
	/*
	*   |                   KEYCODE                         | MODIF |   ASCII   |
	*/

		HID_KEYBOARD_SC_SPACE                                       ,   /* space */
		HID_KEYBOARD_SC_1_AND_EXCLAMATION                   | SHIFT ,   /* ! */
		HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE                | SHIFT ,   /* " */
		HID_KEYBOARD_SC_3_AND_HASHMARK                      | SHIFT ,   /* # */
		HID_KEYBOARD_SC_4_AND_DOLLAR                        | SHIFT ,   /* $ */
		HID_KEYBOARD_SC_5_AND_PERCENTAGE                    | SHIFT ,   /* % */
		HID_KEYBOARD_SC_7_AND_AMPERSAND                     | SHIFT ,   /* & */
		HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE                        ,   /* ' */
		HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS           | SHIFT ,   /* ( */
		HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS           | SHIFT ,   /* ) */
		HID_KEYBOARD_SC_8_AND_ASTERISK                      | SHIFT ,   /* * */
		HID_KEYBOARD_SC_EQUAL_AND_PLUS                      | SHIFT ,   /* + */
		HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN                    ,   /* ,*/
		HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE                        ,   /* - */
		HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN                   ,   /* . */
		HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK                     ,   /* / */
		HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS                   ,   /* 0 */
		HID_KEYBOARD_SC_1_AND_EXCLAMATION                           ,   /* 1 */
		HID_KEYBOARD_SC_2_AND_AT                                    ,   /* 2 */
		HID_KEYBOARD_SC_3_AND_HASHMARK                              ,   /* 3 */
		HID_KEYBOARD_SC_4_AND_DOLLAR                                ,   /* 4 */
		HID_KEYBOARD_SC_5_AND_PERCENTAGE                            ,   /* 5 */
		HID_KEYBOARD_SC_6_AND_CARET                                 ,   /* 6 */
		HID_KEYBOARD_SC_7_AND_AMPERSAND                             ,   /* 7 */
		HID_KEYBOARD_SC_8_AND_ASTERISK                              ,   /* 8 */
		HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS                   ,   /* 9 */
		HID_KEYBOARD_SC_SEMICOLON_AND_COLON                 | SHIFT ,   /* : */
		HID_KEYBOARD_SC_SEMICOLON_AND_COLON                         ,   /* ; */
		HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN            | SHIFT ,   /* < */
		HID_KEYBOARD_SC_EQUAL_AND_PLUS                              ,   /* = */
		HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN           | SHIFT ,   /* > */
		HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK             | SHIFT ,   /* ? */
		HID_KEYBOARD_SC_2_AND_AT                            | SHIFT ,   /* @ */
		HID_KEYBOARD_SC_A                                   | SHIFT ,   /* A */
		HID_KEYBOARD_SC_B                                   | SHIFT ,   /* B */
		HID_KEYBOARD_SC_C                                   | SHIFT ,   /* C */
		HID_KEYBOARD_SC_D                                   | SHIFT ,   /* D */
		HID_KEYBOARD_SC_E                                   | SHIFT ,   /* E */
		HID_KEYBOARD_SC_F                                   | SHIFT ,   /* F */
		HID_KEYBOARD_SC_G                                   | SHIFT ,   /* G */
		HID_KEYBOARD_SC_H                                   | SHIFT ,   /* H */
		HID_KEYBOARD_SC_I                                   | SHIFT ,   /* I */
		HID_KEYBOARD_SC_J                                   | SHIFT ,   /* J */
		HID_KEYBOARD_SC_K                                   | SHIFT ,   /* K */
		HID_KEYBOARD_SC_L                                   | SHIFT ,   /* L */
		HID_KEYBOARD_SC_M                                   | SHIFT ,   /* M */
		HID_KEYBOARD_SC_N                                   | SHIFT ,   /* N */
		HID_KEYBOARD_SC_O                                   | SHIFT ,   /* O */
		HID_KEYBOARD_SC_P                                   | SHIFT ,   /* P */
		HID_KEYBOARD_SC_Q                                   | SHIFT ,   /* Q */
		HID_KEYBOARD_SC_R                                   | SHIFT ,   /* R */
		HID_KEYBOARD_SC_S                                   | SHIFT ,   /* S */
		HID_KEYBOARD_SC_T                                   | SHIFT ,   /* T */
		HID_KEYBOARD_SC_U                                   | SHIFT ,   /* U */
		HID_KEYBOARD_SC_V                                   | SHIFT ,   /* V */
		HID_KEYBOARD_SC_W                                   | SHIFT ,   /* W */
		HID_KEYBOARD_SC_X                                   | SHIFT ,   /* X */
		HID_KEYBOARD_SC_Y                                   | SHIFT ,   /* Y */
		HID_KEYBOARD_SC_Z                                   | SHIFT ,   /* Z */
		HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE           ,   /* [ */
		HID_KEYBOARD_SC_BACKSLASH_AND_PIPE                          ,   /* \ */
		HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE           ,   /* ] */
		HID_KEYBOARD_SC_6_AND_CARET                         | SHIFT ,   /* ^ */
		HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE                | SHIFT ,   /* _ */
		HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE                   ,   /* ` */
		HID_KEYBOARD_SC_A                                           ,   /* a */
		HID_KEYBOARD_SC_B                                           ,   /* b */
		HID_KEYBOARD_SC_C                                           ,   /* c */
		HID_KEYBOARD_SC_D                                           ,   /* d */
		HID_KEYBOARD_SC_E                                           ,   /* e */
		HID_KEYBOARD_SC_F                                           ,   /* f */
		HID_KEYBOARD_SC_G                                           ,   /* g */
		HID_KEYBOARD_SC_H                                           ,   /* h */
		HID_KEYBOARD_SC_I                                           ,   /* i */
		HID_KEYBOARD_SC_J                                           ,   /* j */
		HID_KEYBOARD_SC_K                                           ,   /* k */
		HID_KEYBOARD_SC_L                                           ,   /* l */
		HID_KEYBOARD_SC_M                                           ,   /* m */
		HID_KEYBOARD_SC_N                                           ,   /* n */
		HID_KEYBOARD_SC_O                                           ,   /* o */
		HID_KEYBOARD_SC_P                                           ,   /* p */
		HID_KEYBOARD_SC_Q                                           ,   /* q */
		HID_KEYBOARD_SC_R                                           ,   /* r */
		HID_KEYBOARD_SC_S                                           ,   /* s */
		HID_KEYBOARD_SC_T                                           ,   /* t */
		HID_KEYBOARD_SC_U                                           ,   /* u */
		HID_KEYBOARD_SC_V                                           ,   /* v */
		HID_KEYBOARD_SC_W                                           ,   /* w */
		HID_KEYBOARD_SC_X                                           ,   /* x */
		HID_KEYBOARD_SC_Y                                           ,   /* y */
		HID_KEYBOARD_SC_Z                                           ,   /* z */
		HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE   | SHIFT ,   /* { */
		HID_KEYBOARD_SC_BACKSLASH_AND_PIPE                  | SHIFT ,   /*  | */
		HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE   | SHIFT ,   /* } */
		HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE           | SHIFT ,   /* ~ */
		0

		},

	.azerty_to_keycode_map=
	{
	/*
	*   |                   KEYCODE                         | MODIF |   ASCII   |
	*/

		HID_KEYBOARD_SC_SPACE                                       ,   /* space */
		HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK                     ,   /* ! */
		HID_KEYBOARD_SC_3_AND_HASHMARK                              ,   /* " */
		HID_KEYBOARD_SC_3_AND_HASHMARK                      | ALTGR ,   /* # */
		HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE           ,   /* $ */
		HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE                | SHIFT ,   /* % */
		HID_KEYBOARD_SC_1_AND_EXCLAMATION                           ,   /* & */
		HID_KEYBOARD_SC_4_AND_DOLLAR                                ,   /* ' */
		HID_KEYBOARD_SC_5_AND_PERCENTAGE                            ,   /* ( */
		HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE                        ,   /* ) */
		HID_KEYBOARD_SC_BACKSLASH_AND_PIPE                          ,   /* * */
		HID_KEYBOARD_SC_EQUAL_AND_PLUS                      | SHIFT ,   /* + */
		HID_KEYBOARD_SC_M                                           ,   /* , */
		HID_KEYBOARD_SC_6_AND_CARET                                 ,   /* - */
		HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN            | SHIFT ,   /* . */
		HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN           | SHIFT ,   /* / */
		HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS           | SHIFT ,   /* 0 */
		HID_KEYBOARD_SC_1_AND_EXCLAMATION                   | SHIFT ,   /* 1 */
		HID_KEYBOARD_SC_2_AND_AT                            | SHIFT ,   /* 2 */
		HID_KEYBOARD_SC_3_AND_HASHMARK                      | SHIFT ,   /* 3 */
		HID_KEYBOARD_SC_4_AND_DOLLAR                        | SHIFT ,   /* 4 */
		HID_KEYBOARD_SC_5_AND_PERCENTAGE                    | SHIFT ,   /* 5 */
		HID_KEYBOARD_SC_6_AND_CARET                         | SHIFT ,   /* 6 */
		HID_KEYBOARD_SC_7_AND_AMPERSAND                     | SHIFT ,   /* 7 */
		HID_KEYBOARD_SC_8_AND_ASTERISK                      | SHIFT ,   /* 8 */
		HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS           | SHIFT ,   /* 9 */
		HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN                   ,   /* : */
		HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN                    ,   /* ; */
		HID_KEYBOARD_SC_F3                                          ,   /* < */
		HID_KEYBOARD_SC_EQUAL_AND_PLUS                              ,   /* = */
		HID_KEYBOARD_SC_F3                                  | SHIFT ,   /* > */
		HID_KEYBOARD_SC_M                                   | SHIFT ,   /* ? */
		HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS           | ALTGR ,   /* @ */
		HID_KEYBOARD_SC_Q                                   | SHIFT ,   /* A */
		HID_KEYBOARD_SC_B                                   | SHIFT ,   /* B */
		HID_KEYBOARD_SC_C                                   | SHIFT ,   /* C */
		HID_KEYBOARD_SC_D                                   | SHIFT ,   /* D */
		HID_KEYBOARD_SC_E                                   | SHIFT ,   /* E */
		HID_KEYBOARD_SC_F                                   | SHIFT ,   /* F */
		HID_KEYBOARD_SC_G                                   | SHIFT ,   /* G */
		HID_KEYBOARD_SC_H                                   | SHIFT ,   /* H */
		HID_KEYBOARD_SC_I                                   | SHIFT ,   /* I */
		HID_KEYBOARD_SC_J                                   | SHIFT ,   /* J */
		HID_KEYBOARD_SC_K                                   | SHIFT ,   /* K */
		HID_KEYBOARD_SC_L                                   | SHIFT ,   /* L */
		HID_KEYBOARD_SC_SEMICOLON_AND_COLON                 | SHIFT ,   /* M */
		HID_KEYBOARD_SC_N                                   | SHIFT ,   /* N */
		HID_KEYBOARD_SC_O                                   | SHIFT ,   /* O */
		HID_KEYBOARD_SC_P                                   | SHIFT ,   /* P */
		HID_KEYBOARD_SC_A                                   | SHIFT ,   /* Q */
		HID_KEYBOARD_SC_R                                   | SHIFT ,   /* R */
		HID_KEYBOARD_SC_S                                   | SHIFT ,   /* S */
		HID_KEYBOARD_SC_T                                   | SHIFT ,   /* T */
		HID_KEYBOARD_SC_U                                   | SHIFT ,   /* U */
		HID_KEYBOARD_SC_V                                   | SHIFT ,   /* V */
		HID_KEYBOARD_SC_Z                                   | SHIFT ,   /* W */
		HID_KEYBOARD_SC_X                                   | SHIFT ,   /* X */
		HID_KEYBOARD_SC_Y                                   | SHIFT ,   /* Y */
		HID_KEYBOARD_SC_W                                   | SHIFT ,   /* Z */
		HID_KEYBOARD_SC_5_AND_PERCENTAGE                    | ALTGR ,   /* [ */
		HID_KEYBOARD_SC_8_AND_ASTERISK                      | ALTGR ,   /* \ */
		HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE                | ALTGR ,   /* ] */
		HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE           ,   /* ^ */
		HID_KEYBOARD_SC_8_AND_ASTERISK                              ,   /* _ */
		HID_KEYBOARD_SC_7_AND_AMPERSAND                     | ALTGR ,   /* ` */
		HID_KEYBOARD_SC_Q                                           ,   /* a */
		HID_KEYBOARD_SC_B                                           ,   /* b */
		HID_KEYBOARD_SC_C                                           ,   /* c */
		HID_KEYBOARD_SC_D                                           ,   /* d */
		HID_KEYBOARD_SC_E                                           ,   /* e */
		HID_KEYBOARD_SC_F                                           ,   /* f */
		HID_KEYBOARD_SC_G                                           ,   /* g */
		HID_KEYBOARD_SC_H                                           ,   /* h */
		HID_KEYBOARD_SC_I                                           ,   /* i */
		HID_KEYBOARD_SC_J                                           ,   /* j */
		HID_KEYBOARD_SC_K                                           ,   /* k */
		HID_KEYBOARD_SC_L                                           ,   /* l */
		HID_KEYBOARD_SC_SEMICOLON_AND_COLON                         ,   /* m */
		HID_KEYBOARD_SC_N                                           ,   /* n */
		HID_KEYBOARD_SC_O                                           ,   /* o */
		HID_KEYBOARD_SC_P                                           ,   /* p */
		HID_KEYBOARD_SC_A                                           ,   /* q */
		HID_KEYBOARD_SC_R                                           ,   /* r */
		HID_KEYBOARD_SC_S                                           ,   /* s */
		HID_KEYBOARD_SC_T                                           ,   /* t */
		HID_KEYBOARD_SC_U                                           ,   /* u */
		HID_KEYBOARD_SC_V                                           ,   /* v */
		HID_KEYBOARD_SC_Z                                           ,   /* w */
		HID_KEYBOARD_SC_X                                           ,   /* x */
		HID_KEYBOARD_SC_Y                                           ,   /* y */
		HID_KEYBOARD_SC_W                                           ,   /* z */
		HID_KEYBOARD_SC_4_AND_DOLLAR                        | ALTGR ,   /* { */
		HID_KEYBOARD_SC_6_AND_CARET                         | ALTGR ,   /* | */
		HID_KEYBOARD_SC_EQUAL_AND_PLUS                      | ALTGR ,   /* } */
		HID_KEYBOARD_SC_2_AND_AT                            | ALTGR ,   /* ~ */
		0

	},

	#undef SHIFT
	#undef ALTGR


};