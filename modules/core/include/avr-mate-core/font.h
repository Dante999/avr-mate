#ifndef FONT_HPP
#define FONT_HPP

#include <stdint.h>

#ifdef __AVR__
#	include <avr/pgmspace.h>
#endif

#define FONT_OFFSET_WIDTH  0
#define FONT_OFFSET_HEIGHT 1
#define FONT_OFFSET_DATA   2

inline uint8_t font_get_width(const uint8_t font[])
{
#ifdef __AVR__
	return pgm_read_byte(&font[FONT_OFFSET_WIDTH]);
#else
	return font[FONT_OFFSET_WIDTH];
#endif
}

inline uint8_t font_get_height(const uint8_t font[])
{
#ifdef __AVR__
	return pgm_read_byte(&font[FONT_OFFSET_HEIGHT]);
#else
	return font[FONT_OFFSET_HEIGHT];
#endif
}

inline uint8_t font_get_byte(const uint8_t font[], uint16_t index)
{
#ifdef __AVR__
	return pgm_read_byte(&font[FONT_OFFSET_DATA + index]);
#else
	return font[FONT_OFFSET_DATA + index];
#endif
}

#endif /* FONT_HPP */
