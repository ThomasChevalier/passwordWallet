/*

Copyright 2017 Thomas Chevalier
All rights reserved

*/

#include "Ascii85.h"

#include "../Globals.h"

// A trick that may serve
// 85 = 0b0101'0101
// 85 = (2^6)+(2^4)+(2^2)+(2^1)
// x * 85 = x<<6 + x << 4 + x << 2 + x << 1
#define mul_85(x) ((x<<6) + (x << 4) + (x << 2) + (x << 0))

static const __flash uint32_t powers[5] = {52200625, 614125, 7225, 85, 1};

void encode_16B(uint8_t *input, char *output)
{
	uint8_t i = 0;
	for(; i < 4; ++i)
	{
		uint32_t sum = ((uint32_t)input[0] << 24) + ((uint32_t)input[1] << 16) + ((uint32_t)input[2] << 8) + (uint32_t)input[3];

		uint8_t j = 0;
		for(; j < 5; ++j)
		{
			char tempOutput = sum / powers[j];
			sum -= tempOutput * powers[j];
			tempOutput += 33;
			output[j] = tempOutput;
		}
		input += 4;
		output+= 5;
	}
}

void decode_16B(char *input, uint8_t *output)
{
	uint8_t i = 0;
	for(; i < 4; ++i)
	{
		uint32_t sum = 0;
		uint8_t j = 0;
		for(; j < 5; ++j)
		{
			sum += (input[j]-33) * powers[j];
		}
		output[0] = sum >> 24;
		output[1] = (sum & 0xFF0000) >> 16;
		output[2] = (sum & 0xFF00) >> 8;
		output[3] = sum & 0xFF;

		input += 5;
		output+= 4;
	}
}
