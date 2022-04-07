#ifndef TRANSCEIVER_PIPE_HPP
#define TRANSCEIVER_PIPE_HPP

#include "fifo_pipe.hpp"
#include "transceiver.hpp"

class transceiver_pipe_base {
protected:
	fifo_pipe_c m_handheld_to_cartridge{"/tmp/handheld_to_cartridge.fifo"};
	fifo_pipe_c m_cartridge_to_handheld{"/tmp/cartridge_to_handheld.fifo"};
};

class transceiver_pipe_handheld : public transceiver_handheld_c,
                                  public transceiver_pipe_base {

public:
	transceiver_pipe_handheld()
	{
		spdlog::info("Initializing pipe as handheld");
		m_handheld_to_cartridge.open_pipe(fifo_pipe_c::mode_e::WRITE);
		m_cartridge_to_handheld.open_pipe(fifo_pipe_c::mode_e::READ);
	}

	void send_byte(uint8_t byte) override
	{
		spdlog::debug("Handheld send byte: {}", byte);
		m_handheld_to_cartridge.write_bytes(&byte, 1);
	}

	uint8_t receive_byte() override
	{
		uint8_t result = 0;
		m_cartridge_to_handheld.read_bytes(&result, 1);
		spdlog::debug("Handheld received byte: {}", result);
		return result;
	}
};

class transceiver_pipe_cartridge : public transceiver_cartridge_c,
                                   public transceiver_pipe_base {

public:
	transceiver_pipe_cartridge()
	{
		spdlog::info("Initializing pipe as cartridge");
		m_handheld_to_cartridge.open_pipe(fifo_pipe_c::mode_e::READ);
		m_cartridge_to_handheld.open_pipe(fifo_pipe_c::mode_e::WRITE);
	}

	void send_byte(uint8_t byte) override
	{
		spdlog::debug("Cartridge send byte: {}", byte);
		m_cartridge_to_handheld.write_bytes(&byte, 1);
	}

	uint8_t receive_byte() override
	{
		uint8_t result = 0;
		m_handheld_to_cartridge.read_bytes(&result, 1);
		spdlog::debug("Cartridge received byte: {}", result);
		return result;
	}
};

#endif // TRANSCEIVER_PIPE_HPP
