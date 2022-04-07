#include "gtest/gtest.h"

#include <thread>

#include "avr-boy-emulator/fifo_pipe.hpp"

constexpr const char *path     = "/tmp/fifo_test.pipe";
static const uint8_t  expected = 20;
static uint8_t        actual   = 0;

void thread_read_byte()
{
	fifo_pipe_c fifo_reader(path);
	fifo_reader.open_pipe(fifo_pipe_c::mode_e::READ);
	fifo_reader.read_bytes(&actual, 1);
	fifo_reader.close_pipe();
}

void thread_write_byte()
{
	fifo_pipe_c fifo_writer(path);
	fifo_writer.open_pipe(fifo_pipe_c::mode_e::WRITE);
	fifo_writer.write_bytes(&expected, 1);
	fifo_writer.close_pipe();
}

TEST(fifo_test, test_write)
{

	std::thread thread1(thread_read_byte);
	std::thread thread2(thread_write_byte);

	thread1.join(); // pauses until first finishes
	thread2.join(); // pauses until second finishes

	ASSERT_EQ(expected, actual);
}
