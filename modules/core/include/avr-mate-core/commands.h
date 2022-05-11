#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#pragma pack(1)

#define PAYLOAD_MAX_TEXT_LENGTH 22

enum result { RESULT_OK, RESULT_NOK, RESULT_WRONG_COMMAND };

enum commands {
	/* zero is reserverd for START_BYTE */
	CMD_SYNC = 1,
	CMD_ACK,
	CMD_NACK,
	CMD_NOT_SUPPORTED,
	CMD_PING,
	CMD_PONG,
	CMD_CHECK_VERSION,
	CMD_SET_PIXEL,
	CMD_SET_TEXT,
	CMD_SET_CHAR,
	CMD_SET_TILE_8X8,
	CMD_DRAW_BUFFER,
	CMD_CLEAR_BUFFER,
	CMD_GET_BUTTONS
};

enum cp_color { COLOR_WHITE, COLOR_BLACK };

enum cp_button_msk {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_START,
	BUTTON_SELECT
};

struct cp_text {
	uint8_t x;
	uint8_t y;
	char    text[PAYLOAD_MAX_TEXT_LENGTH];
};

struct cp_pixel {
	uint8_t       x;
	uint8_t       y;
	enum cp_color color;
};

struct cp_char {
	uint8_t x;
	uint8_t y;
	char    letter;
};

struct cp_tile_8x8 {
	uint8_t x;
	uint8_t y;
	uint8_t tile[8];
};

struct cp_buttons {
	uint16_t states;
};

#pragma pack(reset)

#endif /* COMMANDS_H */
