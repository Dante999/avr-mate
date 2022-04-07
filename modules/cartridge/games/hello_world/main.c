#include <stdio.h>

#include "avr-mate-core/cartridge.h"
#include "cartridge/haw_cartridge.h"
#include "cartridge/haw_logger.h"

int main(void)
{
	struct cartridge_dev dev;
	haw_cartridge_init(&dev);

	while (1) {
		cartridge_clear_buffer();
		cartridge_set_text(0, 0, "Hello World!");
		cartridge_draw_buffer();
	}
}
