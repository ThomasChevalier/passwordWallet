#define DEFINE_VARIABLES

#include "String.h"


char str_buffer[26];
/*
const char* const str_table[] PROGMEM =
{
    str(ERR_CARD_STR),
    str(ERR_AUTH_STR),
    str(ERR_READ_STR),
    str(ERR_PWD_STR)
};
*/

void str_to_buffer(const char* str_id)
{
    strcpy_P(str_buffer, (PGM_P)pgm_read_word(&(str_id))); // See http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
}
