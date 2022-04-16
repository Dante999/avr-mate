#ifndef HANDHELD_H
#define HANDHELD_H

#include "avr-mate-core/commands.h"
#include "avr-mate-core/graphx.h"
#include "avr-mate-core/protocol.h"

struct handheld_dev {
	void (*publish_ready)();
	void (*publish_busy)();
	void (*draw_buffer)(const uint8_t *buffer);
	void (*get_buttons)(struct cp_buttons *buttons);
	struct protocol_dev protocol;
};

void handheld_init(struct handheld_dev *cfg);
void handheld_waitfor_instructions();

#endif /* HANDHELD_H */
