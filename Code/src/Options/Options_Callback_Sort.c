#include "Options_Callback_Sort.h"

#include "../Globals.h"

void opt_callback_no_sort(void)
{
	update_opt_flags((OPTIONS_FLAG & ~( (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L) | (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H)) ) | PWD_SORTING_NONE);
}

void opt_callback_sort_use(void)
{
	update_opt_flags((OPTIONS_FLAG & ~( (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L) | (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H)) ) | PWD_SORTING_USE);
}

void opt_callback_sort_alpha(void)
{
	update_opt_flags((OPTIONS_FLAG & ~( (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L) | (1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H)) ) | PWD_SORTING_ALPHA);
}
