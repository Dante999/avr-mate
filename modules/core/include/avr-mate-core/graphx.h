#ifndef GRAPHX_H
#define GRAPHX_H

#include <stdint.h>

#include "font.h"

#define GRAPHX_WIDTH  128
#define GRAPHX_HEIGHT 64
#define GRAPHX_SIZE   (GRAPHX_WIDTH * (GRAPHX_HEIGHT / 8))
#define GRAPHX_MIN_X  0
#define GRAPHX_MIN_Y  0
#define GRAPHX_MAX_X  (GRAPHX_WIDTH - 1)
#define GRAPHX_MAX_Y  (GRAPHX_HEIGHT - 1)

enum graphx_color_e { PIXEL_OFF, PIXEL_ON, PIXEL_TOGGLE };

uint8_t graphx_get_pixel(uint8_t x, uint8_t y);

void graphx_get_tile(uint8_t x, uint8_t y, uint8_t *tile, uint8_t w, uint8_t h);
uint8_t *graphx_get_buffer();

void graphx_fill(uint8_t color);
void graphx_draw_char(const uint8_t font[], uint8_t x, uint8_t y, const char c);
void graphx_draw_string(const uint8_t font[], uint8_t x, uint8_t y,
                        const char *s);
void graphx_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void graphx_draw_hline(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color);
void graphx_draw_vline(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color);
void graphx_draw_rect(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1,
                      uint8_t color);
void graphx_draw_tile(uint8_t x, uint8_t y, const uint8_t *tile, uint8_t w,
                      uint8_t h);

#endif /* GRAPHX_H */
