#ifndef AVRMATE_PROTOCOL_H
#define AVRMATE_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#define PROTOCOL_START_BYTE  0x00
#define PROTOCOL_MAX_PAYLOAD 50

typedef void (*send_byte_cb)(uint8_t);
typedef uint8_t (*receive_byte_cb)();

enum state { WAITFOR_SYNC, WAITFOR_CMD, WAITFOR_LENGTH, WAITFOR_DATA };

struct package {
	uint8_t cmd;
	uint8_t length;
	uint8_t payload[PROTOCOL_MAX_PAYLOAD];
};

struct protocol_internals {
	struct package package;
	bool           receive_complete;
	enum state     state;
};

struct protocol_dev {
	send_byte_cb              send_byte_callback;
	receive_byte_cb           receive_byte_callback;
	struct protocol_internals internals;
};

void protocol_init(struct protocol_dev *cfg);
void protocol_sync(struct protocol_dev *cfg);
void protocol_reset(struct protocol_dev *cfg);
void protocol_send_package(struct protocol_dev *cfg, uint8_t cmd,
                           uint8_t length, const uint8_t *payload);
void protocol_waitfor_package(struct protocol_dev *cfg, struct package *result);

#endif /* AVRMATE_PROTOCOL_H */
