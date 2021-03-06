#include "cartridge/haw_cartridge.h"

#include "cartridge/haw_logger.h"
#include "cartridge/haw_utils.h"
#include "fifo.h"

#include "avr-mate-core/protocol.h"

#include <fcntl.h>

#define PIPE_HANDHELD_TO_CARTRIDGE "/tmp/handheld_to_cartridge.fifo"
#define PIPE_CARTRIDGE_TO_HANDHELD "/tmp/cartridge_to_handheld.fifo"
#define CARTRIDGE_EMULATE_DELAY_MS 30u

struct fifo_pipe g_pipe_cartridge_receive = { //
    .path   = PIPE_HANDHELD_TO_CARTRIDGE,
    .handle = 0,
    .size   = 1};

struct fifo_pipe g_pipe_cartridge_send = { //
    .path   = PIPE_CARTRIDGE_TO_HANDHELD,
    .handle = 0,
    .size   = 1};

static void haw_cartridge_send_byte(uint8_t byte)
{
	fifo_write(&g_pipe_cartridge_send, (char *)&byte);
}

static uint8_t haw_cartridge_receive_byte()
{
	char byte;
	fifo_read(&g_pipe_cartridge_receive, &byte);
	return (uint8_t)byte;
}

static void haw_cartridge_start_transmit()
{
	sleep_for_ms(CARTRIDGE_EMULATE_DELAY_MS);
}

static void haw_cartridge_end_transmit() {}

void haw_cartridge_init(struct cartridge_dev *dev)
{
	log_init();

	dev->before_transmit       = haw_cartridge_start_transmit;
	dev->after_transmit        = haw_cartridge_end_transmit;
	dev->protocol.send_byte    = haw_cartridge_send_byte;
	dev->protocol.receive_byte = haw_cartridge_receive_byte;

	fifo_open(&g_pipe_cartridge_receive, O_RDONLY);
	fifo_open(&g_pipe_cartridge_send, O_WRONLY);

	cartridge_init(dev);
	cartridge_sync_with_handheld();

	enum result ret = cartridge_ping();

	if (ret != RESULT_OK) {
		LOG_WARNING("Result of ping was not pong");
	}
	else {
		LOG_INFO("Ping successful");
	}

	cartridge_clear_buffer();
	cartridge_draw_buffer();
}
