/*
 * File auto-generated by option_parser.
 * Copyright - Thomas Chevalier - 2018.
 * thomasom.chevalier@gmail.com
 * File created on 2018-06-03 20:48:09.854977
*/

#ifndef OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
#define OPTIONS_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>

#include "../Graphics/String.h"

#include "Options_Callback_Password.h"
#include "Options_Callback_Sort.h"
#include "Options_Callback_Advanced.h"

#define OPTIONS_LIST_NUM_OPTIONS (28)
#define OPTIONS_LIST_NONE OPTIONS_LIST_NUM_OPTIONS

#ifndef NULL
#define NULL ((void*)0)
#endif

#define OPTIONS_LIST_MAX_INTRICATION (2)

#define STR(str) str##_index

typedef struct _Option_Node
{
	void (*callback)(void);  //< Function to call when a final node is selected, NULL if their is children, not NULL otherwise
	uint8_t child;
	uint8_t neighbour;
	uint16_t str;
} Option_Node;

#define DEFINE_NODE(name, callback, child, neighbour, strIndex) extern const __flash Option_Node name
#define DECLARE_NODE(name, callback, child, neighbour, strIndex) const __flash Option_Node name = {callback, child, neighbour, strIndex}

// Aliases
#define OPTIONS_LIST_NORMAL (0)
#define OPTIONS_LIST_RECOVERY (22)
#define OPTIONS_LIST_SEND (25)

// Data
/* 00 */    DEFINE_NODE(__auto0, NULL,1, 6, STR(str_option_pwd));
/* 01 */    DEFINE_NODE(__auto1, opt_callback_change_pwd,OPTIONS_LIST_NONE, 2, STR(str_option_change_pwd));
/* 02 */    DEFINE_NODE(__auto2, opt_callback_change_usr_name,OPTIONS_LIST_NONE, 3, STR(str_option_change_usr));
/* 03 */    DEFINE_NODE(__auto3, opt_callback_change_name,OPTIONS_LIST_NONE, 4, STR(str_option_change_name));
/* 04 */    DEFINE_NODE(__auto4, opt_callback_regenerate_pwd,OPTIONS_LIST_NONE, 5, STR(str_option_regen));
/* 05 */    DEFINE_NODE(__auto5, opt_callback_delete_pwd,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_delete));
/* 06 */    DEFINE_NODE(__auto6, NULL,7, 10, STR(str_option_sort));
/* 07 */    DEFINE_NODE(__auto7, opt_callback_no_sort,OPTIONS_LIST_NONE, 8, STR(str_option_sort_no));
/* 08 */    DEFINE_NODE(__auto8, opt_callback_sort_use,OPTIONS_LIST_NONE, 9, STR(str_option_sort_use));
/* 09 */    DEFINE_NODE(__auto9, opt_callback_sort_alpha,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_sort_alpha));
/* 10 */    DEFINE_NODE(__auto10, NULL,11, 21, STR(str_option_param));
/* 11 */    DEFINE_NODE(__auto11, NULL,12, 16, STR(str_option_key));
/* 12 */    DEFINE_NODE(__auto12, opt_callback_change_key,OPTIONS_LIST_NONE, 13, STR(str_option_change_key));
/* 13 */    DEFINE_NODE(__auto13, opt_callback_show_key,OPTIONS_LIST_NONE, 14, STR(str_option_show_key));
/* 14 */    DEFINE_NODE(__auto14, opt_callback_force_card,OPTIONS_LIST_NONE, 15, STR(str_option_force_card));
/* 15 */    DEFINE_NODE(__auto15, opt_callback_force_enter,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_force_enter));
/* 16 */    DEFINE_NODE(__auto16, opt_callback_change_keyboard,OPTIONS_LIST_NONE, 17, STR(str_option_keyboard_layout));
/* 17 */    DEFINE_NODE(__auto17, opt_callback_inverse_screen,OPTIONS_LIST_NONE, 18, STR(str_option_orientation));
/* 18 */    DEFINE_NODE(__auto18, opt_callback_self_test,OPTIONS_LIST_NONE, 19, STR(str_option_self_test));
/* 19 */    DEFINE_NODE(__auto19, opt_callback_system_info,OPTIONS_LIST_NONE, 20, STR(str_system_info));
/* 20 */    DEFINE_NODE(__auto20, opt_callback_full_reset,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_full_reset));
/* 21 */    DEFINE_NODE(__auto21, opt_callback_add_pwd,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_add));
/* 22 */    DEFINE_NODE(__auto22, NULL,OPTIONS_LIST_NONE, 23, STR(str_recovery_title));
/* 23 */    DEFINE_NODE(__auto23, opt_callback_full_reset,OPTIONS_LIST_NONE, 24, STR(str_recovery_full));
/* 24 */    DEFINE_NODE(__auto24, opt_callback_enter_key,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_recovery_key));
/* 25 */    DEFINE_NODE(__auto25, opt_callback_send_both,OPTIONS_LIST_NONE, 26, STR(str_option_send_both));
/* 26 */    DEFINE_NODE(__auto26, opt_callback_send_pwd,OPTIONS_LIST_NONE, 27, STR(str_option_send_pwd));
/* 27 */    DEFINE_NODE(__auto27, opt_callback_send_usr,OPTIONS_LIST_NONE, OPTIONS_LIST_NONE, STR(str_option_send_usr));

// Array
extern const __flash Option_Node* const __flash list_options [OPTIONS_LIST_NUM_OPTIONS];

#endif // OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
