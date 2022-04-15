#include <avr/io.h>

#include "avr-mate-core/handheld.h"
#include <util/delay.h>

#include "logger.h"
#include "spi_slave.h"

#define DDRX_READY  DDRC
#define PORTX_READY PORTC
#define PINX_READY  PINC
#define BIT_READY   (1 << PC0)

static void indicate_handheld_ready()
{
	PORTX_READY |= BIT_READY;
}

static void indicate_handheld_busy()
{
	PORTX_READY &= ~BIT_READY;
}

static void handheld_send_byte(uint8_t byte)
{
	spi_slave_transceive(byte);
}

static uint8_t handheld_receive_byte()
{
	return spi_slave_transceive(CMD_NACK);
}

static void handheld_draw_buffer(const uint8_t *buffer)
{
	(void)buffer;
}

static void handheld_get_buttons(struct cp_buttons *buttons)
{
	(void)buttons;
}

static void init()
{
	spi_slave_init();
	log_init();

	DDRX_READY |= BIT_READY;
}

int main(void)
{
	init();

	struct handheld_dev dev;

	dev.publish_ready                  = indicate_handheld_ready;
	dev.publish_busy                   = indicate_handheld_busy;
	dev.get_buttons                    = handheld_get_buttons;
	dev.draw_buffer                    = handheld_draw_buffer;
	dev.protocol.receive_byte_callback = handheld_receive_byte;
	dev.protocol.send_byte_callback    = handheld_send_byte;

	handheld_init(&dev);
	LOG_INFO("handheld init done!");

	while (1) {
		LOG_INFO("waiting for instructions");
		handheld_waitfor_instructions();
		_delay_ms(1000);
	}
}
