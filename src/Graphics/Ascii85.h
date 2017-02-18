/*

Copyright 2016 Thomas Chevalier
All rights reserved

*/

#ifndef ASCII85_HEADER_THOMAS_CHEVALIER
#define ASCII85_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// Input should be 16 bytes long and output 20 bytes long
void encode_16B(uint8_t *input, char *output);


// Input should be 20 bytes long and output 16 bytes long
void decode_16B(char *input, uint8_t *output);

#endif // ASCII85_HEADER_THOMAS_CHEVALIER
