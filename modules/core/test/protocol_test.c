#include <unity_fixture.h>

#include <string.h>

#include "avr-mate-core/protocol.h"

#define DUMMY_CMD  ((uint8_t)0x10)
#define BUFFER_LEN ((uint8_t)255)

static uint8_t m_receive_buffer[BUFFER_LEN]  = {0x00};
static uint8_t m_transmit_buffer[BUFFER_LEN] = {0x00};

static uint8_t m_transmit_index = 0;
static uint8_t m_receive_index  = 0;

static struct protocol_dev m_cfg;


static void send_byte(uint8_t byte)
{
	m_transmit_buffer[m_transmit_index++] = byte;
}

static uint8_t receive_byte()
{
	return m_receive_buffer[m_receive_index++];
}

TEST_GROUP(protocol);

TEST_SETUP(protocol)
{
	m_cfg.send_byte_callback    = send_byte;
	m_cfg.receive_byte_callback = receive_byte;

	memset(m_receive_buffer, 0, BUFFER_LEN);
	memset(m_transmit_buffer, 0, BUFFER_LEN);
	m_transmit_index = 0;
	m_receive_index  = 0;

	protocol_init(&m_cfg);
}

TEST_TEAR_DOWN(protocol)
{
	protocol_reset(&m_cfg);
}

// #############################################################################

TEST(protocol, test_protocol_send_package)
{
	const uint8_t payload_length = 4;
	uint8_t       data[4]        = {1, 2, 3, 4};

	protocol_send_package(&m_cfg, DUMMY_CMD, payload_length, data);

	TEST_ASSERT_EQUAL(payload_length + 3, m_transmit_index);

	TEST_ASSERT_EQUAL(PROTOCOL_START_BYTE, m_transmit_buffer[0]);
	TEST_ASSERT_EQUAL(DUMMY_CMD, m_transmit_buffer[1]);
	TEST_ASSERT_EQUAL(payload_length, m_transmit_buffer[2]);

	for (uint8_t i = 0; i < payload_length; ++i) {
		TEST_ASSERT_EQUAL(data[i], m_transmit_buffer[i + 3]);
	}
}

TEST(protocol, test_protocol_waitfor_package)
{
	m_receive_buffer[0] = PROTOCOL_START_BYTE;
	m_receive_buffer[1] = DUMMY_CMD;
	m_receive_buffer[2] = 1;
	m_receive_buffer[3] = 2;

	TEST_ASSERT_EQUAL(0, m_receive_index);

	struct package received;
	memset(&received, 0x00, sizeof(received));

	protocol_waitfor_package(&m_cfg, &received);

	TEST_ASSERT_EQUAL(4, m_receive_index);

	TEST_ASSERT_EQUAL(DUMMY_CMD, received.cmd);
	TEST_ASSERT_EQUAL(1, received.length);
	TEST_ASSERT_EQUAL(2, received.payload[0]);
}

TEST(protocol, test_protocol_waitfor_package_with_zero_payload)
{
	uint8_t sync   = PROTOCOL_START_BYTE;
	uint8_t cmd    = DUMMY_CMD;
	uint8_t length = 0;

	m_receive_buffer[0] = sync;
	m_receive_buffer[1] = cmd;
	m_receive_buffer[2] = length;

	// check received data
	struct package received;
	memset(&received, 0x00, sizeof(received));

	protocol_waitfor_package(&m_cfg, &received);

	TEST_ASSERT_EQUAL(3, m_receive_index);
}

TEST(protocol, test_protocol_waitfor_package_with_max_payload)
{
	TEST_ASSERT_TRUE(PROTOCOL_MAX_PAYLOAD < 255);

	uint8_t sync   = PROTOCOL_START_BYTE;
	uint8_t cmd    = DUMMY_CMD;
	uint8_t length = PROTOCOL_MAX_PAYLOAD + 1;

	m_receive_buffer[0] = sync;
	m_receive_buffer[1] = cmd;
	m_receive_buffer[2] = length;

	// check received data
	struct package received;
	memset(&received, 0x00, sizeof(received));

	protocol_waitfor_package(&m_cfg,&received);

	TEST_ASSERT_EQUAL(PROTOCOL_MAX_PAYLOAD, received.length);
}

TEST(protocol, test_protocol_reset_index_when_command_is_start_byte)
{
	m_receive_buffer[0] = 3;
	m_receive_buffer[1] = 3;
	m_receive_buffer[2] = 3;
	m_receive_buffer[3] = PROTOCOL_START_BYTE;
	m_receive_buffer[4] = PROTOCOL_START_BYTE;
	m_receive_buffer[5] = PROTOCOL_START_BYTE;
	m_receive_buffer[6] = PROTOCOL_START_BYTE;
	m_receive_buffer[7] = PROTOCOL_START_BYTE;
	m_receive_buffer[8] = DUMMY_CMD;
	m_receive_buffer[9] = 0;

	// check received data
	struct package received;
	memset(&received, 0x00, sizeof(received));

	protocol_waitfor_package(&m_cfg, &received);

	TEST_ASSERT_EQUAL(DUMMY_CMD, received.cmd);
	TEST_ASSERT_EQUAL(0, received.length);
}

TEST(protocol, test_protocol_sync)
{
	memset(m_transmit_buffer, 0xFF, BUFFER_LEN);

	protocol_sync(&m_cfg);

	for (uint8_t i = 0; i <= PROTOCOL_MAX_PAYLOAD; i++) {
		TEST_ASSERT_EQUAL(PROTOCOL_START_BYTE, m_transmit_buffer[i]);
	}

	TEST_ASSERT_EQUAL(0xFF, m_transmit_buffer[PROTOCOL_MAX_PAYLOAD + 1]);
}

TEST_GROUP_RUNNER(protocol)
{
	RUN_TEST_CASE(protocol, test_protocol_send_package);
	RUN_TEST_CASE(protocol, test_protocol_waitfor_package);
	RUN_TEST_CASE(protocol,
	              test_protocol_waitfor_package_with_zero_payload);
	RUN_TEST_CASE(protocol, test_protocol_waitfor_package_with_max_payload);
	RUN_TEST_CASE(protocol,
	              test_protocol_reset_index_when_command_is_start_byte);
	RUN_TEST_CASE(protocol, test_protocol_sync);
}
