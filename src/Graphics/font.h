#ifndef FONT_HEADER_THOMAS_CHEVALIER
#define FONT_HEADER_THOMAS_CHEVALIER

#include <avr/pgmspace.h>

#undef SMALL_FONT
#define BIG_FONT

#ifdef SMALL_FONT
#define FONT_WIDTH (4)
#define FONT_HEIGHT (5)
// Minimal font
static const uint8_t font[] PROGMEM =
{
	0x00, 0x00, 0x00, 0x00, /* space */
	0x17, 0x00, 0x00, 0x00, /* ! */
	0x01, 0x02, 0x01, 0x02, /* " */
	0x0A, 0x1F, 0x1F, 0x0A, /* # */
	0x17, 0x1D, 0x17, 0x1D,	/* $ */ /* not defined */
	0x19, 0x04, 0x13, 0x00, /* % */
	0x00, 0x00, 0x00, 0x00, /* & */
	0x03, 0x00, 0x00, 0x00, /* ' */
	0x0E, 0x11, 0x00, 0x00, /* ( */
	0x11, 0x0E, 0x00, 0x00, /* ) */
	0x0A, 0x04, 0x0A, 0x00, /* * */
	0x04, 0x0E, 0x04, 0x00, /* + */
	0x20, 0x10, 0x00, 0x00, /* , */
	0x04, 0x04, 0x04, 0x00, /* - */
	0x10, 0x00, 0x00, 0x00, /* . */
	0x18, 0x04, 0x03, 0x00, /* / */
	0x0E, 0x11, 0x0E, 0x00, /* 0 */
	0x12, 0x1F, 0x10, 0x00, /* 1 */
	0x19, 0x15, 0x12, 0x00, /* 2 */
	0x11, 0x15, 0x0A, 0x00, /* 3 */
	0x07, 0x04, 0x1E, 0x00, /* 4 */
	0x17, 0x15, 0x19, 0x00, /* 5 */
	0x1E, 0x15, 0x19, 0x00, /* 6 */
	0x19, 0x05, 0x03, 0x00, /* 7 */
	0x1B, 0x15, 0x1B, 0x00, /* 8 */
	0x17, 0x15, 0x1F, 0x00, /* 9 */
	0x0A, 0x00, 0x00, 0x00, /* : */
	0x20, 0x14, 0x00, 0x00, /* ; */
	0x04, 0x0A, 0x11, 0x00, /* < */
	0x0A, 0x0A, 0x0A, 0x00, /* = */
	0x11, 0x0A, 0x04, 0x00, /* > */
	0x01, 0x15, 0x02, 0x00, /* ? */
	0x1F, 0x11, 0x15, 0x03, /* @ */
	0x1E, 0x05, 0x05, 0x1E, /* A */
	0x1F, 0x15, 0x15, 0x0A, /* B */
	0x0E, 0x11, 0x11, 0x11, /* C */
	0x1F, 0x11, 0x11, 0x0E, /* D */
	0x1F, 0x15, 0x15, 0x11, /* E */
	0x1F, 0x05, 0x05, 0x01, /* F */
	0x0E, 0x11, 0x15, 0x1D, /* G */
	0x1F, 0x04, 0x04, 0x1F, /* H */
	0x11, 0x1F, 0x11, 0x00, /* I */
	0x08, 0x11, 0x0F, 0x01, /* J */
	0x1F, 0x04, 0x0A, 0x11, /* K */
	0x1F, 0x10, 0x10, 0x10, /* L */
	0x1E, 0x01, 0x01, 0x1E, /* M */
	0x1F, 0x02, 0x04, 0x1F, /* N */
	0x1F, 0x11, 0x11, 0x1F, /* O */
	0x1F, 0x05, 0x05, 0x02, /* P */
	0x0F, 0x09, 0x0F, 0x10, /* Q */
	0x1F, 0x05, 0x05, 0x1A, /* R */
	0x12, 0x15, 0x15, 0x09, /* S */
	0x01, 0x1F, 0x01, 0x00, /* T */
	0x1F, 0x10, 0x10, 0x1F, /* U */
	0x0F, 0x10, 0x10, 0x0F, /* V */
	0x0F, 0x18, 0x18, 0x0F, /* W */
	0x1B, 0x04, 0x04, 0x1B, /* X */
	0x03, 0x14, 0x18, 0x07, /* Y */
	0x11, 0x19, 0x15, 0x13, /* Z */
	0x1F, 0x11, 0x00, 0x00, /* [ */
	0x03, 0x04, 0x18, 0x00, /* \ */
	0x11, 0x1F, 0x00, 0x00, /* ] */
	0x02, 0x01, 0x02, 0x00, /* ^ */
	0x08, 0x08, 0x08, 0x00, /* _ */
	0x01, 0x02, 0x00, 0x00, /* ` */
	0x08, 0x14, 0x1C, 0x00, /* a*/
	0x1E, 0x14, 0x1C, 0x00, /* b*/
	0x1C, 0x14, 0x14, 0x00, /* c*/
	0x1C, 0x14, 0x1E, 0x00, /* d*/
	0x0C, 0x1A, 0x14, 0x00, /* e*/
	0x08, 0x1E, 0x0A, 0x00, /* f*/
	0x14, 0x1A, 0x0C, 0x00, /* g*/
	0x1E, 0x08, 0x18, 0x00, /* h*/
	0x08, 0x1A, 0x10, 0x00, /* i*/
	0x08, 0x10, 0x1A, 0x00, /* j*/
	0x1E, 0x08, 0x14, 0x00, /* k*/
	0x12, 0x1E, 0x10, 0x00, /* l*/
	0x1C, 0x04, 0x1C, 0x00, /* m*/
	0x1C, 0x04, 0x18, 0x00, /* n*/
	0x1C, 0x14, 0x1C, 0x00, /* o*/
	0x1E, 0x0A, 0x04, 0x00, /* p*/
	0x04, 0x0A, 0x14, 0x00, /* q*/
	0x1C, 0x08, 0x04, 0x00, /* r*/
	0x14, 0x0A, 0x00, 0x00, /* s*/
	0x04, 0x1E, 0x14, 0x00, /* t*/
	0x1C, 0x10, 0x1C, 0x00, /* u*/
	0x0C, 0x10, 0x0C, 0x00, /* v*/
	0x0C, 0x18, 0x0C, 0x00, /* w*/
	0x14, 0x08, 0x14, 0x00, /* x*/
	0x14, 0x08, 0x04, 0x00, /* y*/
	0x04, 0x1C, 0X10, 0x00, /* z*/
	0x04, 0x1B, 0x11, 0x00, /* { */
	0x1F, 0x00, 0x00, 0x00, /* | */
	0x11, 0x1B, 0x04, 0x00, /* } */
	0x02, 0x01, 0x02, 0x01, /* ~ */
	0x15, 0x0A, 0x15, 0x0A  // Null character
};
#endif

#ifdef BIG_FONT
#define FONT_WIDTH (5)
#define FONT_HEIGHT (8)
// Bigger font
static const uint8_t font[] PROGMEM =
{
	0x00, 0x00, 0x00, 0x00, 0x00, /*   */
	0xBE, 0x00, 0x00, 0x00, 0x00, /* ! */
	0x06, 0x00, 0x06, 0x00, 0x00, /* " */
	0x28, 0x7C, 0x28, 0x7C, 0x28, /* # */
	0x44, 0x4A, 0xFF, 0x4A, 0x32, /* $ */
	0x46, 0x26, 0x10, 0xC8, 0xC4, /* % */
	0x6C, 0x92, 0xAA, 0x44, 0xA0, /* & */
	0x06, 0x00, 0x00, 0x00, 0x00, /* ' */
	0x38, 0x44, 0x82, 0x00, 0x00, /* ( */
	0x82, 0x44, 0x38, 0x00, 0x00, /* ) */
	0x28, 0x10, 0x7C, 0x10, 0x28, /* * */
	0x10, 0x10, 0x7C, 0x10, 0x10, /* + */
	0xA0, 0x60, 0x00, 0x00, 0x00, /* , */
	0x10, 0x10, 0x10, 0x10, 0x10, /* - */
	0xC0, 0xC0, 0x00, 0x00, 0x00, /* . */
	0xC0, 0x20, 0x18, 0x04, 0x03, /* / */
	0x7C, 0xA2, 0x92, 0x8A, 0x7C, /* 0 */
	0x84, 0xFE, 0x80, 0x00, 0x00, /* 1 */
	0x84, 0xC2, 0xA2, 0x92, 0x8C, /* 2 */
	0x82, 0x92, 0x92, 0x92, 0x6C, /* 3 */
	0x30, 0x28, 0x24, 0xFE, 0x20, /* 4 */
	0x4E, 0x8A, 0x8A, 0x8A, 0x72, /* 5 */
	0x78, 0x94, 0x92, 0x92, 0x60, /* 6 */
	0xC6, 0x22, 0x12, 0x0A, 0x06, /* 7 */
	0x6C, 0x92, 0x92, 0x92, 0x6C, /* 8 */
	0x8C, 0x92, 0x92, 0x52, 0x3C, /* 9 */
	0xD8, 0xD8, 0x00, 0x00, 0x00, /* : */
	0xAC, 0x6C, 0x00, 0x00, 0x00, /* ; */
	0x10, 0x28, 0x44, 0x82, 0x00, /* < */
	0x28, 0x28, 0x28, 0x28, 0x28, /* = */
	0x82, 0x44, 0x28, 0x10, 0x00, /* > */
	0x04, 0x02, 0xA2, 0x12, 0x0C, /* ? */
	0x64, 0x92, 0xF2, 0x82, 0x7C, /* @ */
	0xFC, 0x12, 0x12, 0x12, 0xFC, /* A */
	0xFE, 0x82, 0x92, 0x92, 0x6C, /* B */
	0x7C, 0x82, 0x82, 0x82, 0x44, /* C */
	0xFE, 0x82, 0x82, 0x82, 0x7C, /* D */
	0xFE, 0x92, 0x92, 0x92, 0x82, /* E */
	0xFE, 0x12, 0x12, 0x12, 0x02, /* F */
	0x7C, 0x82, 0x92, 0x92, 0xF4, /* G */
	0xFE, 0x10, 0x10, 0x10, 0xFE, /* H */
	0x82, 0x82, 0xFE, 0x82, 0x82, /* I */
	0x62, 0x82, 0x82, 0x7E, 0x02, /* J */
	0xFE, 0x10, 0x28, 0x28, 0xC6, /* K */
	0xFE, 0x80, 0x80, 0x80, 0x80, /* L */
	0xFE, 0x04, 0x18, 0x04, 0xFE, /* M */
	0xFE, 0x08, 0x30, 0x40, 0xFE, /* N */
	0x7C, 0x82, 0x82, 0x82, 0x7C, /* O */
	0xFE, 0x12, 0x12, 0x12, 0x0C, /* P */
	0x7C, 0x82, 0xA2, 0x42, 0xBC, /* Q */
	0xFE, 0x12, 0x32, 0x52, 0x8C, /* R */
	0x4C, 0x92, 0x92, 0x92, 0x64, /* S */
	0x02, 0x02, 0xFE, 0x02, 0x02, /* T */
	0x7E, 0x80, 0x80, 0x80, 0x7E, /* U */
	0x3E, 0x40, 0x80, 0x40, 0x3E, /* V */
	0x7E, 0x80, 0x70, 0x80, 0x7E, /* W */
	0xC6, 0x28, 0x10, 0x28, 0xC6, /* X */
	0x0E, 0x10, 0xE0, 0x10, 0x0E, /* Y */
	0xC2, 0xA2, 0x92, 0x8A, 0x86, /* Z */
	0xFE, 0x82, 0x82, 0x00, 0x00, /* [ */
	0x03, 0x04, 0x18, 0x20, 0xC0, /* \ */
	0x82, 0x82, 0xFE, 0x00, 0x00, /* ] */
	0x08, 0x04, 0x02, 0x04, 0x08, /* ^ */
	0x80, 0x80, 0x80, 0x80, 0x80, /* _ */
	0x02, 0x04, 0x08, 0x00, 0x00, /* ` */
	0x60, 0x94, 0x94, 0x94, 0xF8, /* a */
	0xFE, 0x90, 0x88, 0x88, 0x70, /* b */
	0x78, 0x84, 0x84, 0x84, 0x48, /* c */
	0x70, 0x88, 0x88, 0x88, 0xFE, /* d */
	0x78, 0x94, 0x94, 0x94, 0x98, /* e */
	0x10, 0xFC, 0x12, 0x02, 0x04, /* f */
	0x08, 0x94, 0x94, 0x94, 0x7C, /* g */
	0xFE, 0x10, 0x08, 0x08, 0xF0, /* h */
	0x90, 0xFA, 0x80, 0x00, 0x00, /* i */
	0x40, 0x80, 0x88, 0x7A, 0x00, /* j */
	0xFE, 0x20, 0x50, 0x88, 0x00, /* k */
	0x82, 0xFE, 0x80, 0x00, 0x00, /* l */
	0xFC, 0x04, 0xF8, 0x04, 0xF8, /* m */
	0xFC, 0x08, 0x04, 0x04, 0xF8, /* n */
	0x78, 0x84, 0x84, 0x84, 0x78, /* o */
	0xFC, 0x24, 0x24, 0x24, 0x18, /* p */
	0x18, 0x24, 0x24, 0x28, 0xFC, /* q */
	0xFC, 0x10, 0x08, 0x04, 0x08, /* r */
	0x88, 0x94, 0x94, 0x94, 0x64, /* s */
	0x04, 0x7E, 0x84, 0x80, 0x40, /* t */
	0x7C, 0x80, 0x80, 0x40, 0xFC, /* u */
	0x3C, 0x40, 0x80, 0x40, 0x3C, /* v */
	0x7C, 0x80, 0x60, 0x80, 0x7C, /* w */
	0x84, 0x48, 0x30, 0x48, 0x84, /* x */
	0x0C, 0x90, 0x90, 0x90, 0x7C, /* y */
	0xC4, 0xA4, 0x94, 0x94, 0x8C, /* z */
	0x10, 0x6C, 0x82, 0x00, 0x00, /* { */
	0xFE, 0x00, 0x00, 0x00, 0x00, /* | */
	0x82, 0x6C, 0x10, 0x00, 0x00, /* } */
	0x10, 0x08, 0x08, 0x10, 0x08, /* ~ */
	0x55, 0xAA, 0x55, 0xAA, 0x55, /* Null character */
};
#endif

#if !defined(BIG_FONT) && !defined(SMALL_FONT)
#error A font must be used.
#endif

#if defined(BIG_FONT) && defined(SMALL_FONT)
#error Only one font can be used.
#endif

#endif // FONT_HEADER_THOMAS_CHEVALIER
