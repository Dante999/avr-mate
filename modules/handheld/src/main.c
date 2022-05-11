#include <avr/io.h>

#include "avr-mate-core/font5x7.h"
#include "avr-mate-core/handheld.h"

#include <string.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "logger.h"
#include "spi_slave.h"
#include "ssd1306.h"

#ifndef __AVR__
#	error "no avr compiler!"
#endif

#define DDRX_READY  DDRC
#define PORTX_READY PORTC
#define PINX_READY  PINC
#define BIT_READY   (1 << PC0)

static void publish_ready()
{
	PORTX_READY |= BIT_READY;
}

static void publish_busy()
{
	PORTX_READY &= ~BIT_READY;
}

static void send_byte(uint8_t byte)
{
	spi_slave_transceive(byte);
}

static uint8_t receive_byte()
{
	return spi_slave_transceive(CMD_NACK);
}

static void draw_buffer(const uint8_t *buffer)
{
	ssd1306_draw_buffer(buffer);
}

static void get_buttons(struct cp_buttons *buttons)
{
	static uint16_t old_states = 0xAAAA;

	buttons->states = old_states;
	old_states ^= 0xFFFF;
}

static void init()
{
	DDRX_READY |= BIT_READY;
	publish_busy();

	spi_slave_init();
	i2c_init();

	log_init();
	ssd1306_init();
}

int main(void)
{

	init();

	LOG_INFO("basic init done!");
	struct handheld_dev dev;

	dev.publish_ready         = publish_ready;
	dev.publish_busy          = publish_busy;
	dev.get_buttons           = get_buttons;
	dev.draw_buffer           = draw_buffer;
	dev.protocol.receive_byte = receive_byte;
	dev.protocol.send_byte    = send_byte;

	handheld_init(&dev);
	LOG_INFO("handheld init done!");

	while (1) {
		handheld_waitfor_instructions();
	}
}
