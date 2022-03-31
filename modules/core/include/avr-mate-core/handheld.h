#ifndef HANDHELD_H
#define HANDHELD_H

#include "avr-mate-core/commands.h"
#include "avr-mate-core/graphx.h"
#include "avr-mate-core/protocol.h"

typedef void (*publish_status_cb)();
typedef void (*draw_buffer_cb)(const uint8_t *buffer);
typedef void (*get_buttons_cb)(struct cp_buttons *buttons);

struct handheld_dev {
	publish_status_cb   publish_ready;
	publish_status_cb   publish_busy;
	draw_buffer_cb      draw_buffer;
	get_buttons_cb      get_buttons;
	struct protocol_dev protocol;
};

void handheld_init(struct handheld_dev *cfg);
void handheld_waitfor_instructions();

#endif /* HANDHELD_H */
