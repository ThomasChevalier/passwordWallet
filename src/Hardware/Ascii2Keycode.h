#ifndef ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER
#define ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>

// MDB is set to 1 if modifier is needed and to 0 otherwhise
static const uint8_t ascii_to_keycode_map[] PROGMEM =
{
/*
* |                       KEYCODE / MODIFIER                       |   ASCII   |
*/

	HID_KEYBOARD_SC_ESCAPE, 										/* space */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION | (1<<7), 					/* ! */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE | (1<<7), 					/* " */
	HID_KEYBOARD_SC_3_AND_HASHMARK | (1<<7), 						/* # */
	HID_KEYBOARD_SC_4_AND_DOLLAR | (1<<7), 							/* $ */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE | (1<<7), 						/* % */
	HID_KEYBOARD_SC_7_AND_AMPERSAND | (1<<7), 						/* & */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE, 							/* ' */
	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS | (1<<7), 			/* ( */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS | (1<<7), 			/* ) */
	HID_KEYBOARD_SC_8_AND_ASTERISK | (1<<7), 						/* * */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS | (1<<7), 						/* + */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN, 						/* , */
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE, 							/* - */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN, 						/* . */
	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK, 						/* / */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS, 						/* 0 */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION, 								/* 1 */
	HID_KEYBOARD_SC_2_AND_AT, 										/* 2 */
	HID_KEYBOARD_SC_3_AND_HASHMARK, 								/* 3 */
	HID_KEYBOARD_SC_4_AND_DOLLAR, 									/* 4 */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE, 								/* 5 */
	HID_KEYBOARD_SC_6_AND_CARET, 									/* 6 */
	HID_KEYBOARD_SC_7_AND_AMPERSAND,								/* 7 */
	HID_KEYBOARD_SC_8_AND_ASTERISK, 								/* 8 */
	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS, 						/* 9 */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON | (1<<7), 					/* : */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON, 							/* ; */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN | (1<<7), 				/* < */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS, 								/* = */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN | (1<<7), 			/* > */
	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK | (1<<7),				/* ? */
	HID_KEYBOARD_SC_2_AND_AT | (1<<7), 								/* @ */
	HID_KEYBOARD_SC_A | (1<<7),										/* A */
	HID_KEYBOARD_SC_B | (1<<7),										/* B */
	HID_KEYBOARD_SC_C | (1<<7),										/* C */
	HID_KEYBOARD_SC_D | (1<<7),										/* D */
	HID_KEYBOARD_SC_E | (1<<7),										/* E */
	HID_KEYBOARD_SC_F | (1<<7),										/* F */
	HID_KEYBOARD_SC_G | (1<<7),										/* G */
	HID_KEYBOARD_SC_H | (1<<7),										/* H */
	HID_KEYBOARD_SC_I | (1<<7),										/* I */
	HID_KEYBOARD_SC_J | (1<<7),										/* J */
	HID_KEYBOARD_SC_K | (1<<7),										/* K */
	HID_KEYBOARD_SC_L | (1<<7),										/* L */
	HID_KEYBOARD_SC_M | (1<<7),										/* M */
	HID_KEYBOARD_SC_N | (1<<7),										/* N */
	HID_KEYBOARD_SC_O | (1<<7),										/* O */
	HID_KEYBOARD_SC_P | (1<<7),										/* P */
	HID_KEYBOARD_SC_Q | (1<<7),										/* Q */
	HID_KEYBOARD_SC_R | (1<<7),										/* R */
	HID_KEYBOARD_SC_S | (1<<7),										/* S */
	HID_KEYBOARD_SC_T | (1<<7),										/* T */
	HID_KEYBOARD_SC_U | (1<<7),										/* U */
	HID_KEYBOARD_SC_V | (1<<7),										/* V */
	HID_KEYBOARD_SC_W | (1<<7),										/* W */
	HID_KEYBOARD_SC_X | (1<<7),										/* X */
	HID_KEYBOARD_SC_Y | (1<<7),										/* Y */
	HID_KEYBOARD_SC_Z | (1<<7),										/* Z */
	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE, 				/* [ */
	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE, 							/* \ */
	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE, 				/* ] */
	HID_KEYBOARD_SC_6_AND_CARET | (1<<7), 							/* ^ */
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE | (1<<7), 					/* _ */
	HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE, 						/* ` */
	HID_KEYBOARD_SC_A, 												/* a*/
	HID_KEYBOARD_SC_B, 												/* b*/
	HID_KEYBOARD_SC_C, 												/* c*/
	HID_KEYBOARD_SC_D, 												/* d*/
	HID_KEYBOARD_SC_E, 												/* e*/
	HID_KEYBOARD_SC_F, 												/* f*/
	HID_KEYBOARD_SC_G, 												/* g*/
	HID_KEYBOARD_SC_H, 												/* h*/
	HID_KEYBOARD_SC_I, 												/* i*/
	HID_KEYBOARD_SC_J, 												/* j*/
	HID_KEYBOARD_SC_K, 												/* k*/
	HID_KEYBOARD_SC_L, 												/* l*/
	HID_KEYBOARD_SC_M, 												/* m*/
	HID_KEYBOARD_SC_N, 												/* n*/
	HID_KEYBOARD_SC_O, 												/* o*/
	HID_KEYBOARD_SC_P, 												/* p*/
	HID_KEYBOARD_SC_Q, 												/* q*/
	HID_KEYBOARD_SC_R, 												/* r*/
	HID_KEYBOARD_SC_S, 												/* s*/
	HID_KEYBOARD_SC_T, 												/* t*/
	HID_KEYBOARD_SC_U, 												/* u*/
	HID_KEYBOARD_SC_V, 												/* v*/
	HID_KEYBOARD_SC_W, 												/* w*/
	HID_KEYBOARD_SC_X, 												/* x*/
	HID_KEYBOARD_SC_Y, 												/* y*/
	HID_KEYBOARD_SC_Z, 												/* z*/
	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE | (1<<7), 	/* { */
	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE | (1<<7), 					/* | */
	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE | (1<<7), 	/* } */
	HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE | (1<<7) 				/* ~ */

};

#endif // ASCII_TO_KEYCODE_HEADER_THOMAS_CHEVALIER