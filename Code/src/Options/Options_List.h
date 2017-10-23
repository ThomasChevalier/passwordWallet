#ifndef OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
#define OPTIONS_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>

#include "../Graphics/String.h"

#define OPTIONS_LIST_NUM_OPTIONS (29)
#define OPTIONS_LIST_NONE OPTIONS_LIST_NUM_OPTIONS

#ifdef NONE
#define __NONE NONE
#undef NONE
#endif
#define NONE OPTIONS_LIST_NONE

#ifndef NULL
#define NULL ((void*)0)
#endif

#define OPTIONS_LIST_MAX_INTRICATION (2)

#include "Options_Callback_Password.h"
#include "Options_Callback_Sort.h"
#include "Options_Callback_Advanced.h"

typedef struct _Option_Node
{
	void (*callback)(void);  //< Function to call when a final node is selected, NULL if their is children, not NULL otherwise
	uint8_t child;
	uint8_t neighbour;
	uint16_t str;
} Option_Node;

#define OPTIONS_LIST_NORMAL (0)
#define OPTIONS_LIST_RECOVERY (22)
#define OPTIONS_LIST_SEND (26)

#define STR(str) str##_index

// #define DEFINE_NODE(name, callback, child, neighbour, strIndex) extern const Option_Node name PROGMEM
// #define DECLARE_NODE(name, callback, child, neighbour, strIndex) const Option_Node name PROGMEM = {callback, child, neighbour, strIndex}

// DEFINE_NODE(n_pwd, NULL, 1, 6, STR(str_option_pwd));
// 	DEFINE_NODE(n_change_pwd,  opt_callback_change_pwd,      NONE,    2, STR(str_option_change_pwd));
// 	DEFINE_NODE(n_change_usr,  opt_callback_change_usr_name, NONE,    3, STR(str_option_change_usr));
// 	DEFINE_NODE(n_change_name, opt_callback_change_name,     NONE,    4, STR(str_option_change_name));
// 	DEFINE_NODE(n_pwd_regen,   opt_callback_regenerate_pwd,  NONE,    5, STR(str_option_regen));
// 	DEFINE_NODE(n_pwd_del,     opt_callback_delete_pwd,      NONE, NONE, STR(str_option_delete));

// DEFINE_NODE(n_sort, NULL, 7, 10, STR(str_option_sort));
// 	DEFINE_NODE(n_sort_no,    opt_callback_no_sort,         NONE,    8, STR(str_option_sort_no));
// 	DEFINE_NODE(n_sort_use,   opt_callback_sort_use,        NONE,    9, STR(str_option_sort_use));
// 	DEFINE_NODE(n_sort_alpha, opt_callback_sort_alpha,      NONE, NONE, STR(str_option_sort_alpha));

// DEFINE_NODE(n_param,NULL, 11, 21, STR(str_option_param));

// 	DEFINE_NODE(n_opt_key,NULL, 12, 16, STR(str_option_key));
// 		DEFINE_NODE(n_change_key,  opt_callback_change_key,      NONE,   13, STR(str_option_change_key));
// 		DEFINE_NODE(n_show_key,    opt_callback_show_key,        NONE,   14, STR(str_option_show_key));
// 		DEFINE_NODE(n_force_card,  opt_callback_force_card,      NONE,   15, STR(str_option_force_card));
// 		DEFINE_NODE(n_force_enter, opt_callback_force_enter,     NONE,   NONE, STR(str_option_force_enter));

// 	DEFINE_NODE(n_key_layout,  opt_callback_change_keyboard, NONE,   17, STR(str_option_keyboard_layout));
// 	DEFINE_NODE(n_orientation, opt_callback_inverse_screen,  NONE,   18, STR(str_option_orientation));

// 	DEFINE_NODE(n_self_test,  opt_callback_self_test,       NONE,   19, STR(str_option_self_test));
// 	DEFINE_NODE(n_sys_info,   opt_callback_system_info,     NONE,   20, STR(str_system_info));
// 	DEFINE_NODE(n_full_reset, opt_callback_full_reset,      NONE, NONE, STR(str_option_full_reset));

// DEFINE_NODE(n_pwd_add, opt_callback_add_pwd, NONE, NONE, STR(str_option_add));

// DEFINE_NODE(n_recovery,  NULL, NONE, 23, STR(str_recovery_title));
// DEFINE_NODE(n_rec_full,  opt_callback_full_reset, NONE, 24, STR(str_recovery_full));
// DEFINE_NODE(n_rec_key,   opt_callback_enter_key, NONE, 25, STR(str_recovery_key));
// DEFINE_NODE(n_rec_card,  opt_callback_force_card, NONE, NONE, STR(str_option_force_card));

// DEFINE_NODE(n_send_both, opt_callback_send_both, NONE,   27, STR(str_option_send_both));
// DEFINE_NODE(n_send_pwd,  opt_callback_send_pwd,  NONE,   28,  STR(str_option_send_pwd));
// DEFINE_NODE(n_send_usr,  opt_callback_send_usr,  NONE, NONE,  STR(str_option_send_usr));

// extern const PGM_P const list_options [OPTIONS_LIST_NUM_OPTIONS] PROGMEM;

static const Option_Node list_options [OPTIONS_LIST_NUM_OPTIONS]  = 
{
	/*
	{callback, child, neighbour, strIndex}
	*/

/* 0*/  {NULL, 1, 6, STR(str_option_pwd)},
/* 1*/      {opt_callback_change_pwd,      NONE,    2, STR(str_option_change_pwd)},
/* 2*/      {opt_callback_change_usr_name, NONE,    3, STR(str_option_change_usr)},
/* 3*/      {opt_callback_change_name,     NONE,    4, STR(str_option_change_name)},
/* 4*/      {opt_callback_regenerate_pwd,  NONE,    5, STR(str_option_regen)},
/* 5*/      {opt_callback_delete_pwd,      NONE, NONE, STR(str_option_delete)},

/* 6*/  {NULL, 7, 10, STR(str_option_sort)},
/* 7*/      {opt_callback_no_sort,         NONE,    8, STR(str_option_sort_no)},
/* 8*/      {opt_callback_sort_use,        NONE,    9, STR(str_option_sort_use)},
/* 9*/      {opt_callback_sort_alpha,      NONE, NONE, STR(str_option_sort_alpha)},

/*10*/  {NULL, 11, 21, STR(str_option_param)},

/*11*/		{NULL, 12, 16, STR(str_option_key)},
/*12*/			{opt_callback_change_key,      NONE,   13, STR(str_option_change_key)},
/*13*/			{opt_callback_show_key,        NONE,   14, STR(str_option_show_key)},
/*14*/			{opt_callback_force_card,      NONE,   15, STR(str_option_force_card)},
/*15*/			{opt_callback_force_enter,     NONE,   NONE, STR(str_option_force_enter)},

/*16*/		{opt_callback_change_keyboard, NONE,   17, STR(str_option_keyboard_layout)},
/*17*/		{opt_callback_inverse_screen,  NONE,   18, STR(str_option_orientation)},

/*18*/		{opt_callback_self_test,       NONE,   19, STR(str_option_self_test)},
/*19*/		{opt_callback_system_info,     NONE,   20, STR(str_system_info)},
/*20*/		{opt_callback_full_reset,      NONE, NONE, STR(str_option_full_reset)},

/*21*/  {opt_callback_add_pwd, NONE, NONE, STR(str_option_add)},

/*22*/	{NULL, NONE, 23, STR(str_recovery_title)},
/*23*/	{opt_callback_full_reset, NONE, 24, STR(str_recovery_full)},
/*24*/	{opt_callback_enter_key, NONE, 25, STR(str_recovery_key)},
/*25*/	{opt_callback_force_card, NONE, NONE, STR(str_option_force_card)},

/*26*/	{opt_callback_send_both, NONE,   27, STR(str_option_send_both)},
/*27*/	{opt_callback_send_pwd,  NONE,   28,  STR(str_option_send_pwd)},
/*28*/	{opt_callback_send_usr,  NONE, NONE,  STR(str_option_send_usr)},
};


#ifdef __NONE
#undef NONE
#define NONE __NONE
#undef __NONE
#endif

#endif // OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
