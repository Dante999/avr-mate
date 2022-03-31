#include "avr-mate-core/handheld.h"
#include "avr-mate-core/font5x7.h"

#include <stddef.h>

struct handheld_dev *g_cfg;

static void handheld_transmit(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	g_cfg->publish_ready();
	protocol_send_package(&g_cfg->protocol, cmd, length, data);
	g_cfg->publish_busy();
}

static void response_with(uint8_t cmd)
{
	handheld_transmit(cmd, 0, NULL);
}

static void cmd_ping()
{
	response_with(CMD_PONG);
}

static void cmd_set_pixel(uint8_t *data)
{
	struct cp_pixel *pixel = (struct cp_pixel *)data;

	graphx_draw_pixel(pixel->x, pixel->y,
	                  (pixel->color == COLOR_BLACK) ? PIXEL_ON : PIXEL_OFF);

	response_with(CMD_ACK);
}

static void cmd_set_text(uint8_t *data)
{
	struct cp_text *text = (struct cp_text *)data;

	text->text[PAYLOAD_MAX_TEXT_LENGTH - 1] = '\0'; // safety first
	graphx_draw_string(font5x7, text->x, text->y, text->text);

	response_with(CMD_ACK);
}

static void cmd_set_char(uint8_t *data)
{
	struct cp_char *character = (struct cp_char *)data;

	graphx_draw_char(font5x7, character->x, character->y,
	                 character->letter);

	response_with(CMD_ACK);
}

static void cmd_set_tile_8x8(uint8_t *data)
{
	struct cp_tile_8x8 *tile = (struct cp_tile_8x8 *)data;

	graphx_draw_tile(tile->x, tile->y, tile->tile, 8, 8);

	response_with(CMD_ACK);
}

static void cmd_draw_buffer()
{
	g_cfg->draw_buffer(graphx_get_buffer());
	response_with(CMD_ACK);
}

static void cmd_clear_buffer()
{
	graphx_fill(PIXEL_OFF);
	response_with(CMD_ACK);
}

static void cmd_get_buttons()
{
	struct cp_buttons buttons;

	g_cfg->get_buttons(&buttons);
	handheld_transmit(CMD_ACK, sizeof(buttons), (uint8_t *)(&buttons));
}

void handheld_init(struct handheld_dev *cfg)
{
	g_cfg = cfg;
	graphx_fill(0x00);
	protocol_init(&cfg->protocol);
}

void handheld_waitfor_instructions()
{
	struct package received;

	g_cfg->publish_ready();
	protocol_waitfor_package(&g_cfg->protocol, &received);
	g_cfg->publish_busy();

	switch (received.cmd) {
	case CMD_PING:
		cmd_ping();
		break;
	case CMD_SET_PIXEL:
		cmd_set_pixel(received.payload);
		break;
	case CMD_SET_TEXT:
		cmd_set_text(received.payload);
		break;
	case CMD_SET_CHAR:
		cmd_set_char(received.payload);
		break;
	case CMD_SET_TILE_8X8:
		cmd_set_tile_8x8(received.payload);
		break;
	case CMD_DRAW_BUFFER:
		cmd_draw_buffer();
		break;
	case CMD_CLEAR_BUFFER:
		cmd_clear_buffer();
		break;
	case CMD_GET_BUTTONS:
		cmd_get_buttons();
		break;
	default:
		response_with(CMD_NOT_SUPPORTED);
		break;
	}
}
