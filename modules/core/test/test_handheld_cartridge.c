#include <unity_fixture.h>

#include <pthread.h>
#include <unistd.h>

#include "avr-mate-core/cartridge.h"
#include "avr-mate-core/handheld.h"

#include "mock_communication.h"

static struct handheld_dev  g_handheld_cfg;
static struct cartridge_dev g_cartridge_cfg;
static pthread_t            g_handheld_thread;

void *thr_handheld(void *args)
{
	handheld_init((struct handheld_dev *)args);
	handheld_waitfor_instructions();

	return args;
}

TEST_GROUP(handheld_cartridge);

TEST_SETUP(handheld_cartridge)
{
	g_handheld_cfg.publish_busy                   = mock_set_handheld_busy;
	g_handheld_cfg.publish_ready                  = mock_set_handheld_ready;
	g_handheld_cfg.protocol.send_byte_callback    = mock_handheld_send;
	g_handheld_cfg.protocol.receive_byte_callback = mock_handheld_receive;

	g_cartridge_cfg.start_transmit_cb = mock_block_until_handheld_ready;
	g_cartridge_cfg.end_transmit_cb   = mock_do_nothing;
	g_cartridge_cfg.protocol.send_byte_callback    = mock_cartridge_send;
	g_cartridge_cfg.protocol.receive_byte_callback = mock_cartridge_receive;

	// handheld_init(&g_handheld_cfg);
	cartridge_init(&g_cartridge_cfg);

	pthread_create(&g_handheld_thread, NULL, thr_handheld,
	               (void *)&g_handheld_cfg);
}

TEST_TEAR_DOWN(handheld_cartridge)
{
	pthread_join(g_handheld_thread, NULL);
}

TEST(handheld_cartridge, cmd_ping_pong)
{
	TEST_ASSERT_EQUAL(RESULT_OK, cartridge_ping());
}

TEST_GROUP_RUNNER(handheld_cartridge)
{
	RUN_TEST_CASE(handheld_cartridge, cmd_ping_pong);
}
