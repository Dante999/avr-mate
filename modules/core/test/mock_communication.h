#ifndef MOCK_COMMUNICATION_H
#define MOCK_COMMUNICATION_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define PRINT_DBG(msg)  (void)msg


#define SLEEP_UNTIL_NEXT_CHECK() usleep(100 * 1000)

static pthread_mutex_t g_mutex          = PTHREAD_MUTEX_INITIALIZER;
static volatile bool   g_handheld_ready = false;


static volatile uint8_t g_handheld_buffer;
static volatile uint8_t g_cartridge_buffer;
static volatile bool    g_handheld_buffer_touched  = false;
static volatile bool    g_cartridge_buffer_touched = false;

static inline void mock_handheld_send(uint8_t byte)
{
	while (g_cartridge_buffer_touched) {
		SLEEP_UNTIL_NEXT_CHECK();
	}

	PRINT_DBG("HANDHELD: send byte");
	g_cartridge_buffer         = byte;
	g_cartridge_buffer_touched = true;
}

static inline void mock_cartridge_send(uint8_t byte)
{
	while (g_handheld_buffer_touched) {
		SLEEP_UNTIL_NEXT_CHECK();
	}

	PRINT_DBG("CARTRIDGE: send byte");
	g_handheld_buffer         = byte;
	g_handheld_buffer_touched = true;
}

static inline uint8_t mock_handheld_receive()
{
	while (!g_handheld_buffer_touched) {
		PRINT_DBG("HANDHELD: buffer not touched, looping");
		SLEEP_UNTIL_NEXT_CHECK();
	}

	PRINT_DBG("HANDHELD: receive byte");
	g_handheld_buffer_touched = false;
	return g_handheld_buffer;
}

static inline uint8_t mock_cartridge_receive()
{
	while (!g_cartridge_buffer_touched) {
		PRINT_DBG("HANDHELD: buffer not touched, looping");
		SLEEP_UNTIL_NEXT_CHECK();
	}

	PRINT_DBG("HANDHELD: receive byte");
	g_cartridge_buffer_touched = false;
	return g_cartridge_buffer;
}

static inline void mock_set_handheld_ready()
{
	PRINT_DBG("HANDHELD: set_handheld_ready");
	pthread_mutex_lock(&g_mutex);
	g_handheld_ready = true;
	pthread_mutex_unlock(&g_mutex);
}

static inline void mock_set_handheld_busy()
{
	PRINT_DBG("HANDHELD: set_handheld_busy");
	pthread_mutex_lock(&g_mutex);
	g_handheld_ready = false;
	pthread_mutex_unlock(&g_mutex);
}

static inline void mock_block_until_handheld_ready()
{
	bool ready = false;

	while (!ready) {
		SLEEP_UNTIL_NEXT_CHECK();
		pthread_mutex_lock(&g_mutex);
		ready = g_handheld_ready;
		pthread_mutex_unlock(&g_mutex);
	}

	PRINT_DBG("CARTRIDGE: handheld is ready!");
}

static inline void mock_do_nothing()
{
	// do nothing...
}

#endif // MOCK_COMMUNICATION_H
