#ifndef AVR_BOY_EMULATOR_H
#define AVR_BOY_EMULATOR_H

#include <memory>

#include <SFML/Graphics/Image.hpp>

#include "transceiver_pipe.hpp"

extern "C" {
#include "avr-mate-core/commands.h"
#include "avr-mate-core/handheld.h"
}

class handheld_emulator_c {

      private:
	transceiver_pipe_handheld m_transceiver;
	sf::Image &               m_image;
	struct handheld_dev       m_handheld_dev;

      public:
	handheld_emulator_c(sf::Image &img);

	void do_work();

	void draw_buffer();
	void get_buttons(struct cp_buttons *buttons);

	void send_byte(uint8_t byte)
	{
		m_transceiver.send_byte(byte);
	}

	uint8_t receive_byte()
	{
		return m_transceiver.receive_byte();
	}
};

#endif /* AVR_BOY_EMULATOR_H */
