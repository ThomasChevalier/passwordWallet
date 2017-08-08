#include "Options_Callback_Sort.h"

#include "../Globals.h"

void opt_callback_no_sort(void)
{
	update_opt_flags(OPTIONS_FLAG & 0xF9);
}

void opt_callback_sort_usage(void)
{
	update_opt_flags((OPTIONS_FLAG&0xF9)|0x02);
}
void opt_callback_sort_alpha(void)
{
	update_opt_flags((OPTIONS_FLAG&0xF9)|0x04);
}
