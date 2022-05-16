#include <stdio.h>

#include "avr-mate-core/cartridge.h"
#include "avr-mate-core/graphx.h"
#include "cartridge/haw_cartridge.h"
#include "cartridge/haw_logger.h"
#include "cartridge/haw_utils.h"

#include "ball.h"

int main(void)
{

	struct cartridge_dev dev;
	haw_cartridge_init(&dev);

	cartridge_set_text(0, 0, "bouncing ball");
	cartridge_draw_buffer();
	sleep_for_ms(2000);

	cartridge_clear_buffer();
	cartridge_draw_buffer();

	struct ball ball = {.x     = 10,
		            .y     = 50,
		            .dir   = DIR_UPLEFT,
		            .min_x = GRAPHX_MIN_X,
		            .max_x = GRAPHX_MAX_X,
		            .min_y = GRAPHX_MIN_Y,
		            .max_y = GRAPHX_MAX_Y};

	while (1) {
		cartridge_set_pixel(ball.x, ball.y, COLOR_WHITE);
		ball_bounce(&ball);
		cartridge_set_pixel(ball.x, ball.y, COLOR_BLACK);

		cartridge_draw_buffer();
	}
}
