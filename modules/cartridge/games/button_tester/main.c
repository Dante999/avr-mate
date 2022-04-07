#include <stdio.h>

#include "avr-mate-core/cartridge.h"
#include "cartridge/haw_cartridge.h"
#include "cartridge/haw_logger.h"
#include "cartridge/haw_utils.h"

static char stc(struct cp_buttons *btns, enum cp_button_msk msk)
{
	return (btns->states & (1 << msk)) ? '1' : '0';
}

static void draw_keys()
{
	cartridge_clear_buffer();

	cartridge_set_text(5, 5, "Press buttons");

	cartridge_set_text(5, 20, "up    ");
	cartridge_set_text(5, 30, "down  ");
	cartridge_set_text(5, 40, "left  ");
	cartridge_set_text(5, 50, "right ");

	cartridge_set_text(75, 20, "A      ");
	cartridge_set_text(75, 30, "B      ");
	cartridge_set_text(75, 40, "start  ");
	cartridge_set_text(75, 50, "select ");
	cartridge_draw_buffer();
}

static void draw_values()
{
	struct cp_buttons btns;

	cartridge_get_buttons(&btns);

	cartridge_set_char(40, 20, stc(&btns, BUTTON_UP));
	cartridge_set_char(40, 30, stc(&btns, BUTTON_DOWN));
	cartridge_set_char(40, 40, stc(&btns, BUTTON_LEFT));
	cartridge_set_char(40, 50, stc(&btns, BUTTON_RIGHT));

	cartridge_set_char(120, 20, stc(&btns, BUTTON_A));
	cartridge_set_char(120, 30, stc(&btns, BUTTON_B));
	cartridge_set_char(120, 40, stc(&btns, BUTTON_START));
	cartridge_set_char(120, 50, stc(&btns, BUTTON_SELECT));

	cartridge_draw_buffer();
}

int main(void)
{
	struct cartridge_dev dev;
	haw_cartridge_init(&dev);

	draw_keys();
	while (1) {
		draw_values();
	}
}
