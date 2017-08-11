#ifndef OPTIONS_CALLBACK_SORT_HEADER_THOMAS_CHEVALIER
#define OPTIONS_CALLBACK_SORT_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Function called when the user select the option "no sort".
 * @details Updates the OPTIONS_FLAG
 */
void opt_callback_no_sort(void);

/**
 * @brief Function called when the user select the option "sort by use".
 * @details Updates the OPTIONS_FLAG
 */
void opt_callback_sort_use(void);

/**
 * @brief Function called when the user select the option "sort by name".
 * @details Updates the OPTIONS_FLAG
 */	
void opt_callback_sort_alpha(void);

#endif // OPTIONS_CALLBACK_SORT_HEADER_THOMAS_CHEVALIER
