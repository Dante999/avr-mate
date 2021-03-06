#include "cartridge/haw_cartridge.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "cartridge/haw_logger.h"

#define SPI_DDRX     DDRB
#define SPI_PORTX    PORTB
#define SPI_BIT_MOSI (1 << PB3)
#define SPI_BIT_MISO (1 << PB4)
#define SPI_BIT_SCK  (1 << PB5)
#define SPI_BIT_SS   (1 << PB2)

#define DDRX_READY  DDRC
#define PORTX_READY PORTC
#define PINX_READY  PINC
#define BIT_READY   (1 << PC0)

// valid values: 2, 4, 8, 16, 32, 64, 128
#define SPI_CLK_RATE 2

static void spi_delay(void)
{
	_delay_ms(40);
}

static void spi_enable_slave(void)
{
	SPI_PORTX &= ~SPI_BIT_SS;
}

static void spi_disable_slave(void)
{
	SPI_PORTX |= SPI_BIT_SS;
}

static void spi_init(void)
{

	SPI_DDRX |= SPI_BIT_MOSI | SPI_BIT_SCK | SPI_BIT_SS;

	SPDR = 0x00; // preset data register

	SPCR |= (1 << SPE) | (1 << MSTR);

#if SPI_CLK_RATE == 4
	// default
#elif SPI_CLK_RATE == 16
	SPCR |= (1 << SPR0);
#elif SPI_CLK_RATE == 64
	SPCR |= (1 << SPR1);
#elif SPI_CLK_RATE == 128
	SPCR |= (1 << SPR1) | (1 << SPR0);
#elif SPI_CLK_RATE == 2
	SPCR |= (1 << SPI2X);
#elif SPI_CLK_RATE == 8
	SPCR |= (1 << SPI2X) | (1 << SPR0);
#elif SPI_CLK_RATE == 32
	SPCR |= (1 << SPI2X) | (1 << SPR1);
#elif SPI_CLK_RATE == 64
	SPCR |= (1 << SPI2X) | (1 << SPR1) | (1 << SPR1);

#endif

	spi_disable_slave();
	spi_delay();
	spi_enable_slave();
}

static uint8_t spi_transceive(uint8_t data)
{

	SPDR = data;

	while (!(SPSR & (1 << SPIF))) {
		// Wait for reception complete
	}

	return SPDR;
}

static void haw_cartridge_send_byte(uint8_t byte)
{
	(void)spi_transceive(byte);
}

static uint8_t haw_cartridge_receive_byte()
{
	return spi_transceive(CMD_ACK);
}

static void loop_until_handheld_ready(void)
{
	while (!(PINX_READY & BIT_READY)) {
		// wait...
	}
}

static void haw_cartridge_start_transmit(void)
{
	loop_until_handheld_ready();
}

static void haw_cartridge_end_transmit(void)
{
	spi_disable_slave();
	spi_delay();
	spi_enable_slave();
}

void haw_cartridge_init(struct cartridge_dev *dev)
{
	log_init();
	LOG_INFO("logger initialized");

	loop_until_handheld_ready();
	LOG_INFO("detect handheld is ready");

	spi_init();
	LOG_INFO("spi initialized");

	dev->before_transmit       = haw_cartridge_start_transmit;
	dev->after_transmit        = haw_cartridge_end_transmit;
	dev->protocol.send_byte    = haw_cartridge_send_byte;
	dev->protocol.receive_byte = haw_cartridge_receive_byte;

	cartridge_init(dev);

	LOG_INFO("syncing with handheld");
	cartridge_sync_with_handheld();

	LOG_INFO("sending ping to handheld...");
	enum result ret = RESULT_NOK;

	while (ret != RESULT_OK) {
		ret = cartridge_ping();

		if (ret != RESULT_OK) {
			LOG_WARNING("ping failed");
			cartridge_sync_with_handheld();
		}
		else {
			LOG_INFO("ping successful");
		}
	}

	cartridge_clear_buffer();
	cartridge_draw_buffer();

	LOG_INFO("cartridge initialization done!");
}
