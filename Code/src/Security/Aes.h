#ifndef AES_HEADER_THOMAS_CHEVALIER
#define AES_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// From http://dominik.cc/projekty/avr-aes/

/*
*	Encryption and decryption library for 8-bit Atmel AVR
*	Copyright (C) 2011-2012  D. Kilian
*	Copyright (C) 2003,2006  B. Poettering
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	The first implementation (SMALL) was written by D. Kilian. The other
*	three implementations (FANTASTIC, FURIOUS, FAST) are translated and
*	slightly modified assembler files that wrote B. Poettering.
*	Library is published under the terms of the GNU General Public
*	License. If you need AES code, but this license is unsuitable for
*	your project, feel free to contact authors.
*	D. Kilian: kontakt AT dominik.cc
*	B. Poettering: avraes AT point-at-infinity.org
*/


/*
*	Add aesCipher to the library.
*/
#define AES_CIPHER 1

/*
*	Add aesInvCipher to the library.
*/
#define AES_INVCIPHER 1

/*
*	Add aesKeyRewind to the library, if available.
*/
#define AES_KEYREWIND 1

/*
*	Add aesKeyPatch to the library, if available.
*/
#define AES_KEYPATCH 1

/*
*	Add aesKeyExpand to the library, if available.
*/
#define AES_KEYEXPAND 0

/*
*	Use short array instead of SBOX.
*	Only significant in SMALL implementation.
*/
#define AES_SHORTSBOX 1

/*
*	Use short array instead of inverse of SBOX.
*	Only significant in SMALL implementation and when AES_INVCIPHER != 0.
*/
#define AES_SHORTINVSBOX 1

/*
*	Use faster function for ShiftRows and InvShiftRows transformations.
*	Only significant in SMALL implementation.
*/
#define AES_FASTSHROWS 0

/*
*	Use faster function for MixColumns and InvMixColumns transformations.
*	Only significant in SMALL implementation and when AES_CIPHER != 0 && AES_INVCIPHER != 0.
*/
#define AES_FASTMIXCOL 0

/*
*	Size of short array. Correct values: 10, 12, 16, 20, ..., 4*N, ..., 248, 252, 256
*	Only significant in SMALL implementation and when
*	AES_SHORTSBOX != 0 || AES_SHORTINVSBOX != 0.
*/
#define AES_SHORTSBOXSIZE 16

/*
*	Allow inline assembler.
*	Only significant in SMALL implementation.
*/
#define AES_USEASM 1

void AES128_CBC_encrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv);
void AES128_CBC_decrypt_buffer(uint8_t* input, uint8_t length, uint8_t* key, uint8_t* iv);

#endif // AES_HEADER_THOMAS_CHEVALIER
