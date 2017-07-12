#include "Ascii2Keycode.h"

#include "../Globals.h"

#define SHIFT KEYCODE_MODIFIER_SHIFT
#define ALTGR KEYCODE_MODIFIER_ALTGR
// MDB is set to 1 if modifier is needed and to 0 otherwhise
// Modifier == (1<<7) means that the left shift is pressed
// Modifier == (1<<6) means that the right alt is pressed
static const uint8_t qwerty_to_keycode_map[] PROGMEM =
{
/*
*	|					KEYCODE 						| MODIF |	ASCII	|
*/

	HID_KEYBOARD_SC_SPACE										,	/* space */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION					| SHIFT	,	/* ! */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE				| SHIFT	,	/* " */
	HID_KEYBOARD_SC_3_AND_HASHMARK						| SHIFT	,	/* # */
	HID_KEYBOARD_SC_4_AND_DOLLAR						| SHIFT	,	/* $ */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE					| SHIFT	,	/* % */
	HID_KEYBOARD_SC_7_AND_AMPERSAND						| SHIFT	,	/* & */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE						,	/* ' */
	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS			| SHIFT	,	/* ( */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS			| SHIFT	,	/* ) */
	HID_KEYBOARD_SC_8_AND_ASTERISK						| SHIFT	,	/* * */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS						| SHIFT	,	/* + */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN					,	/* ,*/
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE						,	/* - */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN					,	/* . */
	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK						,	/* / */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS					,	/* 0 */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION							,	/* 1 */
	HID_KEYBOARD_SC_2_AND_AT									,	/* 2 */
	HID_KEYBOARD_SC_3_AND_HASHMARK								,	/* 3 */
	HID_KEYBOARD_SC_4_AND_DOLLAR								,	/* 4 */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE							,	/* 5 */
	HID_KEYBOARD_SC_6_AND_CARET									,	/* 6 */
	HID_KEYBOARD_SC_7_AND_AMPERSAND								,	/* 7 */
	HID_KEYBOARD_SC_8_AND_ASTERISK								,	/* 8 */
	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS					,	/* 9 */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON					| SHIFT	,	/* : */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON							,	/* ; */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN			| SHIFT	,	/* < */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS								,	/* = */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN			| SHIFT	,	/* > */
	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK				| SHIFT	,	/* ? */
	HID_KEYBOARD_SC_2_AND_AT							| SHIFT	,	/* @ */
	HID_KEYBOARD_SC_A									| SHIFT	,	/* A */
	HID_KEYBOARD_SC_B									| SHIFT	,	/* B */
	HID_KEYBOARD_SC_C									| SHIFT	,	/* C */
	HID_KEYBOARD_SC_D									| SHIFT	,	/* D */
	HID_KEYBOARD_SC_E									| SHIFT	,	/* E */
	HID_KEYBOARD_SC_F									| SHIFT	,	/* F */
	HID_KEYBOARD_SC_G									| SHIFT	,	/* G */
	HID_KEYBOARD_SC_H									| SHIFT	,	/* H */
	HID_KEYBOARD_SC_I									| SHIFT	,	/* I */
	HID_KEYBOARD_SC_J									| SHIFT	,	/* J */
	HID_KEYBOARD_SC_K									| SHIFT	,	/* K */
	HID_KEYBOARD_SC_L									| SHIFT	,	/* L */
	HID_KEYBOARD_SC_M									| SHIFT	,	/* M */
	HID_KEYBOARD_SC_N									| SHIFT	,	/* N */
	HID_KEYBOARD_SC_O									| SHIFT	,	/* O */
	HID_KEYBOARD_SC_P									| SHIFT	,	/* P */
	HID_KEYBOARD_SC_Q									| SHIFT	,	/* Q */
	HID_KEYBOARD_SC_R									| SHIFT	,	/* R */
	HID_KEYBOARD_SC_S									| SHIFT	,	/* S */
	HID_KEYBOARD_SC_T									| SHIFT	,	/* T */
	HID_KEYBOARD_SC_U									| SHIFT	,	/* U */
	HID_KEYBOARD_SC_V									| SHIFT	,	/* V */
	HID_KEYBOARD_SC_W									| SHIFT	,	/* W */
	HID_KEYBOARD_SC_X									| SHIFT	,	/* X */
	HID_KEYBOARD_SC_Y									| SHIFT	,	/* Y */
	HID_KEYBOARD_SC_Z									| SHIFT	,	/* Z */
	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE			,	/* [ */
	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE							,	/* \ */
	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE			,	/* ] */
	HID_KEYBOARD_SC_6_AND_CARET							| SHIFT	,	/* ^ */
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE				| SHIFT	,	/* _ */
	HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE					,	/* ` */
	HID_KEYBOARD_SC_A											,	/* a */
	HID_KEYBOARD_SC_B											,	/* b */
	HID_KEYBOARD_SC_C											,	/* c */
	HID_KEYBOARD_SC_D											,	/* d */
	HID_KEYBOARD_SC_E											,	/* e */
	HID_KEYBOARD_SC_F											,	/* f */
	HID_KEYBOARD_SC_G											,	/* g */
	HID_KEYBOARD_SC_H											,	/* h */
	HID_KEYBOARD_SC_I											,	/* i */
	HID_KEYBOARD_SC_J											,	/* j */
	HID_KEYBOARD_SC_K											,	/* k */
	HID_KEYBOARD_SC_L											,	/* l */
	HID_KEYBOARD_SC_M											,	/* m */
	HID_KEYBOARD_SC_N											,	/* n */
	HID_KEYBOARD_SC_O											,	/* o */
	HID_KEYBOARD_SC_P											,	/* p */
	HID_KEYBOARD_SC_Q											,	/* q */
	HID_KEYBOARD_SC_R											,	/* r */
	HID_KEYBOARD_SC_S											,	/* s */
	HID_KEYBOARD_SC_T											,	/* t */
	HID_KEYBOARD_SC_U											,	/* u */
	HID_KEYBOARD_SC_V											,	/* v */
	HID_KEYBOARD_SC_W											,	/* w */
	HID_KEYBOARD_SC_X											,	/* x */
	HID_KEYBOARD_SC_Y											,	/* y */
	HID_KEYBOARD_SC_Z											,	/* z */
	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE	| SHIFT	,	/* { */
	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE					| SHIFT	,	/*	| */
	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE	| SHIFT	,	/* } */
	HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE			| SHIFT		/* ~ */

};

static const uint8_t azerty_to_keycode_map[] PROGMEM =
{
/*
*	|					KEYCODE 						| MODIF |	ASCII	|
*/

	HID_KEYBOARD_SC_SPACE										,	/* space */
	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK						,	/* ! */
	HID_KEYBOARD_SC_3_AND_HASHMARK								,	/* " */
	HID_KEYBOARD_SC_3_AND_HASHMARK						| ALTGR	,	/* # */
	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE			,	/* $ */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE						,	/* % */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION							,	/* & */
	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE						,	/* ' */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE							,	/* ( */
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE						,	/* ) */
	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE							,	/* * */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS						| SHIFT	,	/* + */
	HID_KEYBOARD_SC_M											,	/* , */
	HID_KEYBOARD_SC_6_AND_CARET									,	/* - */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN			| SHIFT	,	/* . */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN			| SHIFT	,	/* / */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS			| SHIFT	,	/* 0 */
	HID_KEYBOARD_SC_1_AND_EXCLAMATION					| SHIFT	,	/* 1 */
	HID_KEYBOARD_SC_2_AND_AT							| SHIFT	,	/* 2 */
	HID_KEYBOARD_SC_3_AND_HASHMARK						| SHIFT	,	/* 3 */
	HID_KEYBOARD_SC_4_AND_DOLLAR						| SHIFT	,	/* 4 */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE					| SHIFT	,	/* 5 */
	HID_KEYBOARD_SC_6_AND_CARET							| SHIFT	,	/* 6 */
	HID_KEYBOARD_SC_7_AND_AMPERSAND						| SHIFT	,	/* 7 */
	HID_KEYBOARD_SC_8_AND_ASTERISK						| SHIFT	,	/* 8 */
	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS			| SHIFT	,	/* 9 */
	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN					,	/* : */
	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN					,	/* ; */
	HID_KEYBOARD_SC_KEYPAD_LESS_THAN_SIGN						,	/* < */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS								,	/* = */
	HID_KEYBOARD_SC_KEYPAD_GREATER_THAN_SIGN					,	/* > */
	HID_KEYBOARD_SC_M									| SHIFT	,	/* ? */
	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS			| ALTGR	,	/* @ */
	HID_KEYBOARD_SC_Q									| SHIFT	,	/* A */
	HID_KEYBOARD_SC_B									| SHIFT	,	/* B */
	HID_KEYBOARD_SC_C									| SHIFT	,	/* C */
	HID_KEYBOARD_SC_D									| SHIFT	,	/* D */
	HID_KEYBOARD_SC_E									| SHIFT	,	/* E */
	HID_KEYBOARD_SC_F									| SHIFT	,	/* F */
	HID_KEYBOARD_SC_G									| SHIFT	,	/* G */
	HID_KEYBOARD_SC_H									| SHIFT	,	/* H */
	HID_KEYBOARD_SC_I									| SHIFT	,	/* I */
	HID_KEYBOARD_SC_J									| SHIFT	,	/* J */
	HID_KEYBOARD_SC_K									| SHIFT	,	/* K */
	HID_KEYBOARD_SC_L									| SHIFT	,	/* L */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON					| SHIFT	,	/* M */
	HID_KEYBOARD_SC_N									| SHIFT	,	/* N */
	HID_KEYBOARD_SC_O									| SHIFT	,	/* O */
	HID_KEYBOARD_SC_P									| SHIFT	,	/* P */
	HID_KEYBOARD_SC_A									| SHIFT	,	/* Q */
	HID_KEYBOARD_SC_R									| SHIFT	,	/* R */
	HID_KEYBOARD_SC_S									| SHIFT	,	/* S */
	HID_KEYBOARD_SC_T									| SHIFT	,	/* T */
	HID_KEYBOARD_SC_U									| SHIFT	,	/* U */
	HID_KEYBOARD_SC_V									| SHIFT	,	/* V */
	HID_KEYBOARD_SC_Z									| SHIFT	,	/* W */
	HID_KEYBOARD_SC_X									| SHIFT	,	/* X */
	HID_KEYBOARD_SC_Y									| SHIFT	,	/* Y */
	HID_KEYBOARD_SC_W									| SHIFT	,	/* Z */
	HID_KEYBOARD_SC_5_AND_PERCENTAGE					| ALTGR	,	/* [ */
	HID_KEYBOARD_SC_8_AND_ASTERISK						| ALTGR	,	/* \ */
	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE				| ALTGR	,	/* ] */
	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE			,	/* ^ */
	HID_KEYBOARD_SC_8_AND_ASTERISK								,	/* _ */
	HID_KEYBOARD_SC_7_AND_AMPERSAND						| ALTGR	,	/* ` */
	HID_KEYBOARD_SC_Q											,	/* a */
	HID_KEYBOARD_SC_B											,	/* b */
	HID_KEYBOARD_SC_C											,	/* c */
	HID_KEYBOARD_SC_D											,	/* d */
	HID_KEYBOARD_SC_E											,	/* e */
	HID_KEYBOARD_SC_F											,	/* f */
	HID_KEYBOARD_SC_G											,	/* g */
	HID_KEYBOARD_SC_H											,	/* h */
	HID_KEYBOARD_SC_I											,	/* i */
	HID_KEYBOARD_SC_J											,	/* j */
	HID_KEYBOARD_SC_K											,	/* k */
	HID_KEYBOARD_SC_L											,	/* l */
	HID_KEYBOARD_SC_SEMICOLON_AND_COLON							,	/* m */
	HID_KEYBOARD_SC_N											,	/* n */
	HID_KEYBOARD_SC_O											,	/* o */
	HID_KEYBOARD_SC_P											,	/* p */
	HID_KEYBOARD_SC_A											,	/* q */
	HID_KEYBOARD_SC_R											,	/* r */
	HID_KEYBOARD_SC_S											,	/* s */
	HID_KEYBOARD_SC_T											,	/* t */
	HID_KEYBOARD_SC_U											,	/* u */
	HID_KEYBOARD_SC_V											,	/* v */
	HID_KEYBOARD_SC_Z											,	/* w */
	HID_KEYBOARD_SC_X											,	/* x */
	HID_KEYBOARD_SC_Y											,	/* y */
	HID_KEYBOARD_SC_W											,	/* z */
	HID_KEYBOARD_SC_4_AND_DOLLAR						| ALTGR	,	/* { */
	HID_KEYBOARD_SC_6_AND_CARET							| ALTGR	,	/* | */
	HID_KEYBOARD_SC_EQUAL_AND_PLUS						| ALTGR	,	/* } */
	HID_KEYBOARD_SC_2_AND_AT							| ALTGR		/* ~ */

};

uint8_t ascii_to_keycode(char c)
{
	if(OPTIONS_FLAG & (1<<OPTIONS_FLAG_OFFSET_QWERTY))
	{
		return qwerty_to_keycode_map[c - ' '];
	}
	else
	{
		return azerty_to_keycode_map[c - ' '];
	}
}

#undef SHIFT
#undef ALTGR