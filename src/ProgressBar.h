#ifndef PROGRESSBAR_HEADER_THOMAS_CHEVALIER
#define PROGRESSBAR_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

void progress_begin(uint8_t complexity);
void progress_add(uint8_t work);
void progress_end();

#endif // PROGRESSBAR_HEADER_THOMAS_CHEVALIER