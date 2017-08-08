#ifndef OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
#define OPTIONS_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>

#define OPTIONS_LIST_NUM_OPTIONS (18)
#define OPTIONS_LIST_NONE OPTIONS_LIST_NUM_OPTIONS

#ifdef NONE
#define __NONE NONE
#undef NONE
#endif
#define NONE OPTIONS_LIST_NONE

#ifndef NULL
#define NULL ((void*)0)
#endif

#define OPTIONS_LIST_MAX_INTRICATION (1)

#include "Options_Callback_Password.h"
#include "Options_Callback_Sort.h"
#include "Options_Callback_Advanced.h"

typedef struct _Option_Node
{
	void (*callback)(void);  //< Function to call when a final node is selected, NULL if their is children, not NULL otherwise
	uint8_t child;
	uint8_t neighbour;
	uint8_t str;
} Option_Node;

#define OPTIONS_LIST_NORMAL (0)

// #define DEFINE_NODE(name, callback, child, neighbour, strIndex) extern const __flash Option_Node name
// #define DECLARE_NODE(name, callback, child, neighbour, strIndex) const __flash Option_Node name = {callback, child, neighbour, strIndex}

// DEFINE_NODE(n_pwd, NULL, 1, 6, 33);
// 	DEFINE_NODE(n_pwd_change_pwd, opt_callback_change_pwd, NONE, 2, 37);
// 	DEFINE_NODE(n_pwd_change_usr, opt_callback_change_usr_name, NONE,    3, 38);
// 	DEFINE_NODE(n_pwd_change_nm , opt_callback_change_name,     NONE,    4, 39);
// 	DEFINE_NODE(n_pwd_regen_pwd , opt_callback_regenerate_pwd,  NONE,    5, 40);
// 	DEFINE_NODE(n_pwd_delete_pwd, opt_callback_delete_pwd,      NONE, NONE, 41);

// DEFINE_NODE(n_sort, NULL, 7, 10, 34);
// 	DEFINE_NODE(n_sort_no   , opt_callback_no_sort,         NONE,    8, 42);
// 	DEFINE_NODE(n_sort_use  , opt_callback_sort_usage,      NONE,    9, 43);
// 	DEFINE_NODE(n_sort_alpha, opt_callback_sort_alpha,      NONE, NONE, 44);

// DEFINE_NODE(n_adv, NULL, 11, 17, 35);
// 	DEFINE_NODE(n_adv_ch_key  , opt_callback_change_key,      NONE,   12, 45);
// 	DEFINE_NODE(n_adv_sh_key  , opt_callback_show_key,        NONE,   13, 46);
// 	DEFINE_NODE(n_adv_inv_sc  , opt_callback_inverse_screen,  NONE,   14, 47);
// 	DEFINE_NODE(n_adv_keyboard, opt_callback_change_keyboard, NONE,   15, 48);
// 	DEFINE_NODE(n_adv_self    , opt_callback_self_test,       NONE,   16, 49);
// 	DEFINE_NODE(n_adv_reset   , opt_callback_full_reset,      NONE, NONE, 50);

// DEFINE_NODE(n_add, opt_callback_add_pwd, NONE, NONE, 36);

// extern const Option_Node* const  __flash list_options [];

static const Option_Node list_options []  = 
{
	/*
	{callback, child, neighbour, strIndex}
	*/

/* 0*/  {NULL, 1, 6, 33},
/* 1*/      {opt_callback_change_pwd,      NONE,    2, 37},
/* 2*/      {opt_callback_change_usr_name, NONE,    3, 38},
/* 3*/      {opt_callback_change_name,     NONE,    4, 39},
/* 4*/      {opt_callback_regenerate_pwd,  NONE,    5, 40},
/* 5*/      {opt_callback_delete_pwd,      NONE, NONE, 41},
/* 6*/  {NULL, 7, 10, 34},
/* 7*/      {opt_callback_no_sort,         NONE,    8, 42},
/* 8*/      {opt_callback_sort_usage,      NONE,    9, 43},
/* 9*/      {opt_callback_sort_alpha,      NONE, NONE, 44},
/*10*/  {NULL, 11, 17, 35},
/*11*/		{opt_callback_change_key,      NONE,   12, 45},
/*12*/		{opt_callback_show_key,        NONE,   13, 46},
/*13*/		{opt_callback_inverse_screen,  NONE,   14, 47},
/*14*/		{opt_callback_change_keyboard, NONE,   15, 48},
/*15*/		{opt_callback_self_test,       NONE,   16, 49},
/*16*/		{opt_callback_full_reset,      NONE, NONE, 50},
/*17*/  {opt_callback_add_pwd, NONE, NONE, 36},
};


#ifdef __NONE
#undef NONE
#define NONE __NONE
#undef __NONE
#endif

#endif // OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
