#include <stdio.h>

#include "avr-mate-core/cartridge.h"
#include "cartridge/haw_cartridge.h"
#include "cartridge/haw_logger.h"
#include "cartridge/haw_utils.h"

#include <string.h>

int main(void)
{

	struct cartridge_dev dev;
	haw_cartridge_init(&dev);

	cartridge_clear_buffer();

	for (uint8_t y = 0; y < 64; ++y) {
		cartridge_set_pixel(126, y, COLOR_BLACK);
		cartridge_set_pixel(127, y, COLOR_BLACK);
	}

	const uint8_t y_offset = 0;
	const uint8_t x_offset = 0;

	cartridge_set_text(x_offset, y_offset + 0, "ABCDEFGHIJKLMNOPQRSTU");
	cartridge_set_text(x_offset, y_offset + 8, "abcdefghijklmnopqrstu");
	cartridge_set_text(x_offset, y_offset + 16, "012345678901234567890");
	cartridge_set_text(x_offset, y_offset + 24, "!\"$%&/()=?`'+*~#,.-{}");
	cartridge_set_text(x_offset, y_offset + 32, "ABCDEFGHIJKLMNOPQRSTU");
	cartridge_set_text(x_offset, y_offset + 40, "abcdefghijklmnopqrstu");
	cartridge_set_text(x_offset, y_offset + 48, "012345678901234567890");
	cartridge_set_text(x_offset, y_offset + 56, "!\"$%&/()=?`'+*~#,.-{}");

	cartridge_draw_buffer();

	while (1) {
		sleep_for_ms(100);
	}
}
