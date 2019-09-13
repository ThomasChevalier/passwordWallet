/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include <stdint.h>
#include <avr/pgmspace.h>
#include "Aes.h"

#define SBOX_DATA_INIT \
	0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, \
	0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, \
	0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, \
	0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, \
	0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, \
	0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, \
	0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, \
	0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, \
	0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, \
	0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, \
	0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, \
	0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, \
	0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, \
	0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, \
	0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, \
	0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16,

#define ISBOX_DATA_INIT \
	0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb, \
	0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb, \
	0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e, \
	0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25, \
	0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92, \
	0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84, \
	0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06, \
	0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b, \
	0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73, \
	0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e, \
	0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b, \
	0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4, \
	0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f, \
	0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef, \
	0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61, \
	0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d,



/************************************************************************/
/*   SMALL                                                 by D. Kilian */
/************************************************************************/




typedef unsigned char byte;

static byte Rcon;

#ifdef AES_USEASM
	#define GF28_MUL2(a) { asm ("lsl %0 \n brcc .+2 \n eor %0, %1 \n" : "=r" (a) : "r" (c0x1b), "0" (a)); }
	#define GF28_DIV2(a) { asm ("lsr %0 \n brcc .+2 \n eor %0, %1 \n" : "=r" (a) : "r" (c0x8d), "0" (a)); }
#else
	#define GF28_MUL2(a) { if (a&0x80) { a = (a<<1) ^ 0x1b; } else { a <<= 1; } }
	#define GF28_DIV2(a) { if (a&0x01) { a = (a>>1) ^ 0x8d; } else { a >>= 1; } }
#endif


#if AES_SHORTSBOX
	#define SBOXTYPE 2  // funkcja
#else
	#define SBOXTYPE 1  // tablica
#endif


#if AES_INVCIPHER
	#if AES_SHORTINVSBOX
		#define INVSBOXTYPE 2  // funkcja
	#else
		#define INVSBOXTYPE 1  // tablica
	#endif
#else
	#define INVSBOXTYPE 0      // brak
#endif


#if SBOXTYPE==2 && INVSBOXTYPE!=2 
#if AES_SHORTSBOXSIZE > 192
#undef SBOXTYPE
#define SBOXTYPE 1
#endif
#endif

#if SBOXTYPE!=2 && INVSBOXTYPE==2
#if AES_SHORTSBOXSIZE > 192
#undef INVSBOXTYPE
#define INVSBOXTYPE 1
#endif
#endif


#if SBOXTYPE==1
	static const byte Sbox[] PROGMEM = { SBOX_DATA_INIT };
	#define SBOX(x) (pgm_read_byte(Sbox+(x)))
#endif


#if INVSBOXTYPE==1
	static const byte InvSbox[] PROGMEM = { ISBOX_DATA_INIT };
	#define INVSBOX(x) (pgm_read_byte(InvSbox+(x)))
#endif


#if SBOXTYPE==2 && INVSBOXTYPE==2
	#define SBOX(x) (aesCalcSbox((x),0))
	#define INVSBOX(x) (aesCalcSbox((x),1))
#else
	#if SBOXTYPE==2
		#define SBOX(x) (aesCalcSbox(x))
	#endif
	#if INVSBOXTYPE==2
		#define INVSBOX(x) (aesCalcSbox(x))
	#endif
#endif


#if SBOXTYPE==2 || INVSBOXTYPE==2

#if AES_SHORTSBOXSIZE < 10
#error AES_SHORTSBOXSIZE too small
#endif

#if AES_SHORTSBOXSIZE > 256
#undef AES_SHORTSBOXSIZE
#define AES_SHORTSBOXSIZE 256
#endif

static const byte invGf28[AES_SHORTSBOXSIZE] PROGMEM = {
	0x00,0x01,0x8d,0xf6,
	0xcb,0x52,0x7b,0xd1,
	0xe8,0x4f,
	#if AES_SHORTSBOXSIZE > 10
	0x29,0xc0,
	#endif
	#if AES_SHORTSBOXSIZE > 12
	0xb0,0xe1,0xe5,0xc7,
	#endif
	#if AES_SHORTSBOXSIZE > 16
	0x74,0xb4,0xaa,0x4b,
	#endif
	#if AES_SHORTSBOXSIZE > 20
	0x99,0x2b,0x60,0x5f,
	#endif
	#if AES_SHORTSBOXSIZE > 24
	0x58,0x3f,0xfd,0xcc,
	#endif
	#if AES_SHORTSBOXSIZE > 28
	0xff,0x40,0xee,0xb2,
	#endif
	#if AES_SHORTSBOXSIZE > 32
	0x3a,0x6e,0x5a,0xf1,
	#endif
	#if AES_SHORTSBOXSIZE > 36
	0x55,0x4d,0xa8,0xc9,
	#endif
	#if AES_SHORTSBOXSIZE > 40
	0xc1,0x0a,0x98,0x15,
	#endif
	#if AES_SHORTSBOXSIZE > 44
	0x30,0x44,0xa2,0xc2,
	#endif
	#if AES_SHORTSBOXSIZE > 48
	0x2c,0x45,0x92,0x6c,
	#endif
	#if AES_SHORTSBOXSIZE > 52
	0xf3,0x39,0x66,0x42,
	#endif
	#if AES_SHORTSBOXSIZE > 56
	0xf2,0x35,0x20,0x6f,
	#endif
	#if AES_SHORTSBOXSIZE > 60
	0x77,0xbb,0x59,0x19,
	#endif
	#if AES_SHORTSBOXSIZE > 64
	0x1d,0xfe,0x37,0x67,
	#endif
	#if AES_SHORTSBOXSIZE > 68
	0x2d,0x31,0xf5,0x69,
	#endif
	#if AES_SHORTSBOXSIZE > 72
	0xa7,0x64,0xab,0x13,
	#endif
	#if AES_SHORTSBOXSIZE > 76
	0x54,0x25,0xe9,0x09,
	#endif
	#if AES_SHORTSBOXSIZE > 80
	0xed,0x5c,0x05,0xca,
	#endif
	#if AES_SHORTSBOXSIZE > 84
	0x4c,0x24,0x87,0xbf,
	#endif
	#if AES_SHORTSBOXSIZE > 88
	0x18,0x3e,0x22,0xf0,
	#endif
	#if AES_SHORTSBOXSIZE > 92
	0x51,0xec,0x61,0x17,
	#endif
	#if AES_SHORTSBOXSIZE > 96
	0x16,0x5e,0xaf,0xd3,
	#endif
	#if AES_SHORTSBOXSIZE > 100
	0x49,0xa6,0x36,0x43,
	#endif
	#if AES_SHORTSBOXSIZE > 104
	0xf4,0x47,0x91,0xdf,
	#endif
	#if AES_SHORTSBOXSIZE > 108
	0x33,0x93,0x21,0x3b,
	#endif
	#if AES_SHORTSBOXSIZE > 112
	0x79,0xb7,0x97,0x85,
	#endif
	#if AES_SHORTSBOXSIZE > 116
	0x10,0xb5,0xba,0x3c,
	#endif
	#if AES_SHORTSBOXSIZE > 120
	0xb6,0x70,0xd0,0x06,
	#endif
	#if AES_SHORTSBOXSIZE > 124
	0xa1,0xfa,0x81,0x82,
	#endif
	#if AES_SHORTSBOXSIZE > 128
	0x83,0x7e,0x7f,0x80,
	#endif
	#if AES_SHORTSBOXSIZE > 132
	0x96,0x73,0xbe,0x56,
	#endif
	#if AES_SHORTSBOXSIZE > 136
	0x9b,0x9e,0x95,0xd9,
	#endif
	#if AES_SHORTSBOXSIZE > 140
	0xf7,0x02,0xb9,0xa4,
	#endif
	#if AES_SHORTSBOXSIZE > 144
	0xde,0x6a,0x32,0x6d,
	#endif
	#if AES_SHORTSBOXSIZE > 148
	0xd8,0x8a,0x84,0x72,
	#endif
	#if AES_SHORTSBOXSIZE > 152
	0x2a,0x14,0x9f,0x88,
	#endif
	#if AES_SHORTSBOXSIZE > 156
	0xf9,0xdc,0x89,0x9a,
	#endif
	#if AES_SHORTSBOXSIZE > 160
	0xfb,0x7c,0x2e,0xc3,
	#endif
	#if AES_SHORTSBOXSIZE > 164
	0x8f,0xb8,0x65,0x48,
	#endif
	#if AES_SHORTSBOXSIZE > 168
	0x26,0xc8,0x12,0x4a,
	#endif
	#if AES_SHORTSBOXSIZE > 172
	0xce,0xe7,0xd2,0x62,
	#endif
	#if AES_SHORTSBOXSIZE > 176
	0x0c,0xe0,0x1f,0xef,
	#endif
	#if AES_SHORTSBOXSIZE > 180
	0x11,0x75,0x78,0x71,
	#endif
	#if AES_SHORTSBOXSIZE > 184
	0xa5,0x8e,0x76,0x3d,
	#endif
	#if AES_SHORTSBOXSIZE > 188
	0xbd,0xbc,0x86,0x57,
	#endif
	#if AES_SHORTSBOXSIZE > 192
	0x0b,0x28,0x2f,0xa3,
	#endif
	#if AES_SHORTSBOXSIZE > 196
	0xda,0xd4,0xe4,0x0f,
	#endif
	#if AES_SHORTSBOXSIZE > 200
	0xa9,0x27,0x53,0x04,
	#endif
	#if AES_SHORTSBOXSIZE > 204
	0x1b,0xfc,0xac,0xe6,
	#endif
	#if AES_SHORTSBOXSIZE > 208
	0x7a,0x07,0xae,0x63,
	#endif
	#if AES_SHORTSBOXSIZE > 212
	0xc5,0xdb,0xe2,0xea,
	#endif
	#if AES_SHORTSBOXSIZE > 216
	0x94,0x8b,0xc4,0xd5,
	#endif
	#if AES_SHORTSBOXSIZE > 220
	0x9d,0xf8,0x90,0x6b,
	#endif
	#if AES_SHORTSBOXSIZE > 224
	0xb1,0x0d,0xd6,0xeb,
	#endif
	#if AES_SHORTSBOXSIZE > 228
	0xc6,0x0e,0xcf,0xad,
	#endif
	#if AES_SHORTSBOXSIZE > 232
	0x08,0x4e,0xd7,0xe3,
	#endif
	#if AES_SHORTSBOXSIZE > 236
	0x5d,0x50,0x1e,0xb3,
	#endif
	#if AES_SHORTSBOXSIZE > 240
	0x5b,0x23,0x38,0x34,
	#endif
	#if AES_SHORTSBOXSIZE > 244
	0x68,0x46,0x03,0x8c,
	#endif
	#if AES_SHORTSBOXSIZE > 248
	0xdd,0x9c,0x7d,0xa0,
	#endif
	#if AES_SHORTSBOXSIZE > 252
	0xcd,0x1a,0x41,0x1c,
	#endif
};

#if SBOXTYPE==2 && INVSBOXTYPE==2

static byte aesCalcSbox(byte x, byte inv)
{
	byte c0x1b = 0x1b;
	byte a, b;

	if (inv) {
		x ^= 0x63;
		a = 0;
		b = 0x4a;
matxor:
		while (x) {
			if (x&1) a ^= b;
			#ifdef AES_USEASM
			asm ("lsl %0 \n brcc .+2 \n ori %0, 1" : "=d" (b) : "0" (b));
			#else
			b = (b<<1) | (b>>7);
			#endif
			x >>= 1;
		}
		if (!inv) return a;
		x = a;
	}
	
	#if AES_SHORTSBOXSIZE < 256
		a = 0;
		while (x >= AES_SHORTSBOXSIZE) {
			GF28_MUL2(x);
			a++;
		}
		x = pgm_read_byte(invGf28 + x);
		while (a--) GF28_MUL2(x);
	#else
		x = pgm_read_byte(invGf28 + x);
	#endif

	if (!inv) {
		a = 0x63;
		b = 0x1f;
		goto matxor;
	}
	return x;
}

#elif SBOXTYPE==2

static byte aesCalcSbox(byte x)
{
	byte c0x1b = 0x1b;
	byte a, b;
	
	a = 0;
	while (x >= AES_SHORTSBOXSIZE) {
		GF28_MUL2(x);
		a++;
	}
	x = pgm_read_byte(invGf28 + x);
	while (a--) GF28_MUL2(x);
	// x^-1 = a2

	a = 0x63;
	b = 0x1f;
	while (x) {
		if (x&1) a ^= b;
		#ifdef AES_USEASM
		asm ("lsl %0 \n brcc .+2 \n ori %0, 1" : "=d" (b) : "0" (b));
		#else
		b = (b<<1) | (b>>7);
		#endif
		x >>= 1;
	}
	return a;
}

#elif INVSBOXTYPE==2

static byte aesCalcSbox(byte x)
{
	byte c0x1b = 0x1b;
	byte a, b;

	x ^= 0x63;
	a = 0;
	b = 0x4a;
	while (x) {
		if (x&1) a ^= b;
		#ifdef AES_USEASM
		asm ("lsl %0 \n brcc .+2 \n ori %0, 1" : "=d" (b) : "0" (b));
		#else
		b = (b<<1) | (b>>7);
		#endif
		x >>= 1;
	}

	b = 0;
	while (a >= AES_SHORTSBOXSIZE) {
		GF28_MUL2(a);
		b++;
	}
	a = pgm_read_byte(invGf28 + a);
	while (b--) GF28_MUL2(a);

	return a;
}

#endif

#endif


#if AES_CIPHER || AES_KEYPATCH

static void aesAddRoundKey(byte* state, byte* key)
{
	byte c0x1b = 0x1b;
	byte *ptr;
	byte a, b, c, d, t, i;
	for (i = 0; i < 16; i+=4) {
		ptr = key + ((i-4)&12);
		a = ptr[0];
		b = ptr[1];
		c = ptr[2];
		d = ptr[3];
		if (i == 0) {
			t = b;
			b = SBOX(c);
			c = SBOX(d);
			d = SBOX(a);
			a = SBOX(t) ^ Rcon;
			GF28_MUL2(Rcon);
		}
		ptr = key + i;
		t = ptr[0]; ptr[0] = a ^ t; a = t;
		t = ptr[1]; ptr[1] = b ^ t; b = t;
		t = ptr[2]; ptr[2] = c ^ t; c = t;
		t = ptr[3]; ptr[3] = d ^ t; d = t;
		if (state) {
			*state++ ^= a;
			*state++ ^= b;
			*state++ ^= c;
			*state++ ^= d;
		}
	}
}

#endif

#if AES_INVCIPHER || AES_KEYREWIND

static void aesInvAddRoundKey(byte* state, byte* key)
{
	byte c0x8d = 0x8d;
	byte *ptr;
	byte a, b, c, d, t, i;
	for (i = 12; !(i&0x80); i-=4) {
		ptr = key + ((i-4)&12);
		a = ptr[0];
		b = ptr[1];
		c = ptr[2];
		d = ptr[3];
		if (i == 0) {
			GF28_DIV2(Rcon);
			t = a;
			a = SBOX(b) ^ Rcon;
			b = SBOX(c);
			c = SBOX(d);
			d = SBOX(t);
		}
		ptr = key + i;
		a ^= ptr[0]; ptr[0] = a;
		b ^= ptr[1]; ptr[1] = b;
		c ^= ptr[2]; ptr[2] = c;
		d ^= ptr[3]; ptr[3] = d;
		if (state) {
			ptr = state + i;
			ptr[0] ^= a;
			ptr[1] ^= b;
			ptr[2] ^= c;
			ptr[3] ^= d;
		}           
	}
	
}

#endif


#if AES_CIPHER
static void aesSubBytes(byte* state)
{
	byte i;
	for (i=0; i<16; i++) {
		*state = SBOX(*state);
		state++;
	}
}
#endif

#if AES_INVCIPHER
static void aesInvSubBytes(byte* state)
{
	byte i;
	for (i=0; i<16; i++) {
		*state = INVSBOX(*state);
		state++;
	}
}
#endif


#if AES_FASTSHROWS

#if AES_CIPHER
static void aesShiftRows(byte* state)
{
	byte t;
	t = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = t;
	t = state[10];
	state[10] = state[2];
	state[2] = t;
	t = state[14];
	state[14] = state[6];
	state[6] = t;
	t = state[3];
	state[3] = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = t;
}
#endif


#if AES_INVCIPHER
static void aesInvShiftRows(byte* state)
{
	byte t;
	t = state[1];
	state[1] = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = t;
	t = state[10];
	state[10] = state[2];
	state[2] = t;
	t = state[14];
	state[14] = state[6];
	state[6] = t;
	t = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = t;
}
#endif

#else 

#if AES_CIPHER && AES_INVCIPHER

#define aesShiftRows(s) aesShiftRows_B((s),0)
#define aesInvShiftRows(s) aesShiftRows_B((s),1)

static void aesShiftRows_B(byte* state, byte inv)

#elif AES_CIPHER

static void aesShiftRows(byte* state)

#else

static void aesInvShiftRows(byte* state)

#endif
{
	#if !AES_CIPHER || !AES_INVCIPHER
		#if AES_CIPHER
			byte inv = 0;
		#else
			byte inv = 1;
		#endif
	#endif
	byte *ptr;
	byte i, j, k, t;
	for (i=1; i<4; i++) {
		for (j=i; j<4; j++) {
			ptr = state + j;
			if (inv) ptr += 8;
			for (k=j; k<12; k+=4) {
				t = ptr[0];
				ptr[0] = ptr[4];
				ptr[4] = t;
				if (inv) {
					ptr -= 4;
				} else {
					ptr += 4;
				}
			}
		}
	}
}

#endif

#if !AES_CIPHER || !AES_INVCIPHER
#undef AES_FASTMIXCOL
#define AES_FASTMIXCOL 1
#endif

#if AES_FASTMIXCOL

#if AES_CIPHER
static void aesMixColumns(byte* state)
{
	byte c0x1b = 0x1b;
	byte i, a0, a1, a2, a3, sum, b;
	byte* ptr = state;
	for (i = 0; i < 16; i+=4) {
		a0 = ptr[0];
		a1 = ptr[1];
		a2 = ptr[2];
		a3 = ptr[3];
		sum = a0 ^ a1 ^ a2 ^ a3;
		b = a0^a1; GF28_MUL2(b);
		*ptr++ = sum ^ a0 ^ b;
		b = a1^a2; GF28_MUL2(b);
		*ptr++ = sum ^ a1 ^ b;
		b = a2^a3; GF28_MUL2(b);
		*ptr++ = sum ^ a2 ^ b;
		b = a3^a0; GF28_MUL2(b);
		*ptr++ = sum ^ a3 ^ b;
	}
}
#endif


#if AES_INVCIPHER
static void aesInvMixColumns(byte* state)
{
	byte c0x1b = 0x1b;
	byte i, a0, a1, a2, a3, sum, b, c, d, e;
	byte* ptr = state;
	for(i = 0; i < 16; i+=4)
	{
		a0 = ptr[0];
		a1 = ptr[1];
		a2 = ptr[2];
		a3 = ptr[3];
		sum = a0 ^ a1 ^ a2 ^ a3;
		c = sum; GF28_MUL2(c);
		d = c ^ a0 ^ a2; GF28_MUL2(d); GF28_MUL2(d);
		d ^= sum;
		e = c ^ a1 ^ a3; GF28_MUL2(e); GF28_MUL2(e);
		e ^= sum;
		b = a0^a1; GF28_MUL2(b);
		*ptr++ = d ^ a0 ^ b;
		b = a1^a2; GF28_MUL2(b);
		*ptr++ = e ^ a1 ^ b;
		b = a2^a3; GF28_MUL2(b);
		*ptr++ = d ^ a2 ^ b;
		b = a3^a0; GF28_MUL2(b);
		*ptr++ = e ^ a3 ^ b;
	}
}
#endif

#else

#define aesMixColumns(state) aesMixColumns_B(state, 0)
#define aesInvMixColumns(state) aesMixColumns_B(state, 1)

static void aesMixColumns_B(byte* state, byte inv) 
{
	byte c0x1b = 0x1b;
	byte i, a0, a1, a2, a3, sum, b, c, d, e;
	byte* ptr = state;
	for(i = 0; i < 16; i+=4)
	{
		a0 = ptr[0];
		a1 = ptr[1];
		a2 = ptr[2];
		a3 = ptr[3];
		sum = a0 ^ a1 ^ a2 ^ a3;
		if (inv) {
			c = sum; GF28_MUL2(c);
			d = c ^ a0 ^ a2; GF28_MUL2(d); GF28_MUL2(d);
			d ^= sum;
			e = c ^ a1 ^ a3; GF28_MUL2(e); GF28_MUL2(e);
			e ^= sum;
		} else {
			d = e = sum;
		}           
		b = a0^a1; GF28_MUL2(b);
		*ptr++ = d ^ a0 ^ b;
		b = a1^a2; GF28_MUL2(b);
		*ptr++ = e ^ a1 ^ b;
		b = a2^a3; GF28_MUL2(b);
		*ptr++ = d ^ a2 ^ b;
		b = a3^a0; GF28_MUL2(b);
		*ptr++ = e ^ a3 ^ b;
	}
}


#endif


#if AES_CIPHER
static void aesCipher(unsigned char* key, unsigned char* state)
{
	byte r;

	Rcon = 1;

	for (r = 0; r < 11; r++) {
		if (r) {
			aesSubBytes(state);
			aesShiftRows(state);
			if (r != 10) aesMixColumns(state);
		}
		aesAddRoundKey(state, key);
	}

}
#endif


#if AES_INVCIPHER
static void aesInvCipher(unsigned char* patched, unsigned char* state)
{
	byte r;

	Rcon = 0xD8;

	for (r = 10; !(r&0x80); r--) {
		aesInvAddRoundKey(state, patched);
		if (r) {
			if (r != 10) aesInvMixColumns(state);
			aesInvShiftRows(state);
			aesInvSubBytes(state);
		}
	}

}
#endif


#if AES_KEYREWIND
static void aesKeyRewind(unsigned char* patched)
{
	byte r;
	Rcon = 0xD8;
	for (r = 0; r < 11; r++) {
		aesInvAddRoundKey(0, patched);
	}       
}
#endif


#if AES_KEYPATCH
static void aesKeyPatch(unsigned char* key)
{
	byte r;
	Rcon = 1;
	for (r = 0; r < 11; r++) {
		aesAddRoundKey(0, key);
	}
}
#endif

static void mem_xor(uint8_t* out, uint8_t* xo)
{
	uint8_t i = 0;
	for(; i < 16; ++i)
	{
		out[i] ^= xo[i];
	}
}

void AES128_CBC_encrypt_buffer(uint8_t* input, uint8_t sz, uint8_t* key, uint8_t* iv)
{
	uint8_t i = 0;
	for(; i < sz; i+=16)
	{
		mem_xor(input, iv);
		aesCipher(key, input);  // This function return a modified key and accept the original key
		aesKeyRewind(key);  // Convert the modified key to the original
		iv=input;
		input+=16;
	}
}

void AES128_CBC_decrypt_buffer(uint8_t* input, uint8_t sz, uint8_t* key, uint8_t* iv)
{
	uint8_t i = 0;

	uint8_t tempCipher[16];
	uint8_t pos = 0;

	uint8_t *xorWith = iv;

	for(; i < sz; i+=16)
	{	
		uint8_t j = 0;
		if(pos == 0)
		{
			for(; j < 16; ++j)
			{
				tempCipher[j] = input[j];
			}
			pos = 1;
		}
		else
		{
			for(; j < 16; ++j)
			{
				iv[j] = input[j];
			}
			pos = 0;
		}

		aesKeyPatch(key);  // Patch the original key
		aesInvCipher(key, input);  // Decrypt data with a patched key. It returns the original key.
		mem_xor(input, xorWith);

		// If an element of the input contains a zero, and because only strings are encrypted
		// it means that the string is over. So return the result now because the others functions
		// in the programs will ignore the value of the byte after a 0 in a string.
		for(uint8_t j = 0; j < 16; ++j)
		{
			if(input[j] == 0)
			{
				return;
			}
		}

		xorWith = (pos == 1) ? tempCipher : iv;
		input+=16;
	}
}
