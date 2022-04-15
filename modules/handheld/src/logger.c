#include <avr/io.h>

#include "logger.h"

#define CR      "\r"
#define CRLF    "\r\n"
#define NEWLINE CRLF

// target Baudrate
#define BAUD 19200UL

// value for the UBRR Register
#define UBRR_VAL ((F_CPU + BAUD * 8) / (BAUD * 16) - 1)

// real Baudrate
#define BAUD_REAL (F_CPU / (16 * (UBRR_VAL + 1)))

// error Rate in promille, 1000 = 0K
#define BAUD_ERROR ((BAUD_REAL * 1000) / BAUD)

// check if the baudrate error is in range
#if ((BAUD_ERROR < 990) || (BAUD_ERROR > 1010))
#	error Rate of Baudrate Errors greater than 1%! Change system-clock or baudrate!
#endif

static void uart_init()
{
	UBRR0H = (uint8_t)UBRR_VAL >> 8;
	UBRR0L = (uint8_t)UBRR_VAL & 0xFF;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

	// Async (8-bit)(no-parity)(1-stop-bit)
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static void uart_putc(char c)
{
	while (!(UCSR0A & (1 << UDRE0))) {
		// waiting until sender is free...
	}

	UDR0 = c; // save character for sending
}

static void uart_puts(const char *s)
{
	while (*s != '\0') {
		uart_putc(*s);
		s++;
	}
}

void log_init()
{
	uart_init();
}

void log_println(const char *prefix, const char *msg)
{
#if LOG_LEVEL > LEVEL_OFF
	uart_puts(prefix);
	uart_puts(msg);
	uart_puts(NEWLINE);
#endif
}
