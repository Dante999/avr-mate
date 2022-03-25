#include "unity.h"

#include "avr-mate-core/protocol.h"

#include <stdio.h>

enum state { WAITFOR_SYNC, WAITFOR_CMD, WAITFOR_LENGTH, WAITFOR_DATA };

static enum state          g_state            = WAITFOR_SYNC;
static bool                g_receive_complete = false;
static struct package      g_package;
static struct protocol_dev g_cfg;

void protocol_init(struct protocol_dev cfg)
{
	g_cfg = cfg;
	protocol_reset();
}

static bool parse_received_byte(uint8_t byte)
{
	static uint8_t i = 0;

	switch (g_state) {
	case WAITFOR_SYNC:

		if (byte == PROTOCOL_START_BYTE) {
			i                  = 0;
			g_state            = WAITFOR_CMD;
			g_receive_complete = false;
		}

		break;

	case WAITFOR_CMD:

		if (byte != PROTOCOL_START_BYTE) {
			g_package.cmd = byte;
			g_state       = WAITFOR_LENGTH;
		}

		break;

	case WAITFOR_LENGTH:

		g_package.length = byte;

		if (g_package.length > PROTOCOL_MAX_PAYLOAD)
			g_package.length = PROTOCOL_MAX_PAYLOAD;

		if (g_package.length == 0) {
			i                  = 0;
			g_state            = WAITFOR_SYNC;
			g_receive_complete = true;
		}
		else {
			g_state = WAITFOR_DATA;
		}

		break;

	case WAITFOR_DATA:

		if (i < g_package.length) {
			g_package.payload[i] = byte;
			i++;
		}

		if (i >= g_package.length || i >= PROTOCOL_MAX_PAYLOAD) {
			g_state            = WAITFOR_SYNC;
			i                  = 0;
			g_receive_complete = true;
		}

		break;
	}

	return g_receive_complete;
}

static void copy_received_to(struct package *result)
{
	result->cmd    = g_package.cmd;
	result->length = g_package.length;

	for (uint8_t i = 0; i < g_package.length; i++) {
		result->payload[i] = g_package.payload[i];
	}
}

void protocol_waitfor_package(struct package *result)
{

	while (!g_receive_complete) {
		const uint8_t c = g_cfg.receive_byte_callback();
		parse_received_byte(c);
	}

	copy_received_to(result);
	protocol_reset();
}

void protocol_sync(void)
{
	for (uint8_t i = 0; i <= PROTOCOL_MAX_PAYLOAD; i++) {
		g_cfg.send_byte_callback(PROTOCOL_START_BYTE);
	}

	protocol_reset();
}

void protocol_reset(void)
{
	g_state            = WAITFOR_SYNC;
	g_receive_complete = false;
}

void protocol_send_package(uint8_t cmd, uint8_t length, const uint8_t *payload)
{
	g_cfg.send_byte_callback(PROTOCOL_START_BYTE);
	g_cfg.send_byte_callback(cmd);
	g_cfg.send_byte_callback(length);

	for (uint8_t i = 0; i < length; ++i) {
		g_cfg.send_byte_callback(payload[i]);
	}
}
