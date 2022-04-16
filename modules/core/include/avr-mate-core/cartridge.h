#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "avr-mate-core/commands.h"
#include "avr-mate-core/protocol.h"

struct cartridge_dev {
	void (*before_transmit)();
	void (*after_transmit)();
	struct protocol_dev protocol;
};

void cartridge_init(struct cartridge_dev *cfg);

void cartridge_sync_with_handheld();

enum result cartridge_ping();
enum result cartridge_set_pixel(uint8_t x, uint8_t y, enum cp_color color);
enum result cartridge_set_text(uint8_t x, uint8_t y, const char *text);
enum result cartridge_set_char(uint8_t x, uint8_t y, char c);
enum result cartridge_set_tile_8x8(uint8_t x, uint8_t y, const uint8_t tile[8]);
enum result cartridge_get_buttons(struct cp_buttons *buttons);
enum result cartridge_draw_buffer();
enum result cartridge_clear_buffer();

#endif /* CARTRIDGE_H */
