#include <stdio.h>

#include "avr-mate-core/cartridge.h"
#include "cartridge/haw_cartridge.h"
#include "cartridge/haw_logger.h"

int main(void)
{
	struct cartridge_dev dev;

	LOG_INFO("haw_cartridge_init");
	haw_cartridge_init(&dev);

	LOG_INFO("cartridge_init");
	cartridge_init(&dev);

	LOG_INFO("syncing with handheld...");
	cartridge_sync_with_handheld();

	enum result ret = cartridge_ping();

	if (ret != RESULT_OK) {
		LOG_WARNING("Result of ping was not pong");
	}
	else {
		LOG_INFO("Ping successful");
	}

	while (1) {
		cartridge_clear_buffer();
		cartridge_set_text(0, 0, "Hello World!");
		cartridge_draw_buffer();
	}
}
