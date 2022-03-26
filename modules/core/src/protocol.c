#include "avr-mate-core/protocol.h"

#include <stdio.h>

void protocol_init(struct protocol_dev *cfg)
{
	cfg->internals.state            = WAITFOR_SYNC;
	cfg->internals.receive_complete = false;
	protocol_reset(cfg);
}

static bool parse_received_byte(struct protocol_dev *cfg, uint8_t byte)
{
	static uint8_t i = 0;

	switch (cfg->internals.state) {
	case WAITFOR_SYNC:

		if (byte == PROTOCOL_START_BYTE) {
			i = 0;

			cfg->internals.state            = WAITFOR_CMD;
			cfg->internals.receive_complete = false;
		}

		break;

	case WAITFOR_CMD:

		if (byte != PROTOCOL_START_BYTE) {
			cfg->internals.package.cmd = byte;
			cfg->internals.state       = WAITFOR_LENGTH;
		}

		break;

	case WAITFOR_LENGTH:

		cfg->internals.package.length = byte;

		if (cfg->internals.package.length > PROTOCOL_MAX_PAYLOAD)
			cfg->internals.package.length = PROTOCOL_MAX_PAYLOAD;

		if (cfg->internals.package.length == 0) {
			i = 0;

			cfg->internals.state            = WAITFOR_SYNC;
			cfg->internals.receive_complete = true;
		}
		else {
			cfg->internals.state = WAITFOR_DATA;
		}

		break;

	case WAITFOR_DATA:

		if (i < cfg->internals.package.length) {
			cfg->internals.package.payload[i] = byte;
			i++;
		}

		if (i >= cfg->internals.package.length ||
		    i >= PROTOCOL_MAX_PAYLOAD) {
			i = 0;

			cfg->internals.state            = WAITFOR_SYNC;
			cfg->internals.receive_complete = true;
		}

		break;
	}

	return cfg->internals.receive_complete;
}

static void copy_received_to(struct protocol_dev *cfg, struct package *result)
{
	result->cmd    = cfg->internals.package.cmd;
	result->length = cfg->internals.package.length;

	for (uint8_t i = 0; i < cfg->internals.package.length; i++) {
		result->payload[i] = cfg->internals.package.payload[i];
	}
}

void protocol_waitfor_package(struct protocol_dev *cfg, struct package *result)
{

	while (!cfg->internals.receive_complete) {
		const uint8_t c = cfg->receive_byte_callback();
		parse_received_byte(cfg, c);
	}

	copy_received_to(cfg, result);
	protocol_reset(cfg);
}

void protocol_sync(struct protocol_dev *cfg)
{
	for (uint8_t i = 0; i <= PROTOCOL_MAX_PAYLOAD; i++) {
		cfg->send_byte_callback(PROTOCOL_START_BYTE);
	}

	protocol_reset(cfg);
}

void protocol_reset(struct protocol_dev *cfg)
{
	cfg->internals.state            = WAITFOR_SYNC;
	cfg->internals.receive_complete = false;
}

void protocol_send_package(struct protocol_dev *cfg, uint8_t cmd,
                           uint8_t length, const uint8_t *payload)
{
	cfg->send_byte_callback(PROTOCOL_START_BYTE);
	cfg->send_byte_callback(cmd);
	cfg->send_byte_callback(length);

	for (uint8_t i = 0; i < length; ++i) {
		cfg->send_byte_callback(payload[i]);
	}
}
