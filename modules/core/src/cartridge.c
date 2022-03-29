#include "avr-mate-core/cartridge.h"

#include <string.h>

static struct cartridge_dev *g_cfg;
static struct package g_answer_package;


static void transmit_and_wait_for_answer(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	g_cfg->start_transmit_cb();
	protocol_send_package(&g_cfg->protocol, cmd, length, data);
	g_cfg->end_transmit_cb();
	protocol_waitfor_package(&g_cfg->protocol, &g_answer_package);
}

void cartridge_init(struct cartridge_dev *cfg)
{
	g_cfg = cfg;
}

enum result cartridge_ping()
{
	transmit_and_wait_for_answer(CMD_PING, 0, NULL);

	return g_answer_package.cmd == CMD_PONG ? RESULT_OK : RESULT_WRONG_COMMAND;
}

void cartridge_sync_with_handheld()
{
	protocol_sync(&g_cfg->protocol);
}

enum result cartridge_set_pixel(uint8_t x, uint8_t y,
                                enum cp_color color)
{
	struct cp_pixel pixel = {x, y, color};

	transmit_and_wait_for_answer(CMD_SET_PIXEL, sizeof(pixel),
	                             (uint8_t *)(&pixel));

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_set_text(uint8_t x, uint8_t y, const char *text)
{
	struct cp_text data = {x, y, ""};

	strncpy(data.text, text, PAYLOAD_MAX_TEXT_LENGTH - 1);

	transmit_and_wait_for_answer(CMD_SET_TEXT, sizeof(data),
	                             (uint8_t*)(&data));

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_set_char(uint8_t x, uint8_t y, char c)
{
	struct cp_char data = {x, y, c};

	transmit_and_wait_for_answer(CMD_SET_CHAR, sizeof(data),
	                             (uint8_t*)(&data));

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_set_tile_8x8(uint8_t x, uint8_t y, const uint8_t tile[])
{
	struct cp_tile_8x8 data;

	data.x = x;
	data.y = y;

	memcpy(data.tile, tile, 8);

	transmit_and_wait_for_answer(CMD_SET_TILE_8X8, sizeof(data),
	                             (uint8_t*)(&data));

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_get_buttons(struct cp_buttons *buttons)
{
	transmit_and_wait_for_answer(CMD_GET_BUTTONS, 0, NULL);

	memcpy(buttons, g_answer_package.payload, sizeof(*buttons));

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_draw_buffer()
{
	transmit_and_wait_for_answer(CMD_DRAW_BUFFER, 0, NULL);

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

enum result cartridge_clear_buffer()
{
	transmit_and_wait_for_answer(CMD_CLEAR_BUFFER, 0, NULL);

	return g_answer_package.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}
